/****************************************************************************
 *   FileName    : iAP2Coprocessor.c
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "iAP2Control.h"
#include "iAP2Interface.h"
#include "iAP2Coprocessor.h"
#include <iAP2Log.h>
#include <utils/Log.h>

//#include <mach/tcccp_ioctl.h>

#include <errno.h>

#include <sys/ioctl.h>
#include "iAP2Callback.h"
#define LOG_TAG "iAP2"

#ifdef __cplusplus
namespace android{
extern "C" {
#endif


#define CP_RESETCTL_LOW	(0)
#define CP_RESETCTL_HI		(1)

#define CP_POWERCTL_OFF	(0)
#define CP_POWERCTL_ON		(1)

#define CP_STATE_DISABLE	(0)
#define CP_STATE_ENABLE	(1)


/*******************************************
I2C Controller IOCTL CODE
********************************************/



#define MAX_BUF_SIZE       1024
#define	IAP_CP_slaveaddr	0x20

#define CP_CHIP_VERSION_20C 0x20C
#define CP_CHIP_VERSION_20B 0x20B

void *piPodIOHandle;

iAP2ChkConn_callback gCpchkConn_cb = NULL;

void iAP2CP_SetConnectionCallback(iAP2ChkConn_callback chkConn_cb)
{
    gCpchkConn_cb = chkConn_cb;
}


void *iAP2CP_MemAlloc(int length) {
    return malloc(length);
}

int iAP2CP_MemDealloc(void *pMem) {
    if (pMem) {
        free(pMem);
    }
    return 0;
}

void iAP2CP_OS_Sleep(int arg,unsigned short time_ms)
{
    IAP2_CTRL_UNUSED(arg);
    usleep(time_ms*1000);
}

void iAP2CP_MemSet(void *pBuffer, unsigned char ucVal, int length) {
    memset(pBuffer,ucVal,length);
}

int iAP2CP_MemCopy(void *pDest,void *pSrc, int length) {
    int retval=0;

    if (pDest && pSrc && length) {
        (void)memcpy(pDest,pSrc,length);
        retval=length;
    }
    return retval;
}

static int iAP2CP_PowerControl(int fd, long OnOff ){
    int ret = 0;
    iAP2LogDbg("%s, %d fd=%d OnOff=%d\r\n",__FUNCTION__,__LINE__,fd,OnOff);
	#ifdef USE_TCC_IAP2
    if(fd){
        if(OnOff != CP_POWERCTL_OFF)
            ret = ioctl(fd, IOCTL_CP_CTRL_PWR, CP_POWERCTL_ON);
        else
            ret = ioctl(fd, IOCTL_CP_CTRL_PWR, CP_POWERCTL_OFF);
        if(ret < 0){
            ALOGE(" %s %d  power control fd %d onOff %ld",__FUNCTION__,__LINE__ ,fd,OnOff);
            return 0;
        }
        ret = 1;
    }
	#endif
    return ret;
}

static int iAP2CP_ResetControl(int fd, long lowHi){
    int ret = 0;
    iAP2LogDbg("%s, %d fd=%d lowHi=%d\r\n",__FUNCTION__,__LINE__,fd,lowHi);
#ifdef USE_TCC_IAP2	
    if(fd){
        if(lowHi != CP_RESETCTL_LOW)
            ret = ioctl(fd, IOCTL_CP_CTRL_RESET, CP_RESETCTL_HI);
        else
            ret = ioctl(fd, IOCTL_CP_CTRL_RESET, CP_RESETCTL_LOW);
        if(ret < 0){
            ALOGE(" %s %d  reset control fd %d lowHi %ld",__FUNCTION__,__LINE__ ,fd,lowHi);
            return 0;
        }
        ret = 1;
    }
	#endif
    return ret;
}

