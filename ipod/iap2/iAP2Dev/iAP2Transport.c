/****************************************************************************
 *   FileName    : iAP2Transport.c
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall
constitute any express or implied warranty of any kind, including without limitation, any warranty
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright
or other third party intellectual property right. No warranty is made, express or implied,
regarding the information's accuracy, completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of
or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/Log.h>
#include <fcntl.h>
#include <poll.h>

#include <iAP2Log.h>
#include "iPod_HID.h"

#include "iAP2Transport.h"
//#include "iAP2Callback.h"

#define LOG_TAG "iAP2"
#define DEV_IAP2_URL            "/dev/iap2"

enum
{
    IAP2_TRANSPORT_USB = 0,
    IAP2_TRANSPORT_EXTERNAL = 1
};

extern const uint8_t   kIap2PacketDetectData[];
extern uint32_t        kIap2PacketDetectDataLen;

void* iAP2TransportRecvThread(void* param);
iAP2Transport_t* iAP2TransportCreate(
                                                    iAP2LinkRunLoop_t* linkRunLoop, 
                                                    BOOL bDevice,
                                                    iAP2TransportAttachedCB_t loopAttachedCb,
                                                    iAP2TransportRecvCB_t receiveCb,
                                                    iAP2Notify_callback noti_cb,
                                                    iAP2NotiTransportError transErr_cb,
                                                    int transportType,
                                                    iAP2ExtWriteAttached_callback attached_cb,
                                                    iAP2ExtDoWrite_callback doWrite_cb,
                                                    iAP2ExtWriteDetached_callback detached_cb
                                                                    )
{
    iAP2Transport_t* transport = (iAP2Transport_t*) malloc(
    		sizeof(iAP2Transport_t));
   // assert(transport);
    if(transport == NULL){
        return transport;
    }

    memset(transport,0,sizeof(iAP2Transport_t));
	if(g_handle!=NULL)
    transport->fd = g_handle->fd_hidraw;

    // start uevent monitor

    transport->bDevice = bDevice;
    transport->linkRunLoop = linkRunLoop;
    transport->bDestroy = FALSE;
    transport->packet = NULL;
    transport->bufferSize = MAX_IAP2_TRANSPORT_BUF_SIZE;
    transport->buffer = (uint8_t*) malloc(transport->bufferSize);
    transport->loopAttachedCb = loopAttachedCb;
    transport->receiveCb = receiveCb;
    transport->noti_cb = noti_cb;
    transport->transErr_cb = transErr_cb;
    transport->transportType = transportType;
    transport->btAttached = 0;
    transport->pEnv = NULL;
    transport->attached_cb = attached_cb;
    transport->doWrite_cb = doWrite_cb;
    transport->detached_cb = detached_cb;
   // assert(transport->buffer);
    pthread_mutex_init(&transport->syncMutex, NULL);
    pthread_cond_init(&transport->syncCond, NULL); 
    pthread_mutex_init(&transport->openMutex, NULL);

    if(transport->transportType == IAP2_TRANSPORT_USB)
    {  
        pthread_mutex_lock(&transport->syncMutex);
        pthread_create(&transport->thread, NULL, iAP2TransportRecvThread,
        		transport);
        pthread_cond_wait(&transport->syncCond, &transport->syncMutex);
        pthread_mutex_unlock(&transport->syncMutex);
    }
    else
    {

        if(transport->loopAttachedCb)
            (*transport->loopAttachedCb)(transport->linkRunLoop);
    }
     if(transport->loopAttachedCb)
            (*transport->loopAttachedCb)(transport->linkRunLoop);
    return transport;
}

void iAP2TransportDestroy(iAP2Transport_t* transport) 
{
    //ALOGI("%s %d  \n",__FUNCTION__,__LINE__);
    if(transport == NULL)
    {
        iAP2LogDbg("%s %d  transport is already NULL\n",__FUNCTION__,__LINE__);
        return;
    }
    pthread_mutex_lock(&transport->openMutex);
    transport->bDestroy = TRUE;
    pthread_mutex_unlock(&transport->openMutex);
    if(transport->transportType == IAP2_TRANSPORT_USB)
    {
        pthread_join(transport->thread, NULL);
        ALOGI("%s %d  after pthread_join fd %d bRead %d bWrite %d\n",
                                                                __FUNCTION__,
                                                                __LINE__,
                                                                transport->fd,
                                                                transport->bReading,
                                                                transport->bWriting);
       // if (transport->fd != -1) {
          //  iAP2TransportClose(transport);
          //  iAP2LogDbg("%s %d close\n",__FUNCTION__,__LINE__);
        //}
    }
    pthread_cond_destroy(&transport->syncCond);
    pthread_mutex_destroy(&transport->syncMutex);
    pthread_mutex_destroy(&transport->openMutex);
    
    if(transport->buffer)
        free(transport->buffer);
    free(transport);
}

BOOL iAP2TransportOpen(iAP2Transport_t* transport) 
{
    BOOL ret = FALSE;
    BOOL sendAttached = FALSE;
    pthread_mutex_lock(&transport->openMutex);
    if(transport->bDestroy == FALSE)
    {
        if(transport->fd < 0)
        {
        chmod(DEV_IAP2_URL,0666);
            int fd = open(DEV_IAP2_URL, O_RDWR|O_NONBLOCK);
            if (fd < 0) 
            {
                if((transport->openFailCount % 20) == 0)
                    ALOGI(" %s open failed fd = %d openFailCount %d\n",__FUNCTION__,transport->fd,transport->openFailCount);
                transport->openFailCount++;
            }
            else
            {
                transport->fd = fd;
                ALOGI(" %s fd = %d  +++++++q+++++++\n",__FUNCTION__,transport->fd);
                sendAttached = TRUE;
                ret = TRUE;
            }
        }
        else
            ret = TRUE;
    }
    pthread_mutex_unlock(&transport->openMutex);
    
    // Send init Detect packet
    if(sendAttached == TRUE)
    {
        if(transport->loopAttachedCb)
        	{
        	
            (*transport->loopAttachedCb)(transport->linkRunLoop);
        	}
    }
    
    return ret;
}

void iAP2TransportClose(iAP2Transport_t* transport) 
{
    ALOGI(" %s fd = %d \n",__FUNCTION__,transport->fd);
    pthread_mutex_lock(&transport->openMutex);
   /* if (transport->fd != -1) {
        iAP2LogDbg(" %s file close ----------------- \n",__FUNCTION__);
        close(transport->fd);
    }

    transport->fd = -1;
	*/
	if(g_handle)
	IPOD_HID_CloseHIDChannel(g_handle);
   g_handle = NULL;
    pthread_mutex_unlock(&transport->openMutex);
}

