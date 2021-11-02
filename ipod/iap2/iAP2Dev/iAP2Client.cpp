/****************************************************************************
 *   FileName    : iAP2Client.c
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
#include <cutils/properties.h>
#include <binder/Parcel.h>
#include <fcntl.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include <iAP2Log.h>

#include "iAP2Time.h"
#include "iAP2Packet.h"
#include "iAP2LinkRunLoop.h"
#include "iAP2Link.h"
#include "iAP2LinkPrivate.h"

#include "iAP2Transport.h"
#include "iAP2Client.h"
#include "iAP2Transport.h"
#include "iAP2Control.h"
#include "iAP2ListArray.h"
#include "iAP2SessionControl.h"
#include "iAP2Coprocessor.h"
#include "iAP2Rsd.h"
#include "iAP2EventQueue.h"
#include "iAP2Callback.h"
#include "iAP2_jni.h"
#include "iPod_HID.h"
#include "iAP2FileTransfer.h"
#include "iAP2BuffPool.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "iAP2_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define LOG_TAG "iAP2"



using namespace IAP2JNIH;

namespace android{
iAP2Client_t* g_iap2handle;
iAP2FileTransferDataBuffer_st * g_iap2FtDataBufferList = NULL;
iAP2TransferData_st * g_iap2Sesion1DataBufferList = NULL;

iAP2TransferData_st * g_iAP2FileTransferData_st = NULL;

pthread_mutex_t lockIap2FtDataBufferList; 
pthread_mutex_t lockIap2Session1DataBufferList;

pthread_mutex_t lockIap2FtDataRecieve; 


BOOL exitSession1DataThread ;
BOOL exitFileTransferDataThread ;

void iAP2DispatchFtData(iAP2FileTransferSetupData_t * pFTDataBuffer );

	



iAP2Callback_t gIap2CallbackFunctions = {
	#ifdef USE_TCC_IAP2
    iAP2SessCtrlMalloc,
    iAP2SessCtrlFree,
    iAP2SessCtrlLinkPacketDebugInit,
    iAP2SessCtrlLinkPacketAdd,
    iAP2SessCtrlLinkPacketDel,
    iAP2SessCtrlLinkPacketDebugPrint
    #endif
};
void initLockIap2FtDataBufferList()
{
	pthread_mutex_init(&lockIap2FtDataBufferList, NULL);	
	pthread_mutex_init(&lockIap2FtDataRecieve, NULL);
	pthread_mutex_init(&lockIap2Session1DataBufferList, NULL);
	
}

void addIap2Session1DataBuffer(struct iAP2Link_st* link,const uint8_t * buf,uint32_t dataSize)	
{
  pthread_mutex_lock(&lockIap2Session1DataBufferList);
	 iAP2TransferData_st * temBuf = (iAP2TransferData_st *)malloc(sizeof(iAP2TransferData_st));
	 
	 temBuf->data = (uint8_t *)malloc(dataSize);
	
	 memcpy(temBuf->data,buf,dataSize);
	 temBuf->link = link;
	 temBuf->size = dataSize;
	  temBuf->next = NULL;
	 if(g_iap2Sesion1DataBufferList == NULL)
		{
		 g_iap2Sesion1DataBufferList = temBuf;
		}
	 else
		{
		  iAP2TransferData_st * temBuf2 = g_iap2Sesion1DataBufferList;
		//	LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);
		  while(temBuf2->next!=NULL)
			{
			 temBuf2 = temBuf2->next;
			}
		  temBuf2->next = temBuf;
		}
	  pthread_mutex_unlock(&lockIap2Session1DataBufferList);
}
iAP2TransferData_st * getIap2Session1DataBufferToFree() //malloc memery,must free after used ,when get ,must be free(g_iap2FtDataBufferList->setupData.fileSetupData) ,else if 
//it will cause a memery leak;
{

   pthread_mutex_lock(&lockIap2Session1DataBufferList);
   if(g_iap2Sesion1DataBufferList == NULL)
   	{
   	 pthread_mutex_unlock(&lockIap2Session1DataBufferList);
	 return NULL;
   	}
     iAP2TransferData_st * temBuf = g_iap2Sesion1DataBufferList;
	 g_iap2Sesion1DataBufferList = g_iap2Sesion1DataBufferList->next;
   
   pthread_mutex_unlock(&lockIap2Session1DataBufferList);
   return temBuf;
}

iAP2TransferData_st * getIap2Session1DataBuffer() //malloc memery,must free after used ,when get ,must be free(g_iap2FtDataBufferList->setupData.fileSetupData) ,else if 
//it will cause a memery leak;
{

   pthread_mutex_lock(&lockIap2Session1DataBufferList);
   if(g_iap2Sesion1DataBufferList == NULL)
   	{
   	 pthread_mutex_unlock(&lockIap2Session1DataBufferList);
	 return NULL;
   	}
     //iAP2TransferData_st * temBuf = g_iap2Sesion1DataBufferList;
	// g_iap2Sesion1DataBufferList = g_iap2Sesion1DataBufferList->next;

	 ///
	   iAP2TransferData_st * retBuf  = (iAP2TransferData_st *)malloc(sizeof(iAP2TransferData_st));
   
     iAP2TransferData_st * temBuf = g_iap2Sesion1DataBufferList;
	  memcpy(retBuf,temBuf,sizeof(iAP2TransferData_st));
	  retBuf->link = temBuf->link;
	  if(temBuf->data!=NULL)
	  {
	    retBuf->data = (uint8_t*)malloc(temBuf->size);
		memcpy(retBuf->data,temBuf->data,temBuf->size);
		free(temBuf->data);
	  }
	  
	 g_iap2Sesion1DataBufferList = g_iap2Sesion1DataBufferList->next;
	 free(temBuf);
	 ////
   
   pthread_mutex_unlock(&lockIap2Session1DataBufferList);
   return retBuf;
}
void freeAllIap2Session1DataBuffer()
{
 iAP2TransferData_st *p =getIap2Session1DataBufferToFree();
  while(p!=NULL)
  	{
  	
   if(p->data)
   	{
   
   	 free(p->data);
	 p->data= NULL;
   	}
      free(p);
  	  p = NULL;
  	  p =getIap2Session1DataBufferToFree();
  	}

   exitSession1DataThread = true;
}
void addIap2FtReceiveDataBuffer(struct iAP2Link_st* link,const uint8_t * buf,uint32_t dataSize)
{

  pthread_mutex_lock(&lockIap2FtDataRecieve);
 //
	 iAP2TransferData_st * temBuf = (iAP2TransferData_st *)malloc(sizeof(iAP2TransferData_st));
	 
	 temBuf->data = (uint8_t *)malloc(dataSize);
	
	 memcpy(temBuf->data,buf,dataSize);
	 temBuf->link = link;
	 temBuf->size = dataSize;
	  temBuf->next = NULL;
	  
	// memcpy(temBuf,&buf,sizeof(iAP2FileTransferDataBuffer_st));
	// temBuf->buf = (u8 *)malloc(buf.bufLen);
	//temBuf->setupData = buf;
	// 
	 //memcpy(temBuf->buf,buf.buf,buf.bufLen);
	 if(g_iAP2FileTransferData_st == NULL)
		{
		 g_iAP2FileTransferData_st = temBuf;
		}
	 else
		{
		  iAP2TransferData_st * temBuf2 = g_iAP2FileTransferData_st;
		  while(temBuf2->next!=NULL)
			{
			 temBuf2 = temBuf2->next;
			}
		  temBuf2->next = temBuf;
		}
	  pthread_mutex_unlock(&lockIap2FtDataRecieve);
	
}

iAP2TransferData_st * getIap2FtReceiveDataBufferToFree() //malloc memery,must free after used ,when get ,must be free(g_iap2FtDataBufferList->setupData.fileSetupData) ,else if 
//it will cause a memery leak;
{
   //	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);

   pthread_mutex_lock(&lockIap2FtDataRecieve);
   
  // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
   if(g_iAP2FileTransferData_st == NULL)
   	{
   	 pthread_mutex_unlock(&lockIap2FtDataRecieve);
	 return NULL;
   	}
   	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
     iAP2TransferData_st * temBuf = g_iAP2FileTransferData_st;
		LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	 g_iAP2FileTransferData_st = g_iAP2FileTransferData_st->next;
	 	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
   
   pthread_mutex_unlock(&lockIap2FtDataRecieve);
   
   LOGI("%s  line=%d dataSize = %d\n",__FUNCTION__,__LINE__,temBuf->size);
   return temBuf;
}

iAP2TransferData_st * getIap2FtReceiveDataBuffer() //malloc memery,must free after used ,when get ,must be free(g_iap2FtDataBufferList->setupData.fileSetupData) ,else if 
//it will cause a memery leak;
{
   //	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);

   pthread_mutex_lock(&lockIap2FtDataRecieve);
   
  // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
   if(g_iAP2FileTransferData_st == NULL)
   	{
   	 pthread_mutex_unlock(&lockIap2FtDataRecieve);
	 return NULL;
   	}
   iAP2TransferData_st * retBuf  = (iAP2TransferData_st *)malloc(sizeof(iAP2TransferData_st));
   
     iAP2TransferData_st * temBuf = g_iAP2FileTransferData_st;
	  memcpy(retBuf,temBuf,sizeof(iAP2TransferData_st));
	  retBuf->link = temBuf->link;
	  if(temBuf->data!=NULL)
	  {
	    retBuf->data = (uint8_t*)malloc(temBuf->size);
		memcpy(retBuf->data,temBuf->data,temBuf->size);
		free(temBuf->data);
	  }
	  
	 g_iAP2FileTransferData_st = g_iAP2FileTransferData_st->next;
	 free(temBuf);
   
   pthread_mutex_unlock(&lockIap2FtDataRecieve);
   
   return retBuf;
}

void freeAllIap2FtReceiveDataBuffer()
{
 iAP2TransferData_st *p =getIap2FtReceiveDataBufferToFree();
  while(p!=NULL)
  	{
  	
   if(p->data)
   	{
   
   	 free(p->data);
	 p->data= NULL;
   	}
      free(p);
  	  p = NULL;
  	  p =getIap2FtReceiveDataBufferToFree();
  	}
  exitFileTransferDataThread = true;
}


void addIap2FtDataBuffer(iAP2FileTransferSetupData_t * buf)
{
  pthread_mutex_lock(&lockIap2FtDataBufferList);
 // LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);
     iAP2FileTransferDataBuffer_st * temBuf = (iAP2FileTransferDataBuffer_st *)malloc(sizeof(iAP2FileTransferDataBuffer_st));
	 
	 temBuf->setupData = buf;
	 temBuf->next = NULL;
	// memcpy(temBuf,&buf,sizeof(iAP2FileTransferDataBuffer_st));
	// temBuf->buf = (u8 *)malloc(buf.bufLen);
	//temBuf->setupData = buf;
	// 
	 //memcpy(temBuf->buf,buf.buf,buf.bufLen);
	 if(g_iap2FtDataBufferList == NULL)
	 	{
	 	 g_iap2FtDataBufferList = temBuf;
	 	}
	 else
	 	{
	 	  iAP2FileTransferDataBuffer_st * temBuf2 = g_iap2FtDataBufferList;
		  while(temBuf2->next!=NULL)
		  	{
		  	 temBuf2 = temBuf2->next;
		  	}
		  temBuf2->next = temBuf;
	 	}
	  pthread_mutex_unlock(&lockIap2FtDataBufferList);
}

void cpFtDataBuffer(iAP2FileTransferDataBuffer_st * src,iAP2FileTransferDataBuffer_st * dest)
{
	
 
  if(dest!=NULL && dest->setupData!=NULL)
  	{
  	  if(dest->setupData->fileData!=NULL)
	  	free(dest->setupData->fileData);
	  if(dest->setupData->fileSetupData!=NULL)
	  	free(dest->setupData->fileSetupData);
	  free(dest->setupData);
	  free(dest);
	  dest = NULL;
	  
  	}
  if(src != NULL && src->setupData!=NULL)
  	{

    dest = (iAP2FileTransferDataBuffer_st *)malloc(sizeof(iAP2FileTransferDataBuffer_st));
	dest->setupData = (iAP2FileTransferSetupData_t *)malloc(sizeof(iAP2FileTransferSetupData_t));
	memcpy(dest->setupData,src->setupData,sizeof(iAP2FileTransferSetupData_t));
	 if(src->setupData->fileData!=NULL)
	 	{
		 	dest->setupData->fileData = (uint8_t *)malloc(src->setupData->fileSize);
			memcpy(dest->setupData->fileData,src->setupData->fileData,src->setupData->fileSize);
	  	//free(dest->fileData);
	 	}
	  if(dest->setupData->fileSetupData!=NULL)
	  	{
		  	dest->setupData->fileSetupData = (uint8_t *)malloc(src->setupData->fileSetupDataSize);
			memcpy(dest->setupData->fileSetupData,src->setupData->fileSetupData,src->setupData->fileSetupDataSize);
	  	}
  	}
	
    
}


iAP2FileTransferDataBuffer_st * getIap2FtDataBufferToFree() //malloc memery,must free after used ,when get ,must be free(g_iap2FtDataBufferList->setupData.fileSetupData) ,else if 
//it will cause a memery leak;
{
   pthread_mutex_lock(&lockIap2FtDataBufferList);
   if(g_iap2FtDataBufferList == NULL)
   	{
   	 pthread_mutex_unlock(&lockIap2FtDataBufferList);
	 return NULL;
   	}
     iAP2FileTransferDataBuffer_st * temBuf = g_iap2FtDataBufferList;
	 g_iap2FtDataBufferList = g_iap2FtDataBufferList->next;
   
   pthread_mutex_unlock(&lockIap2FtDataBufferList);
   return temBuf;
}

void freeAllIap2FtDataBuffer()
{
 iAP2FileTransferDataBuffer_st *p =getIap2FtDataBufferToFree();
  while(p!=NULL)
  	{
  	
   if(p->setupData->fileData)
   	{
   
   	 free(p->setupData->fileData);
	 p->setupData->fileData = NULL;
   	}
 
   if(p->setupData->fileSetupData)
   	{
   
   	 free(p->setupData->fileSetupData);
	 p->setupData->fileSetupData = NULL;
   	}
 
  if(p->setupData)
   free(p->setupData);
   p->setupData = NULL;
 
   free(p);
  	  p = NULL;
  	  p =getIap2FtDataBufferToFree();
  	}
}

void iAP2CheckLog(iAP2LogType_t type);

char *iAP2ClientGetVersion(void)
{
    return (char *)"170223.1";
}

void iAP2ClientTransportRecvCB(
                                                        void * pTransport,
                                                        uint8_t* buffer, 
                                                        int len
                                                        ) 
{   
    struct iAP2Transport_st* transport = (struct iAP2Transport_st*)pTransport;
   
    if (len == 0) {
    }

    #if 0
    if (len == 6 && buffer[0] == 0xFF && buffer[1] == 0x55 && buffer[2] == 0x02
        && buffer[3] == 0x00 && buffer[4] == 0xEE && buffer[5] == 0x10) {
        // woosok[03]
        iAP2LogDbg(
        "==========> [03] RecvDetect & FSM Handle: %s, %s(%d)\n\n",
        __FUNCTION__, __FILE__, __LINE__);
        iAP2FSMHandleEvent(transport->linkRunLoop->link->fsm,
        kiAP2LinkEventRecvDetect);
        return;
    }
    #endif

    // iPhone send Suspend Packet
    #if 1
    if (len == 9) {
        if (buffer[0] == 0xFF && buffer[1] == 0x5A && buffer[2] == 0x00
            && buffer[3] == 0x09 && buffer[4] == 0x08 && buffer[6] == 0x00 && buffer[7] == 0x00 ) {
            ALOGD("[sunghwan] iPhone is going to suspend mode");
            //setAppleDeviceSuspend();
        }
    }
    #endif

    // Initialization Check
    // iAP1 General Lingo RequestIdentify Packet 55 02 00 00 FE
    // 					  BadParam Packet 		 55 04 00 02 04 EE 08
    // In either case, send roleback request to RSD
    #if 1
    if ( len == 5 || len == 7) {
        if (buffer[0] == 0x55 && buffer[1] == 0x02 && buffer[2] == 0x00
            && buffer[3] == 0x00 && buffer[4] == 0xFE ) {
            iAP2LinkRunLoop_t*      linkRunLoop;
            ALOGD(
            "[sunghwan] iAP1 General Lingo RequestIdentify Packet detected");
            //setAppleDeviceSuspend();
            //iAP2ClientStop();
            transport->noti_cb(IAP2_NOTI_INIT_FAILED_IAP1,0,0,NULL);
        }
        if  (buffer[0] == 0x55 && buffer[1] == 0x04 && buffer[2] == 0x00
            && buffer[3] == 0x02 && buffer[4] == 0x04 && buffer[5] == 0xEE && buffer[6] == 0x08  ) {
            ALOGD("[sunghwan] iAP1 General Lingo BadParam Packet Packet detected");
            //setAppleDeviceSuspend();
            //iAP2ClientStop();
            transport->noti_cb(IAP2_NOTI_INIT_FAILED_IAP1,0,0,NULL);
        }
    }
    #endif
    while (len > 0) {
        if (transport->packet == NULL) {
            transport->packet = iAP2PacketCreateEmptyRecvPacket(
            transport->linkRunLoop->link);
        }

        BOOL bDetect;
        uint32_t failedChecksums;
        uint32_t sopDetect;

        uint32_t parsedLen = iAP2PacketParseBuffer(buffer, len,
        transport->packet, 65535, &bDetect, &failedChecksums,
        &sopDetect);
		
		//ALOGD(" %s %d parsedLen:%d",__FUNCTION__,__LINE__,parsedLen);
        len -= parsedLen;
        buffer += parsedLen;

        if (iAP2PacketIsComplete(transport->packet)) {
         //   ALOGD("[ACC] recv(signal) packetLen: %d, state: %d\n\n",
           // transport->packet->packetLen, transport->packet->state);

            iAP2LinkRunLoopHandleReadyPacket(transport->linkRunLoop,
            transport->packet);

            transport->packet = NULL;
        }
    }
}

void iAP2ClientNotiTransportError(void * pTransport,int type)
{
    struct iAP2Transport_st* transport = NULL;(struct iAP2Transport_st*)pTransport;
    if(pTransport == NULL)
        return;
    transport = (struct iAP2Transport_st*)pTransport;
    if(transport->noti_cb){
        int notiType = -1;
        switch(type){
            case TRANS_ERR_EIO:
                notiType = IAP2_NOTI_USB_EIO_ERR;
                break;
        }
        if(notiType >= 0)
            transport->noti_cb(notiType,0,0,NULL);
    }
}

iAP2Transport_t *iAP2ClientGetTransportFromLink(struct iAP2Link_st* link)
{
    iAP2LinkRunLoop_t *linkRunLoop = NULL;
    iAP2Client_t *client = NULL;
    if (!link)
    {
        iAP2LogError("%s, %s(%d) invalid iAP2Link_st", 
                                                                    __FUNCTION__, 
                                                                    __FILE__, 
                                                                    __LINE__);
        return NULL;
    }
    linkRunLoop = (iAP2LinkRunLoop_t*)link->context;
    if (!linkRunLoop)
    {
        iAP2LogError("%s, %s(%d) invalid iAP2LinkRunLoop_t", 
                                                                    __FUNCTION__, 
                                                                    __FILE__, 
                                                                    __LINE__);
        return NULL;
    }

    client = (iAP2Client_t *)linkRunLoop->context;
    if (!client)
    {
        iAP2LogError("%s, %s(%d) invalid iAP2Client_t",
                                                                    __FUNCTION__, 
                                                                    __FILE__, 
                                                                    __LINE__);
        return NULL;
    }
    return client->trans;
}

 

BOOL iAP2FileTransferGotDataCB (struct iAP2FileTransfer_st* fileXfer, void* userInfo)
{
	//LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);

	if(userInfo == NULL)
		return false;
	//LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);
	iAP2FileTransferSetupData_t * pftsdata= (iAP2FileTransferSetupData_t *)userInfo;
	/*if(pftsdata->fileType == 3) {// NowPlayingPlaybackQueueContents
	}
	else if(pftsdata->fileType == 4) { //MeidaLibraryUpdatePlaylistContents
	}
	else */if(pftsdata->fileType == 2 || pftsdata->fileType == 6 ||
	pftsdata->fileType == 7 || pftsdata->fileType == 3 || pftsdata->fileType == 4)
	{
		//LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);
       if(kiAP2FileTransferStatePauseRecv == pftsdata->state)
       {
			iAP2FileTransferSetupData_t  *ftTfSetupData = (iAP2FileTransferSetupData_t *)malloc(sizeof(iAP2FileTransferSetupData_t));
			memset(ftTfSetupData,0,sizeof(iAP2FileTransferSetupData_t));
			memcpy(ftTfSetupData,pftsdata,sizeof(iAP2FileTransferSetupData_t));
			ftTfSetupData->fileSetupData = (uint8_t*)malloc(pftsdata->fileSetupDataSize);
			
			memcpy(ftTfSetupData->fileSetupData,pftsdata->fileSetupData,pftsdata->fileSetupDataSize);
			//iAP2FileTransferDataBuffer_st buf;
			//memcpy(&buf.setupData,&ftTfSetupData,sizeof(iAP2FileTransferSetupData_t));
			//buf.bufLen = fileXfer->buffSize;
			//buf.buf = fileXfer->pBuffer;
			//buf.buf = (u8 *)malloc(buf.bufLen);
			//memcpy(&buf.buf,fileXfer.pBuffer,buf.bufLen);
		//	LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);
			//addIap2FtDataBuffer( ftTfSetupData);
			iAP2DispatchFtData(ftTfSetupData);
			//free(ftTfSetupData.fileSetupData);
				if(pftsdata->fileSetupData!=NULL)
			free(pftsdata->fileSetupData);
			if(pftsdata->fileData!=NULL)
			free(pftsdata->fileData);
			free(pftsdata);
       	}
	    else
	    {
	       	iAP2FileTransferSetupData_t  *ftTfData = (iAP2FileTransferSetupData_t *)malloc(sizeof(iAP2FileTransferSetupData_t));
			
			memset(ftTfData,0,sizeof(iAP2FileTransferSetupData_t));
			memcpy(ftTfData,pftsdata,sizeof(iAP2FileTransferSetupData_t));
			ftTfData->fileData= (uint8_t*)malloc(pftsdata->fileSize);
			memcpy(ftTfData->fileData,pftsdata->fileData,pftsdata->fileSize);
			if(pftsdata->fileSetupData!=NULL && pftsdata->fileSetupDataSize>0)
			{ 
						ftTfData->fileSetupData = (uint8_t*)malloc(pftsdata->fileSetupDataSize);
			
			      memcpy(ftTfData->fileSetupData,pftsdata->fileSetupData,pftsdata->fileSetupDataSize);
			}
			//iAP2FileTransferDataBuffer_st buf;
			//memcpy(&buf.setupData,&ftTfSetupData,sizeof(iAP2FileTransferSetupData_t));
			//buf.bufLen = fileXfer->buffSize;
			//buf.buf = fileXfer->pBuffer;
			//buf.buf = (u8 *)malloc(buf.bufLen);
			//memcpy(&buf.buf,fileXfer.pBuffer,buf.bufLen);
			//LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);
			iAP2DispatchFtData(ftTfData);
			//addIap2FtDataBuffer( ftTfData);
			//free(ftTfData.fileData)
			//	LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);
			if(pftsdata->fileSetupData!=NULL)
				free(pftsdata->fileSetupData);
			if(pftsdata->fileData!=NULL)
			free(pftsdata->fileData);
			//	LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);
			free(pftsdata);
			//	LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);
	    }
		
	}

  return true;
}