int iAP2CP_SetReset( void *iPodIOHandle )
{
    iAP2_AUTHCP_CONTEXT_t *piPodAuthCPHandle = (iAP2_AUTHCP_CONTEXT_t *)iPodIOHandle;
    int fd = (long)piPodAuthCPHandle->gxp_handle;
    int ret = 0;
    iAP2LogDbg("%s, %d fd=%d",__FUNCTION__,__LINE__,fd);
    if(fd){
        int i = 0;
        int retry = 10;
        int delayMs = 0;
        int delayExtendMs = 5;
        int delayDefaultMs = 5;
        for(i = 0; i < retry;i++){

            if(gCpchkConn_cb != NULL)
            {
                if(gCpchkConn_cb() == FALSE)
                    break;
            }
            delayMs = i*delayExtendMs + delayDefaultMs;
                
            if(piPodAuthCPHandle->cpVersion == CP_CHIP_VERSION_20C)
            {
                ret = iAP2CP_PowerControl(fd, CP_POWERCTL_OFF );
                if ( 0 == ret ){
                    goto EXIT;
                }
                ret = iAP2CP_ResetControl(fd, CP_RESETCTL_LOW);
                if ( 0 == ret ){
                    goto EXIT;
                }
                iAP2CP_OS_Sleep(0,10);            
                ret = iAP2CP_PowerControl(fd, CP_POWERCTL_ON );
                if ( 0 == ret ){
                    return ret;
                }
                iAP2CP_OS_Sleep(0,20);
                piPodAuthCPHandle->auth20CReset = 1;
            }else{
                ret  = iAP2CP_PowerControl((int)fd, CP_POWERCTL_OFF );
                if ( 0 == ret  ){
                    goto EXIT;
                }
                ret  = iAP2CP_ResetControl((int)fd, CP_RESETCTL_LOW );
                if ( 0 == ret  ){
                    goto EXIT;
                }
                iAP2CP_OS_Sleep((int)0,delayMs);
                ret  = iAP2CP_PowerControl((int)fd, CP_POWERCTL_ON );
                if ( 0 == ret  ){
                    goto EXIT;
                }
                iAP2CP_OS_Sleep((int)0,delayMs);
                ret  = iAP2CP_ResetControl((int)fd, CP_RESETCTL_HI );
                if ( 0 == ret  ){
                    goto EXIT;
                }
                iAP2CP_OS_Sleep((int)0,delayMs);
            }
            if(iAP2CP_CheckVer(iPodIOHandle, 1) < 0){
                ALOGE(" %s %d CP Init fail count %d",__FUNCTION__,__LINE__,i);
                ret = 0;
                continue;
            }else{
                break;
            }
        }
    }
EXIT:    
    return ret;
}

void iAP2CP_Set_CpState(int fd,int state)
{
#ifdef USE_TCC_IAP2	

    ioctl((int)fd, IOCTL_CP_SET_STATE, state);
    iAP2LogDbg(" %s %d  CP set state %d",__FUNCTION__,__LINE__ ,state);
	#endif
}

int iAP2CP_Get_CpState(void *iPodIOHandle)
{
    int ret = 0;
    int state = CP_STATE_DISABLE;
	#ifdef USE_TCC_IAP2	
    iAP2_AUTHCP_CONTEXT_t *piPodAuthCPHandle = (iAP2_AUTHCP_CONTEXT_t *)iPodIOHandle;
    int fd = (int)piPodAuthCPHandle->gxp_handle;
    ret = ioctl(fd, IOCTL_CP_GET_STATE, &state);
    if( ret == 0){
        piPodAuthCPHandle->cpState = state;
    }else{
        piPodAuthCPHandle->cpState = CP_STATE_DISABLE;
        state = CP_STATE_DISABLE;
    }
    iAP2LogDbg("tcc-cp, cpState: fd %d 0x%x ret %d\n",fd, state,ret );
	#endif
    return state;
}

void iAP2CP_State_Clear(void * data)
{
    iAP2_AUTHCP_CONTEXT_t *context = NULL;

    iAP2LogDbg(" %s: data %p\n",__FUNCTION__,data);
    if(!data)
        data = piPodIOHandle;
    if(data){
        context = (iAP2_AUTHCP_CONTEXT_t *)data;
        iAP2LogDbg(" %s: handle %d\n",__FUNCTION__,(long)context->gxp_handle);
        iAP2CP_Set_CpState((long)context->gxp_handle, CP_STATE_DISABLE);
    }
}