int32_t iAP2TransportSend(iAP2Transport_t* transport, uint8_t* buffer,uint32_t len) 
{
    int32_t ret = 0;
	uint32_t r = 0;
	for(r = 0;r<len;r++)
		{
		// ALOGD("%s  %d transport->fd %d buffer[%d]:0x%02x\n ",__FUNCTION__,__LINE__,transport->fd,r,buffer[r]);
		}

    if(transport->transportType == IAP2_TRANSPORT_USB)
    {
      //  if (transport->fd == -1) {
           // ALOGI("%s, %s(%d) tid %d", __FUNCTION__, __FILE__, __LINE__,gettid());
            //return -1;
       // }

        /*
        The accessory does not need to send another ZLP data transfer on the interrupt IN endpoint
        as long as it	continues to return the maximum USB packet size in response to each bulk IN read
        */
        //ioctl(transport->fd, 1, 0);
        transport->bWriting = TRUE;
	   // pthread_mutex_lock(&transport->openMutex);
		if(g_handle!=NULL)
		  ret = IPOD_HID_SendPacket(g_handle,buffer, len);
		// pthread_mutex_unlock(&transport->openMutex);
       // ret = write(transport->fd, buffer, len);
        transport->bWriting = FALSE;
        if (ret < 0) {
            ALOGI("%s(%d) write error ret = %d errno = %d", __FUNCTION__, __LINE__,ret,errno);
        }
		else
			{
			//ALOGI("%s(%d) write ok ret = %d ", __FUNCTION__, __LINE__,ret);
			}
    }
    else
    {
        if(transport->btAttached == 0)
        {
            if(transport->attached_cb != NULL)
            {
                transport->pEnv = transport->attached_cb(transport->pEnv,&transport->btAttached);
            }
        }
        if(transport->doWrite_cb != NULL)
            ret = transport->doWrite_cb(transport->pEnv,(void *)buffer, len);
		#ifdef USE_TCC_IAP2
        iAP2LogDump ((const char* )buffer,len);
        #endif
        if(transport->btAttached)
        {
            if(transport->detached_cb != NULL)
                transport->detached_cb(transport->btAttached);
            transport->btAttached = 0;
        }

    }
    return ret;
}