/* iAP2LinkSendPacketCB_t     sendPacketCB */
static void LinkSendPacketCB(struct iAP2Link_st* link, iAP2Packet_t* packet)
	{
		iAP2Transport_t *transport = NULL;
		uint8_t *data = NULL;
		transport = iAP2ClientGetTransportFromLink(link);
		if (!transport)
		{
			ALOGD("%s, %s(%d) invalid transport",
																		__FUNCTION__, 
																		__FILE__, 
																		__LINE__);
			return;
		}
		
		data = iAP2PacketGenerateBuffer(packet);
	
#if iAP2LINK_DEBUG_PACKET
		//iAP2PacketDebugPrintPacket (packet, __FILE__, __LINE__, "Send>", "");
#endif

//IPOD_HID_SendPacket(g_handle,data,packet->packetLen);
		iAP2TransportSend(transport, data, packet->packetLen);
	}

/* iAP2LinkDataReadyCB_t      recvDataCB */
static BOOL LinkDataReadyCB(
                                                    struct iAP2Link_st* link, 
                                                    uint8_t* data, 
                                                    uint32_t dataLen, 
                                                    uint8_t session
                                                    )
{
	

  //data = iAP2PacketGenerateBuffer(packet);
   //add jni call backe data to process
	   
   if(0x40==data[1] && 0x40 == data[0] && session == 1)//session == 1 control
   	{
   	addIap2Session1DataBuffer(link,data, dataLen);
    //iAP2SessCtrlDispatchMsg(link, data, dataLen, session);
   	}
   else if(session == 2)//session == 2 file
   	{
     LOGI("LinkDataReadyCB file transfer dataLen=%d\n",dataLen);
	  addIap2FtReceiveDataBuffer(link,data, dataLen);
   	 // iAP2FileTransferHandleRecv(g_iap2handle->fileTrans,data, dataLen);
   	}
    return false;
    //return ret;
}