int iAP2CP_CheckVer(void *iPodIOHandle,int setFlag)
{
    unsigned char devVer = 0;
    unsigned char fwVer = 0;
    unsigned char wantedDevVer = 0;
    unsigned char wantedFwVer = 0;    
    iAP2_AUTHCP_CONTEXT_t *piPodAuthCPHandle = (iAP2_AUTHCP_CONTEXT_t *)iPodIOHandle;
    int hGXPDevice = (long)piPodAuthCPHandle->gxp_handle;

    if(piPodAuthCPHandle->cpVersion == CP_CHIP_VERSION_20C){
        wantedDevVer = 0x05;
        wantedFwVer = 0x01;
    }else{
        wantedDevVer = 0x03;
        wantedFwVer = 0x01;
    }
    
    if(iAP2CP_Read(iPodIOHandle, &devVer,0x00,1) < 0){
        ALOGE(" %s %d  devVer (read fail)",__FUNCTION__,__LINE__ );
        goto EXIT;
    }
    if(devVer != wantedDevVer){
        ALOGE(" %s %d  devVer %d(valid = 3)",__FUNCTION__,__LINE__ ,devVer);
        goto EXIT;
    }
    
    if(iAP2CP_Read(iPodIOHandle, &fwVer,0x01,1) < 0){
        ALOGE(" %s %d  fwVer (read fail)",__FUNCTION__,__LINE__ );
        goto EXIT;
    }
    
    if(fwVer == wantedFwVer){
        iAP2LogDbg(" %s %d  CP Init success",__FUNCTION__,__LINE__ );
        if(setFlag)
            iAP2CP_Set_CpState((int)hGXPDevice, CP_STATE_ENABLE);
        return 0;
    }else{
        ALOGE(" %s %d  fwVer %d(valid = 1)",__FUNCTION__,__LINE__ ,fwVer);
    }
EXIT:
    iAP2CP_Set_CpState(hGXPDevice,CP_STATE_DISABLE);    
    return -1;
}

int iAP2CP_Initialize(void *ExtInfo, void *iPodIOHandle)
{
    iAP2_AUTHCP_CONTEXT_t *piPodAuthCPHandle = (iAP2_AUTHCP_CONTEXT_t *)iPodIOHandle;
    int IsSuccess = 0;

    int hGXPDevice = (long)piPodAuthCPHandle->gxp_handle;
#if 0
    iAP2CP_OS_Sleep(ExtInfo,350);
#else
    int state = iAP2CP_Get_CpState(iPodIOHandle);
    if(state == CP_STATE_ENABLE){
        if(iAP2CP_CheckVer(iPodIOHandle, 1) < 0){
            state = CP_STATE_DISABLE;
            iAP2CP_Set_CpState(hGXPDevice,state);
        }else{
            IsSuccess = 1;
        }
    }

    if(state == CP_STATE_DISABLE){
        if ( hGXPDevice ){
            iAP2LogDbg(" %s %d  reset CP fd %d",__FUNCTION__,__LINE__,(int)hGXPDevice);
            IsSuccess = iAP2CP_SetReset(iPodIOHandle);
        }
        else{
            goto EXIT;
        }
    }else{
        iAP2LogDbg(" %s %d  valid CP state",__FUNCTION__,__LINE__);
    }
#endif
    EXIT:
    if(IsSuccess != 1){
        iAP2CP_Set_CpState((int)hGXPDevice, CP_STATE_DISABLE);
    }
    return IsSuccess;
}


long iAP2CP_Read(void *iPodIOHandle, unsigned char *ptr, unsigned char SUBADDR, unsigned long length)
{
    long ret_val = -1;
	#ifdef USE_TCC_IAP2
    iAP2_AUTHCP_CONTEXT_t *piPodAuthCPHandle = (iAP2_AUTHCP_CONTEXT_t *)iPodIOHandle;
    int fd = (int)piPodAuthCPHandle->i2c_handle;
    int I2C_ChannelNum = piPodAuthCPHandle->i2c_channelnumber;
    int i2c_addr = piPodAuthCPHandle->i2c_slaveaddr;
    iAP2LogDbg(" %s %d  fd = %d i2c address 0x%x SUBADDR 0x%x ptr %p length %d",__FUNCTION__,__LINE__ ,fd,i2c_addr,SUBADDR,ptr,length);
    if (fd && ptr && length){
        struct i2c_msg			i2c_msg[2];
        struct i2c_rdwr_ioctl_data	i2c_rw;

        unsigned char sndBuf[4];
        //unsigned char rcvBuf[MAX_BUF_SIZE];
        iAP2CP_MemSet(i2c_msg, 0x00, sizeof(struct i2c_msg)*2);
        iAP2CP_MemSet(&i2c_rw, 0x00, sizeof(struct i2c_rdwr_ioctl_data));
        iAP2CP_MemSet(sndBuf, 0x00, 4);
        //iAP2CP_MemSet(rcvBuf, 0x00, MAX_BUF_SIZE);
        iAP2CP_MemSet(ptr, 0x00, length);
        sndBuf[0] = SUBADDR;

        i2c_msg[0].addr = i2c_addr;
        //if(piPodAuthCPHandle->cpVersion == (int)CP_CHIP_VERSION_20C)
            i2c_msg[0].flags = I2C_M_TCC_IPOD;

        i2c_msg[0].len = 1;
        i2c_msg[0].buf = sndBuf;

        i2c_msg[1].addr = i2c_addr;
        //if(piPodAuthCPHandle->cpVersion == (int)CP_CHIP_VERSION_20C)
            i2c_msg[1].flags = I2C_M_RD | I2C_M_TCC_IPOD;
        //else
        //    i2c_msg[1].flags = I2C_M_RD;

        i2c_msg[1].len = length;
        //i2c_msg[1].buf = rcvBuf;
        i2c_msg[1].buf = ptr;

        i2c_rw.msgs = i2c_msg;
        i2c_rw.nmsgs = 2;

        ret_val = ioctl(fd, I2C_RDWR, &i2c_rw);
        iAP2LogDbg(" %s %d  ret_val %ld",__FUNCTION__,__LINE__ ,ret_val);
        if(ret_val > 0){
            //iAP2CP_MemCopy(ptr,rcvBuf,length);
        }else{
            ALOGE(" %s %d  ret_val %ld(read fail)",__FUNCTION__,__LINE__ ,ret_val);
        }
    }
    if(piPodAuthCPHandle->cpVersion == (int)CP_CHIP_VERSION_20C){
        if(piPodAuthCPHandle->auth20CReset){
            iAP2CP_OS_Sleep(0,1);
            iAP2CP_ResetControl((int)piPodAuthCPHandle->gxp_handle,CP_RESETCTL_HI);
            piPodAuthCPHandle->auth20CReset = 0;
        }
    }
	#endif
    return ret_val;
}