void* iAP2TransportRecvThread(void* param) 
	{
		iAP2Transport_t* transport = (iAP2Transport_t*) param;
		int len =0;
		uint8_t *tmp_buffer = NULL;
		uint8_t *buffer = transport->buffer;
		int bufferLen = transport->bufferSize;
		BOOL fNextDump = FALSE;

		if(bufferLen > 0)
			{
			tmp_buffer = (uint8_t *)malloc(bufferLen+2);
			memset(tmp_buffer,0,bufferLen+2);
			}
	

	
		pthread_mutex_lock(&transport->syncMutex);
		pthread_cond_signal(&transport->syncCond);
		pthread_mutex_unlock(&transport->syncMutex);
	
		while (transport->bDestroy == FALSE ) {
			
			usleep(1000);

			//if (iAP2TransportOpen(transport) == FALSE) {
				//usleep(1000 * 5);
				//continue;
			//}
	
#if 0
			{
				struct pollfd pfd[1];
				int timeout;
				int nfd = 0;
				pfd[0].fd = transport->fd;
				pfd[0].events = POLLIN | POLLERR | POLLRDNORM ;
				pfd[0].revents = 0; 	  
				timeout = 500; // ms
				iAP2LogDbg(" %s %d wait poll event\n",__FUNCTION__,__LINE__);
				nfd = poll(pfd, 1, timeout);
				if (nfd <= 0){ 
					iAP2LogDbg(" %s %d timeout nfd = %d \n",__FUNCTION__,__LINE__,nfd);
					continue;
				}
				iAP2LogDbg(" %s %d nfd = %d \n",__FUNCTION__,__LINE__,nfd);
			}
#endif
	
			/* need timeout */
			//alarm(3);
			//iAP2LogDbg("Transport Thread read \n");
			pthread_mutex_lock(&transport->openMutex);
			if(transport->bDestroy == FALSE)
			{
				transport->bReading = TRUE;
				//len = read(transport->fd, buffer, bufferLen);
				if(g_handle!=NULL)
					{
				len = iPOD_HID_ReadHID(g_handle,tmp_buffer, bufferLen+2);
				if(len > 2)
					memcpy(buffer,&tmp_buffer[2],len-2);
				len = len -2;
				
					}
					else{
						  sleep(1);
						  pthread_mutex_unlock(&transport->openMutex);
						  	
						  	continue;
						}
				transport->bReading = FALSE;
			}
			else
			{
		//	ALOGI(" %s %d iAP2TransportClose\n",__FUNCTION__,__LINE__);
				pthread_mutex_unlock(&transport->openMutex);
			//	iAP2TransportClose(transport);
				transport->bDestroy = TRUE;
				goto done;
			}
			pthread_mutex_unlock(&transport->openMutex);
			//iAP2LogDbg("Transport Thread read done len %d\n",len);
			
			//if(len < 0 && transport->bDestroy == TRUE){
			//	ALOGI(" %s %d return error after disconnect \n",__FUNCTION__,__LINE__);
			//}
			
			if (len < 0) {
				//ALOGI(" %s %d return len %d \n",__FUNCTION__,__LINE__,len);
				if (errno == EIO || errno == ENODEV) {
					//ALOGE(" %s %d return errno %d from the read function\n",__FUNCTION__,__LINE__,errno);
					if(transport->bDestroy == FALSE){
						if(transport->transErr_cb != NULL)
							transport->transErr_cb((void *)transport,TRANS_ERR_EIO);
					}
					//iAP2TransportClose(transport);
					iAP2LogDbg("Transport Thread intruppt \n");
					transport->bDestroy = TRUE;
					goto done;
				}				
				/*
				if (errno == EINTR) {
					iAP2LogDbg("Transport Thread intruppt \n");
					transport->bDestroy = TRUE;
					continue;
				}
				*/
				iAP2LogDbg("Transport Thread read errno %d\n",errno);
			}
	
			//alarm(0);
        #if 0
			{
				//if(buffer[0] == 0xFF && buffer[1] == 0x5A && buffer[9] == 0x40 && buffer[10] == 0x40 && buffer[13] == 0x50 && buffer[14] == 0x01 ){
				//	  if(buffer[12] > 0x60)
					{
						iAP2LogTypeBinaryProc(0,buffer, len);
						fNextDump = TRUE;
					}
				//}else if(fNextDump){
				//	  fNextDump = FALSE;
				//	  iAP2LogTypeBinaryProc(0,buffer, len);
				//}
			}
        #endif
	
			/* parse DATA & Packet */
			if(transport->bDestroy == FALSE && len > 0){
				if(transport->receiveCb)
					(*transport->receiveCb)((void *)transport, buffer, len);
			}
		}
		iAP2TransportClose(transport);
	done:	
		iAP2LogDbg("iAP2TransportRecvThread stop by flag \n");
		if(tmp_buffer!=NULL)
			free(tmp_buffer);
		pthread_exit((void *) 0);
		
		return 0;
	}