/* iAP2LinkConnectedCB_t      connectedCB */
static void LinkConnectedCB(struct iAP2Link_st* link, BOOL bConnected)
{
    IAP2_CTRL_UNUSED(link);
    IAP2_CTRL_UNUSED(bConnected);
    iAP2LogDbg(" %s %d bConnected %d",__FUNCTION__,__LINE__,bConnected);	
}

/* iAP2LinkSendDetectCB_t     sendDetectCB */
static void LinkSendDetectCB(struct iAP2Link_st* link, BOOL bBad)
	{
		iAP2Transport_t *transport = NULL;
		IAP2_CTRL_UNUSED(bBad);
		transport = iAP2ClientGetTransportFromLink(link);
		if (!transport)
		{
			iAP2LogError("%s, %s(%d) invalid transport", 
																		__FUNCTION__, 
																		__FILE__, 
																		__LINE__);
			return;
		}
		
		iAP2TransportSend(
										transport, 
										(uint8_t*)kIap2PacketDetectData, 
										kIap2PacketDetectDataLen
										);

		//IPOD_HID_SendPacket(g_handle,kIap2PacketDetectData,kIap2PacketDetectDataLen);
	}


iAP2InterfaceCB_t *GetInterfaceWithLinkRunLoop(void *plinkRunLoop)
{
    iAP2Client_t *client = NULL;
    iAP2LinkRunLoop_t *linkRunLoop = NULL;
    iAP2InterfaceCB_t *interfCb = NULL;

    if(plinkRunLoop == NULL){
        ALOGE("  %s(%d)  plinkRunLoop is NULL", __FUNCTION__,  __LINE__);
        return NULL;
    }
    linkRunLoop = (iAP2LinkRunLoop_t *)plinkRunLoop;

    if(linkRunLoop->context == NULL){
        ALOGE("  %s(%d)  linkRunLoop->context is NULL", 
                                                                            __FUNCTION__,  
                                                                            __LINE__
                                                                            );
        return NULL;
    }

    client = (iAP2Client_t *)linkRunLoop->context;

    if(client != NULL){
        interfCb= &client->interfaceCb;
    }else{
        ALOGE("  %s(%d)  client is NULL", __FUNCTION__,  __LINE__);
    }

    return interfCb;
}