long iAP2CP_Write(void *iPodIOHandle, unsigned char *ptr, unsigned char SUBADDR, unsigned long length)
{
    iAP2_AUTHCP_CONTEXT_t *piPodAuthCPHandle = (iAP2_AUTHCP_CONTEXT_t *)iPodIOHandle;
    int fd = (long)piPodAuthCPHandle->i2c_handle;
    int i2c_addr = piPodAuthCPHandle->i2c_slaveaddr;
    int ret = 0;
    unsigned char nTemp[1024];
    int i;
    long ret_val = -1;
#ifdef USE_TCC_IAP2
    struct i2c_msg			i2c_msg;
    struct i2c_rdwr_ioctl_data	i2c_rw;

    if (fd && ptr && length)
    {
        if(1){
            nTemp[0]=SUBADDR;
            iAP2CP_MemCopy(&nTemp[1],ptr,length);
            iAP2CP_MemSet(&i2c_msg, 0x00, sizeof(struct i2c_msg));
            iAP2CP_MemSet(&i2c_rw, 0x00, sizeof(struct i2c_rdwr_ioctl_data));
            iAP2CP_MemSet(nTemp, 0x00, MAX_BUF_SIZE);

            nTemp[0] = SUBADDR;
            iAP2CP_MemCopy(&nTemp[1], ptr, length);

            i2c_msg.addr = i2c_addr;
            //if(piPodAuthCPHandle->cpVersion == CP_CHIP_VERSION_20C)
                i2c_msg.flags = I2C_M_TCC_IPOD;        
            i2c_msg.len = 1 + length;
            i2c_msg.buf = nTemp;

            i2c_rw.msgs = &i2c_msg;
            i2c_rw.nmsgs = 1;

            ret_val = ioctl(fd, I2C_RDWR, &i2c_rw);
        }
        if(piPodAuthCPHandle->cpVersion == CP_CHIP_VERSION_20C){
            if(piPodAuthCPHandle->auth20CReset){
                iAP2CP_OS_Sleep(0,1);
                iAP2CP_ResetControl((int)piPodAuthCPHandle->gxp_handle,CP_RESETCTL_HI);
                piPodAuthCPHandle->auth20CReset = 0;
            }
        }
    }
#endif
    return ret_val;
}