static void LinkTimeoutCB(void * plinkRunLoop, iAP2LinkRunLoopEventMask_t bit)
{
    iAP2Notify_callback iAP2Notify_cb = NULL;
    iAP2InterfaceCB_t *interfCb = GetInterfaceWithLinkRunLoop(plinkRunLoop);

    if(interfCb == NULL){
        ALOGE("  %s(%d)  interfCb is NULL", __FUNCTION__,  __LINE__);
        return;
    }

    iAP2Notify_cb = interfCb->notiInterf.iAP2Notify_cb;

    if(iAP2Notify_cb){
        int event = 0;
        switch((int)bit){
            case kiAP2LinkRunLoopEventMaskWaitACKTimeout:
                //event = IAP2_NOTI_TIMEOUT_WAITACK;
                ALOGI(" %s %d waitAck timeout notify 0x%x event %d",__FUNCTION__,__LINE__,bit,IAP2_NOTI_TIMEOUT_WAITACK);
                break;
            case kiAP2LinkRunLoopEventMaskSendACKTimeout:
                //ALOGI(" %s %d SendAck timeout notify 0x%x event %d",__FUNCTION__,__LINE__,bit,IAP2_NOTI_TIMEOUT_SENDACK);
                //event = IAP2_NOTI_TIMEOUT_SENDACK;
                break;
            case kiAP2LinkRunLoopEventMaskWaitDetectTimeout:
                ALOGI(" %s %d waitDetect timeout notify 0x%x event %d",__FUNCTION__,__LINE__,bit,IAP2_NOTI_TIMEOUT_WAITDETECT);
                event = IAP2_NOTI_TIMEOUT_WAITDETECT;
                break;
        }
        if(event > 0){
            ALOGI(" %s %d timeout notify 0x%x event %d",__FUNCTION__,__LINE__,bit,event);
            iAP2Notify_cb(event,0,0,NULL);
        }
    }
}

//#ifdef USE_TCC_IAP2

static void LinkNotifyCB(void * plinkRunLoop, iAP2LinkRunLoopNotifyMask_t bit)
{
    iAP2Notify_callback iAP2Notify_cb= NULL;
    iAP2InterfaceCB_t *interfCb = GetInterfaceWithLinkRunLoop(plinkRunLoop);

    if(interfCb == NULL){
        ALOGE("  %s(%d)  interfCb is NULL", __FUNCTION__,  __LINE__);
        return;
    }

    iAP2Notify_cb = interfCb->notiInterf.iAP2Notify_cb;

    if(iAP2Notify_cb){
        int event = 0;
        switch((int)bit){
            case kiAP2LinkRunLoopNotifyMaskMaxResendData:
                event = IAP2_NOTI_LIMIT_MAX_RESEND;
                ALOGI(" %s %d LimitMaxResendData event %d",__FUNCTION__,__LINE__,event);
                break;
            case kiAP2LinkRunLoopNotifyMaskMaxResendMissing:
                event = IAP2_NOTI_LIMIT_MAX_RESEND;
                ALOGI(" %s %d LimitResendMissing event %d",__FUNCTION__,__LINE__,event);
                break;
            case kiAP2LinkRunLoopNotifyMaskMaxResendInOrderPckSYN:
                event = IAP2_NOTI_LIMIT_MAX_RESEND;
                ALOGI(" %s %d LimitResendInOrderPckSYN event %d",__FUNCTION__,__LINE__,event);
                break;
        }
        if(event > 0){
            iAP2Notify_cb(event,0,0,NULL);
        }
    }
}
//#endif

void iAP2Client_DebugTrigger(int handle)
{
#if USE_TCC_IAP2
    iAP2Client_t* piAP2Client = (iAP2Client_t*)handle;
    if(piAP2Client == NULL)
        return;
    iAP2CheckLog(kiAP2LogTypeError);
    iAP2CheckLog(kiAP2LogTypeLog);
    iAP2CheckLog(kiAP2LogTypeLogDbg);
    iAP2CheckLog(kiAP2LogTypeData);
    iAP2SessCtrl_DebugTrigger(piAP2Client->controlSession,piAP2Client->eaSession,piAP2Client->fileSession);
 #endif
}




void iAP2CheckLog(iAP2LogType_t type)
{
    char iapDebug[PROPERTY_VALUE_MAX];

    switch(type)
    {
        case kiAP2LogTypeError:
            property_get("persist.iap2.debug.error", iapDebug, "f");
            break;
        case kiAP2LogTypeLog:
            property_get("persist.iap2.debug.log", iapDebug, "f");
            break;
        case kiAP2LogTypeLogDbg:
            property_get("persist.iap2.debug.dbg", iapDebug, "f");
            break;
        case kiAP2LogTypeData:
            property_get("persist.iap2.debug.data", iapDebug, "f");
            break;
        default:
            return;
    }
    
    switch(iapDebug[0]){
        case 't':
        case 'T':
            iAP2LogEnable(type);
            break;
        default:
            iAP2LogDisable(type);
            break;
    }
}

BOOL iAP2ClientIsDiconnected(iAP2Client_t* iap2handle)
{
    BOOL isDisconnected = TRUE;
    if(iap2handle != NULL){
        if(iap2handle->interfaceCb.javaInterf.iAP2ChkConn_cb){
            BOOL isConnected = FALSE;
            isConnected = iap2handle->interfaceCb.javaInterf.iAP2ChkConn_cb();
            if(isConnected)
                isDisconnected = FALSE;
        }
    }
    if(isDisconnected)
        ALOGD(" %s detect disconnected", __FUNCTION__);
    return isDisconnected;
}

size_t iAP2ClientReqBuffSize(void)
{
    size_t buffSize = 0;
    buffSize += sizeof(iAP2LinkRunLoop_t);
    buffSize += iAP2LinkGetBuffSize (MAX_PACKET_SENT_AT_ONCE);
    buffSize += 1024; // dummy
    return buffSize;
}

int iAP2ClientExternalReadDone(iAP2Client_t* handle,void *buffer,int size)
{
    iAP2Client_t* piAP2Client = (iAP2Client_t*)handle;

    if(piAP2Client == NULL)
        return -1;
	#ifdef USE_TCC_IAP2	
    iAP2LogDump((const char* )buffer,size);
	#endif
    iAP2ClientTransportRecvCB(piAP2Client->trans,(uint8_t* )buffer,size);
    return size;
}

void iAP2ClientDebugInit(void)
{
#ifdef USE_TCC_IAP2
    char iapDebug[PROPERTY_VALUE_MAX];
    property_get("persist.iap2.debug.mem", iapDebug, "f");
    switch(iapDebug[0]){
        case 't':
        case 'T':
            iAP2SessCtrlSetMemDebug(TRUE);
            break;
        default:
            iAP2SessCtrlSetMemDebug(FALSE);
            break;
    }

    property_get("persist.iap2.debug.pck", iapDebug, "f");
    switch(iapDebug[0]){
        case 't':
        case 'T':
            iAP2SessCtrlSetPckDebug(TRUE);
            break;
        default:
            iAP2SessCtrlSetPckDebug(FALSE);
            break;
    }

    setCallback(&gIap2CallbackFunctions);
    iAP2SessCtrlInitMallocInfo();
    iAP2SessCtrlLinkPacketDebugInit();
	#endif
}