void *iAP2CP_AuthCP_Init(void)
{
    void *retval=0;
    iAP2_AUTHCP_CONTEXT_t *piPodContext=(iAP2_AUTHCP_CONTEXT_t*)iAP2CP_MemAlloc(sizeof(iAP2_AUTHCP_CONTEXT_t));

    iAP2LogDbg("iAP2CP_AuthCP_Init\n");
#ifdef USE_TCC_IAP2
    if (piPodContext)
    {
        int ret;
        int fd;
        char buffer[20];
        int bytes;
        iAP2CP_MemSet(piPodContext,0,sizeof(iAP2_AUTHCP_CONTEXT_t));

        fd = open("/dev/tcc-cp", O_RDWR);

        if(fd < 0){
            retval = 0;
        }else{
            piPodContext->gxp_handle=(void *)fd;

            iAP2LogDbg("tcc-cp, fd: %d\n", fd);

            ret = ioctl(fd, IOCTL_CP_GET_VERSION, &piPodContext->cpVersion);
            iAP2LogDbg("tcc-cp, mCPver: 0x%x\n", piPodContext->cpVersion);

            ioctl(fd, IOCTL_CP_GET_STATE, &piPodContext->cpState);
            iAP2LogDbg("tcc-cp, cpState: 0x%x\n", piPodContext->cpState);

            if(ret >= 0){
                piPodContext->i2c_slaveaddr=IAP_CP_slaveaddr>>1;
                //if(piPodContext->cpVersion == CP_CHIP_VERSION_20C){
                    piPodContext->i2c_retries = 30;
                //}else{
                //    piPodContext->i2c_retries = 2;
                //}

                ret = ioctl(fd, IOCTL_CP_GET_CHANNEL, &piPodContext->i2c_channelnumber);
                iAP2LogDbg("iAP2CP_AuthCP_Init: I2C Channel number: %d, ret=%d\n", piPodContext->i2c_channelnumber, ret);

                if(ret >= 0){
                    piPodContext->i2c_channelnumber = piPodContext->i2c_channelnumber;    

                    iAP2CP_MemSet(buffer,0,20);
                    bytes = sprintf(buffer, "/dev/i2c-%ld", piPodContext->i2c_channelnumber);

                    fd = open(buffer, O_RDWR);
                    if ( fd < 0 ) {
                        iAP2LogDbg("iAP2CP_AuthCP_Init: can't open i2c-%d dev: fd=%d\n", piPodContext->i2c_channelnumber, fd);
                        close((int)piPodContext->gxp_handle);
                        retval = 0;
                    }
                    else
                    {
                        iAP2LogDbg("i2c-%d, fd: %d, CPVer:0x%x\n", piPodContext->i2c_channelnumber, fd, piPodContext->cpVersion);
                        piPodContext->i2c_handle=(void *)fd;

                        ret = ioctl((int)piPodContext->i2c_handle, I2C_SLAVE, piPodContext->i2c_slaveaddr);

                        if ( ret < 0 ) {
                            iAP2LogDbg("iAP2CP_AuthCP_Init: can't execute ioctl ret=%d\n", ret);
                            close((int)piPodContext->i2c_handle);
                            retval = 0;
                        }
                        //if(piPodContext->cpVersion == CP_CHIP_VERSION_20C){
                            ret = ioctl((int)piPodContext->i2c_handle, I2C_RETRIES, piPodContext->i2c_retries);

                            if ( ret < 0 ) {
                                iAP2LogDbg("iAP2CP_AuthCP_Init: can't execute the 2nd ioctl ret=%d\n", ret);									
                                close((int)piPodContext->i2c_handle);
                                retval = 0;
                            }				
                        //}
                        //else
                        //{
                        //    iAP2LogDbg("iAP2CP_AuthCP_Init: It is for 20B version");
                        //}

                        retval = (void *)piPodContext;
                    }                                           
                }
            }
        }
    }

    if (retval==0)
    {
    iAP2CP_MemDealloc(piPodContext);
    }
#endif
    return retval;
}

void *iAP2CP_Init()
{
    iAP2_AUTHCP_CONTEXT_t *context = (iAP2_AUTHCP_CONTEXT_t *)iAP2CP_AuthCP_Init();
    iAP2LogDbg(" %s %d i2c %d gxp %d",
                                                            __FUNCTION__,
                                                            __LINE__,
                                                            (long)context->i2c_handle,
                                                            (long)context->gxp_handle
                                                            );
    piPodIOHandle = (void *)context;
    iAP2CP_Initialize((void *)NULL,(void *)context);
    iAP2LogDbg(" %s %d end",__FUNCTION__,__LINE__);
    return (void *)context;
}

void iAP2CP_Deinit(void * data)
{
    iAP2_AUTHCP_CONTEXT_t *context = NULL;

    if(!data)
        data = piPodIOHandle;
    if(data){
        context = (iAP2_AUTHCP_CONTEXT_t *)data;
        iAP2LogDbg(" %s %d ",__FUNCTION__,__LINE__ );
        if((long)context->gxp_handle >= 0){

            if(iAP2CP_Get_CpState(data) == CP_STATE_DISABLE)
            {
                int hGXPDevice = (long)context->gxp_handle;
                iAP2CP_PowerControl(hGXPDevice, CP_POWERCTL_OFF );
                iAP2CP_ResetControl(hGXPDevice, CP_RESETCTL_LOW);
            }
            close((long)context->gxp_handle);
        }

        if((long)context->i2c_handle >= 0){
            close((long)context->i2c_handle);
        }
        
        free(data);
    }
}

#ifdef __cplusplus
}
}
#endif