void* iAP2HandleSession1DataThread(void* param) 
{
	while(!exitSession1DataThread)
	   {
	   
	   	iAP2TransferData_st * pFTDataBuffer = (iAP2TransferData_st *)getIap2Session1DataBuffer();
		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
  	   if(pFTDataBuffer == NULL)
  	   {
  	  	 usleep(50*1000);
		 // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		 continue;
  	   }
	   iAP2SessCtrlDispatchMsg(pFTDataBuffer->link, pFTDataBuffer->data, pFTDataBuffer->size, 1);
	   // iAP2FileTransferHandleRecv(g_iap2handle->fileTrans,pFTDataBuffer->data, pFTDataBuffer->size);
		free(pFTDataBuffer->data);
		free(pFTDataBuffer);
	  }

	return NULL;

}


void* iAP2HandleFileTransferDataThread(void* param) 
{
	while(!exitFileTransferDataThread)
	   {
	    
	   	iAP2TransferData_st * pFTDataBuffer = (iAP2TransferData_st *)getIap2FtReceiveDataBuffer();
		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
  	   if(pFTDataBuffer == NULL)
  	   {
  	  	 usleep(50*1000);
		 // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		 continue;
  	   }
	   LOGI("iAP2HandleFileTransferDataThread pFTDataBuffer->size=%d\n",pFTDataBuffer->size);	
	    iAP2FileTransferHandleRecv(g_iap2handle->fileTrans,pFTDataBuffer->data, pFTDataBuffer->size);
		free(pFTDataBuffer->data);
		free(pFTDataBuffer);
	  }
	return NULL;

}
//FILE * fp_file = NULL;

int fp_file2 = -1;
int fp_file3 = -1;
int fp_file4 = -1;
int fp_file6 = -1;
int fp_file7 = -1;

void iAP2DispatchFtData(iAP2FileTransferSetupData_t * pFTDataBuffer )
{
	char	 saveFileName[128] = {0};
	char     idfileName[20]={0};
//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	//LOGI("%s  line=%d\n",__FUNCTION__,__LINE__);

    if(access("/storage/emulated/0/Download/ipod",0)==-1)
    {
       mkdir("/storage/emulated/0/Download/ipod",0777);
    }
 // while(1)
  	//{
  	
  	memset(saveFileName,0,128);
	memset(idfileName,0,20);
		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
  //	iAP2FileTransferDataBuffer_st * pFTDataBuffer = getIap2FtDataBuffer();
		  if(pFTDataBuffer!=NULL && pFTDataBuffer->fileData!=NULL && pFTDataBuffer->fileSize > 0)
		  	{
		  	
			LOGI("iAP2DispatchFtData 000 fwrite  pFTDataBuffer->setupData->fileSize=%d\n",pFTDataBuffer->fileSize);
		  	}
  	   if(pFTDataBuffer == NULL)
  	   {
  	  	// usleep(50*1000);
		 return;
  	   }
	   int fd;
	  if(pFTDataBuffer->fileType == 2)//jpeg
	  	{
            sprintf(saveFileName,"/storage/emulated/0/Download/ipod/APArtwork.jpeg");
		   // sprintf();
	  	}
	  else if(pFTDataBuffer->fileType == 3)//MediaItemNowPlaying PlaybackQueueContents
	  	{
			 sprintf(saveFileName,"/storage/emulated/0/Download/ipod/%d.npq",pFTDataBuffer->fileTransferID);
			 sprintf(idfileName,"%d.npq",pFTDataBuffer->fileTransferID);
			
	  	}
	   else if(pFTDataBuffer->fileType == 4)//MediaItemNowPlaying PlaybackQueueContents
	  	{
			 sprintf(saveFileName,"/storage/emulated/0/Download/ipod/%d.mup",pFTDataBuffer->fileTransferID);
		
		sprintf(idfileName,"%d.mup",pFTDataBuffer->fileTransferID);
	  	}
	  else if(pFTDataBuffer->fileType == 6)//MediaItemNowPlaying PlaybackQueueContents
	  	{
			 sprintf(saveFileName,"/storage/emulated/0/Download/ipod/%d.mn",pFTDataBuffer->fileTransferID);
			
			sprintf(idfileName,"%d.mn",pFTDataBuffer->fileTransferID);
	  	}
	  else if(pFTDataBuffer->fileType == 7)//MediaItemMediaLibraryUpdate PlayListContents
	  	{
			 sprintf(saveFileName,"/storage/emulated/0/Download/ipod/%d.mm",pFTDataBuffer->fileTransferID);	
			
			sprintf(idfileName,"%d.mm",pFTDataBuffer->fileTransferID);
	  	}
	   if(pFTDataBuffer->state == kiAP2FileTransferStatePauseRecv /*&& pFTDataBuffer->setupData.saveFileName[0]=='0'*/)//first data for setup, createfile
	   	{

			if((access(saveFileName,F_OK))!=-1)   
				{
			     remove(saveFileName);
				}
			
			   if(pFTDataBuffer->fileType == 2)
			 fp_file2 = open(saveFileName, O_RDWR|O_CREAT,0777);
			   else if(pFTDataBuffer->fileType == 3)
			 fp_file3 = open(saveFileName, O_RDWR|O_CREAT,0777);
			     else if(pFTDataBuffer->fileType == 4)
			 fp_file4 = open(saveFileName, O_RDWR|O_CREAT,0777);
				   else if(pFTDataBuffer->fileType == 6)
			 fp_file6 = open(saveFileName, O_RDWR|O_CREAT,0777);
				     else if(pFTDataBuffer->fileType == 7)
			 fp_file7 = open(saveFileName, O_RDWR|O_CREAT,0777);
					  
			   	
				  //fp_file = fopen(saveFileName,"a+");      //读/写打开，创建/清空

			
			if(pFTDataBuffer->fileType == 2)//MeidaNowPlayingArtworkm.jpeg
				{
				LOGI("%s %d\n",__FUNCTION__,__LINE__);
				mIpodService->notifyOnMediaNowPlayingArtworkStart();
				}
			else if(pFTDataBuffer->fileType == 3)//NowPlayingPlaybackQueueContents
				{
				
				mIpodService->NowPlayingPBQueueContentsStart((uint8_t *)idfileName,strlen(idfileName)+1);
				
				}
				else if(pFTDataBuffer->fileType == 4)//MediaLibraryUpdatePlaylistContents
				{
				// LOGI("%s line:%d start idfileName=%s pFTDataBuffer->setupData->fileSetupDataSize=%d\n",__FUNCTION__,__LINE__,idfileName,pFTDataBuffer->setupData->fileSetupDataSize);
				// LOGI("%s line:%d start  pFTDataBuffer->setupData->fileSetupDataSize=%d\n",__FUNCTION__,__LINE__,pFTDataBuffer->setupData->fileSetupDataSize);
				 //if(pFTDataBuffer->setupData->fileSetupData==NULL)
				 //	LOGI("pFTDataBuffer->setupData->fileSetupData==null\n");
				 //else
				  //  LOGI("pFTDataBuffer->setupData->fileSetupData!=null \n");
				// LOGI("strlen(idfileName)=%d\n",strlen(idfileName));
				 
				  	mIpodService->MediaLibraryUpdatePlaylistContentsStart(strlen(idfileName),(unsigned char *)idfileName,8,(unsigned char *)pFTDataBuffer->fileSetupData,
				  													pFTDataBuffer->fileSetupDataSize-8,
				  													(unsigned char  *)(pFTDataBuffer->fileSetupData+8));					
				}
				else if(pFTDataBuffer->fileType == 6)//MediaItemListNowPlayingPlaybackQueueContents
				{
				  mIpodService->MediaItemNowPlayingPBQueueContentsStart((uint8_t *)idfileName,strlen(idfileName+1));
				}
				else if(pFTDataBuffer->fileType == 7)//MediaItemListMediaLibraryUpdatePlaylistContents
				{
				   	mIpodService->MediaItemListUpdatePlaylistContentsStart(strlen(idfileName),(unsigned char *)idfileName,8,pFTDataBuffer->fileSetupData,
				  													pFTDataBuffer->fileSetupDataSize-8,
				  													pFTDataBuffer->fileSetupData+8);	
				}

	   }
	  // else 
	   	//{
	   	//LOGI("%s line:%d saveFileName=%s\n",__FUNCTION__,__LINE__,saveFileName);
	
		 // if(fp_file!=NULL)
//{
		  if(pFTDataBuffer->fileData!=NULL && pFTDataBuffer->fileSize > 0)
		  	{
		  	int ret = -1;
		   if(pFTDataBuffer->fileType == 2)	
		   	{
		      ret = write(fp_file2, pFTDataBuffer->fileData,pFTDataBuffer->fileSize);
			   LOGI("strerror(errno) =%s fp_file2=%d\n",strerror(errno),fp_file2);
		   	}
			else if(pFTDataBuffer->fileType == 3){
				ret = write(fp_file3, pFTDataBuffer->fileData,pFTDataBuffer->fileSize);
				
				   LOGI("strerror(errno) =%s fp_file3=%d\n",strerror(errno),fp_file3);
				}
			else if(pFTDataBuffer->fileType == 4){
				ret = write(fp_file4, pFTDataBuffer->fileData,pFTDataBuffer->fileSize);
				
				   LOGI("strerror(errno) =%s fp_file4=%d\n",strerror(errno),fp_file4);
				}
			else if(pFTDataBuffer->fileType == 6){
				ret = write(fp_file6, pFTDataBuffer->fileData,pFTDataBuffer->fileSize);\
					
				   LOGI("strerror(errno) =%s fp_file6=%d\n",strerror(errno),fp_file6);
				}
			else if(pFTDataBuffer->fileType == 7){
				ret = write(fp_file7, pFTDataBuffer->fileData,pFTDataBuffer->fileSize);
				
				   LOGI("strerror(errno) =%s fp_file7=%d\n",strerror(errno),fp_file7);
				}
		  	}
	   if(pFTDataBuffer->state == kiAP2FileTransferStateFinishRecv)
	   	{
	   	
	   	   //todo : add code here to start parse;
	   	   if(pFTDataBuffer->fileType==2)//NowPlayingArtwork.jpg	   
	   	   	{
		   		mIpodService->notifyOnMediaNowPlayingArtworkFinish();
				if(fp_file2!=-1)
				close(fp_file2);
				fp_file2 = -1;
	   	   	}
		   else if(pFTDataBuffer->fileType == 3)//NowPlayingPlaybackQueueContents
		   	{
		  
				mIpodService->NowPlayingPBQueueContentsFinish((uint8_t *)idfileName,strlen(idfileName)+1);
				
				if(fp_file3!=-1)
				close(fp_file3);
				fp_file3 = -1;
		   	}
		   
		   else if(pFTDataBuffer->fileType == 4)//MediaLibraryUpdatePlaylistContents
				{
				
				  	mIpodService->MediaLibraryUpdatePlaylistContentsFinish(strlen(idfileName),(unsigned char *)idfileName,8,pFTDataBuffer->fileSetupData,
				  													pFTDataBuffer->fileSetupDataSize-8,
				  											pFTDataBuffer->fileSetupData+8);
				if(fp_file4!=-1)
				close(fp_file4);
				fp_file4 = -1;
					
				}
		   else if(pFTDataBuffer->fileType == 6)//MediaItemListNowPlayingPlaybackQueueContents
				{
				
				 mIpodService->MediaItemNowPlayingPBQueueContentsFinish((uint8_t *)idfileName+1,strlen(idfileName));
				if(fp_file6!=-1)
				close(fp_file6);
				fp_file6 = -1;
				}
		    else if(pFTDataBuffer->fileType == 7)//MediaItemListMediaLibraryUpdatePlaylistContents
				{ 
				
				   	mIpodService->MediaItemListUpdatePlaylistContentsFinish(strlen(idfileName),(unsigned char *)idfileName,8,pFTDataBuffer->fileSetupData,
				  													pFTDataBuffer->fileSetupDataSize-8,
				  													pFTDataBuffer->fileSetupData+8);	\
				  													LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
				if(fp_file7!=-1)
				close(fp_file7);
				fp_file7 = -1;
				}
			
	   	     
	   	}
	   
}




iAP2Client_t * iAP2ClientStart(/*iAP2Notify_callback noti_cb,
                                    void *pData,
                                    void *pCpData,
                                    iAP2ChkConn_callback chkConn_cb,
                                    int transportType,
                                    iAP2ExtWriteAttached_callback attached_cb,
                                    iAP2ExtDoWrite_callback doWrite_cb,
                                    iAP2ExtWriteDetached_callback detached_cb
                                    */)
{
    iAP2Client_t* iap2handle = NULL;
    iAP2LinkRunLoop_t*  runloop = NULL;
    uint8_t sessionId = 1;//1,control session,2,file sessoin
    uint8_t fileSessionId = 2;
    BOOL isSuccess = FALSE;
    
    uint8_t libVersion[20];
    int libNameLen = 0;
	uint8_t controlVer = 1; //iap2 版本为1
	int index = 0;
	
	

  //  iAP2ClientDebugInit();
    g_iap2handle = (iAP2Client_t*)malloc(sizeof(iAP2Client_t));
   // iAP2FileTransferSetupData_st * fsSetupData = (iAP2FileTransferSetupData_st*)malloc(sizeof(iAP2FileTransferSetupData_st));
    iap2handle = g_iap2handle;
    #if 0
    char iapDebug[PROPERTY_VALUE_MAX];

    property_get("persist.iap2.debug.start", iapDebug, "0");
    #endif

    assert(iap2handle);
    if(iap2handle == NULL){
        goto EXIT;
    }
    iap2handle->transportType = 0;

    iap2handle->buffer = NULL;
    iap2handle->runloop = NULL;
    iap2handle->controlSession = NULL;
    iap2handle->fileSession = NULL;
    iap2handle->eaSession = NULL;
    iap2handle->trans = NULL;
    iap2handle->sessionThread = NULL;    
	iap2handle->fileTrans = NULL;


   // iAP2CheckLog(kiAP2LogTypeError);
    //iAP2CheckLog(kiAP2LogTypeLog);
    //iAP2CheckLog(kiAP2LogTypeLogDbg);
    //iAP2CheckLog(kiAP2LogTypeData);
    initLockIap2FtDataBufferList();

    iap2handle->bufferSize = iAP2ClientReqBuffSize();
    iap2handle->buffer = malloc(iap2handle->bufferSize);
    assert(iap2handle->buffer);
    if(iap2handle->buffer == NULL){
        goto EXIT;
    }
    memset(iap2handle->buffer,0,iap2handle->bufferSize);
    iap2handle->bufferNext = iap2handle->buffer;
    iAP2LogDbg(" %s %d ",__FUNCTION__,__LINE__);	

    iAP2PacketSYNData_t synData;
    iAP2LinkSetDefaultSYNParam (&synData);

        synData.sessionInfo[index].id = sessionId++;
		
        synData.sessionInfo[index].type = kIAP2PacketServiceTypeControl;
		
        synData.sessionInfo[index].version= controlVer;
        index++;
	
        synData.sessionInfo[index].id = fileSessionId;
        synData.sessionInfo[index].type = kIAP2PacketServiceTypeBuffer;
        synData.sessionInfo[index].version= iap2_session_file_ver;
        index++;
		/*
        synData.sessionInfo[index].id = sessionId++;
        synData.sessionInfo[index].type = kIAP2PacketServiceTypeEA;
        synData.sessionInfo[index].version= iap2_session_ea_ver;
        index++;   
		*/
	
        synData.numSessionInfo = index;

      runloop = iAP2LinkRunLoopCreateAccessory(&synData,
                                iap2handle,
                                LinkSendPacketCB,
                                LinkDataReadyCB ,
                                LinkConnectedCB,
                                LinkSendDetectCB, FALSE, MAX_PACKET_SENT_AT_ONCE, (uint8_t*)iap2handle->buffer);
	

	assert(runloop);
	
    if(runloop == NULL){
     
        goto EXIT;
    }
	
    runloop->timeoutCb_t = LinkTimeoutCB; // 140930
    runloop->notifyCb_t = LinkNotifyCB; // 141124
  
    iap2handle->runloop = runloop;

    iap2handle->bufferNext = runloop->linkRLBufferNext;
    iap2handle->bufferRemain 
        = iap2handle->bufferSize - ((uint8_t *)iap2handle->bufferNext - (uint8_t *)iap2handle->buffer);
    
	iap2handle->fileTrans = iAP2FileTransferCreate(iap2handle->runloop->link,
													fileSessionId,
													0,
													iAP2FileTransferGotDataCB,
													NULL,
													FALSE,
													iap2handle->fileTrans);
	

 
    iap2handle->trans = iAP2TransportCreate(iap2handle->runloop, 
                                                            FALSE,
                                                            iAP2LinkRunLoopAttached,
                                                            iAP2ClientTransportRecvCB,
                                                            NULL,
                                                            iAP2ClientNotiTransportError,
                                                            0,
                                                            NULL,
                                                            NULL,
                                                            NULL);
      pthread_t piAP2FtReceiveDataThread;                                                   
	 pthread_t piAP2Session1DataThread;
	 exitSession1DataThread = false;
	 exitFileTransferDataThread = false;
	 
	 pthread_create(&piAP2Session1DataThread, NULL, iAP2HandleSession1DataThread /*iAP2TransportRecvThread*/,
        		NULL);
	
	 
	 pthread_create(&piAP2FtReceiveDataThread, NULL, iAP2HandleFileTransferDataThread /*iAP2TransportRecvThread*/,
        		NULL);
	
	 // pthread_t piAP2DispatchFtDataThread;
	  

	 // pthread_create(&piAP2DispatchFtDataThread, NULL, iAP2DispatchFtDataThread /*iAP2TransportRecvThread*/,
        	//	NULL);
	  
    assert(iap2handle->trans);
    if(iap2handle->trans == NULL /*|| iAP2ClientIsDiconnected(iap2handle)*/){// || iapDebug[0] == '7'){
        ALOGD(" %s %s",__FUNCTION__, (iap2handle->trans == NULL)? "trans == NULL":"trans disconnected");
        goto EXIT;
    }

	

    isSuccess = TRUE;
	
EXIT:
    if(isSuccess == FALSE){
        if(iap2handle != NULL)
            iAP2ClientStop((iAP2Client_t *)iap2handle);
        iap2handle = NULL;
      
    }else{
     
    }
	
    return iap2handle;
}

int iAP2ClientStop(iAP2Client_t * handle)
{
    int ret = 0;
   iAP2Client_t* piAP2Client = NULL;
    if(handle != 0){
        piAP2Client = (iAP2Client_t*)handle;
      #if 0
	     iAP2TransportClose(piAP2Client->trans);
        if(piAP2Client->trans != NULL)
            ALOGI(" %s[%d]  1.5  read %d write %d\n",
                                                                    __FUNCTION__,
                                                                    __LINE__,
                                                                    piAP2Client->trans->bReading,
                                                                    piAP2Client->trans->bWriting);
        else
            ALOGI(" %s[%d]  1.5 transport is null\n",__FUNCTION__,__LINE__);
		#else
		
		 if(piAP2Client->trans != NULL)
        iAP2TransportDestroy(piAP2Client->trans);
        iAP2LinkRunLoopDelete(piAP2Client->runloop);
        free(piAP2Client->buffer);
        free(piAP2Client);
		iAP2FileTransferCleanup(piAP2Client->fileTrans);
        piAP2Client = NULL;
		g_iap2handle = NULL;
		#endif
       
		
		
		 ALOGI(" %s[%d]  6  \n",__FUNCTION__,__LINE__);
    }
    //iAP2SessCtrlMallocDebug();
    //iAP2SessCtrlLinkPacketDebugPrint();
	
    return ret;
}


#define USB_PATH_VID "/sys/class/android_usb/android0/idVendor"
#define USB_PATH_PID "/sys/class/android_usb/android0/idProduct"
#define MAX_BUFF_SIZE   128

int iAP2ClientGetUsbProperties(Parcel *pParcel,const char *path)
{
    int sz;
    int fd;
    char data[MAX_BUFF_SIZE];
    int ret = 0;

    data[0] = 0;
	chmod(path,0666);
    fd = open(path, O_RDONLY);
    if(fd < 0)
    {
        ALOGE("cannot open %s", path);
        return -1;
    }else{
        sz = read(fd, data, MAX_BUFF_SIZE);
        close(fd);

        if(sz > 0 && sz < MAX_BUFF_SIZE)
        {
            data[sz] = 0;
            ALOGI(" path %s data: %s",USB_PATH_VID, data);
        }
        else
        {
            ALOGE("cannot load %s", path);
            ret = -ENODEV;
        }
    }

    pParcel->writeString16(String16(String8(&data[0])));
    
    return ret;
}

bool iAP2ClientGetUsbInfo(void *pData){
    Parcel *pParcel = (Parcel *)pData;
    if(iAP2ClientGetUsbProperties(pParcel, USB_PATH_VID) < 0)
        return FALSE;
    if(iAP2ClientGetUsbProperties(pParcel, USB_PATH_PID) < 0)
        return FALSE;
    return TRUE;
}

}

