/****************************************************************************
 *   FileName    : iPod_CP.c
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
#include <cutils/properties.h>
#include <utils/Log.h>

#include "iPod_PAL.h"
#include "iPod_OS.h"
#include "iPod_HID.h"
#include "iPod_CP.h"
#include <mach/tcccp_ioctl.h>
#include <errno.h>

#include <sys/ioctl.h>
#define CP_RESETCTL_LOW	(0)
#define CP_RESETCTL_HI		(1)

#define CP_POWERCTL_OFF	(0)
#define CP_POWERCTL_ON		(1)

#define CP_STATE_DISABLE	(0)
#define CP_STATE_ENABLE	(1)

IPOD_IO_HANDLE *giPodIOHandle = NULL;
IPOD_AUTHCP_CONTEXT *gpiPodContext = NULL;

#define LOG_TAG "IapService"

int IAPCP_DEBUG = -1;
#define IAPCP_DBUG(msg...)	if (IAPCP_DEBUG == 1) { ALOGD( " " msg); }


/*******************************************
I2C Controller IOCTL CODE
********************************************/



#define MAX_BUF_SIZE       1024

#define	IAP_CP_slaveaddr	0x20

#define CP_CHIP_VERSION_20C 0x20C
#define CP_CHIP_VERSION_20B 0x20B

unsigned int mCPver = 0;
unsigned int mCPI2cChannel = 0;

int fAuth20CReset = 0;
int IPOD_CP_CheckVer(IPOD_IO_HANDLE *iPodIOHandle,int setFlag);
static int IPOD_CP_PowerControl(int hGXPDevice, int OnOff )
{
    int ret = 0;

    IAPCP_DBUG("%d %s start fd(%d)",__LINE__,__FUNCTION__,hGXPDevice );

    if (hGXPDevice)
    {
        IAPCP_DBUG("%d %s ioctl start",__LINE__,__FUNCTION__);

        if (OnOff != CP_POWERCTL_OFF)
        ret = ioctl(hGXPDevice, IOCTL_CP_CTRL_PWR, CP_POWERCTL_ON);
        else
        ret = ioctl(hGXPDevice, IOCTL_CP_CTRL_PWR, CP_POWERCTL_OFF);

        IAPCP_DBUG("%d %s ioctl end",__LINE__,__FUNCTION__);

        if (ret < 0)
        return 0;

        ret = 1;
    }
    IAPCP_DBUG("%d %s end",__LINE__,__FUNCTION__);
    return ret;
}

static int IPOD_CP_ResetControl(int hGXPDevice, int lowHi){
    int ret = 0;
    int fd = hGXPDevice;
    IAPCP_DBUG("%d %s start fd(%d)",__LINE__,__FUNCTION__,fd );
    if(fd){
        if(lowHi != CP_RESETCTL_LOW)
            ret = ioctl(fd, IOCTL_CP_CTRL_RESET, CP_RESETCTL_HI);
        else
            ret = ioctl(fd, IOCTL_CP_CTRL_RESET, CP_RESETCTL_LOW);
        if(ret < 0)
            return 0;
        ret = 1;
    }
    IAPCP_DBUG("%d %s end",__LINE__,__FUNCTION__);
    return ret;
}

int IPOD_CP_SetReset( IPOD_IO_HANDLE *iPodIOHandle)
{
    IPOD_AUTHCP_CONTEXT *piPodAuthCPHandle = (IPOD_AUTHCP_CONTEXT *)iPodIOHandle->iPodAuthHandle;
    int hGXPDevice = (int)piPodAuthCPHandle->gxp_handle;
    int ret = 0;
    
    IAPCP_DBUG("%d %s start fd(%d)",__LINE__,__FUNCTION__,hGXPDevice);
    if(hGXPDevice){
            int i = 0;
            int retry = 10;
            int delayMs = 0;
            int delayExtendMs = 5;
            int delayDefaultMs = 5;
            for(i = 0; i < retry;i++){
                delayMs = i*delayExtendMs + delayDefaultMs;
                        
                if(mCPver == CP_CHIP_VERSION_20C){
                    ret = IPOD_CP_PowerControl(hGXPDevice, CP_POWERCTL_OFF );
                    if ( 0 == ret ){
                        goto EXIT;
                    }
                    ret = IPOD_CP_ResetControl(hGXPDevice, CP_RESETCTL_LOW);
                    if ( 0 == ret ){
                        goto EXIT;
                    }
                     IPOD_OS_Sleep(0,10);
                        
                    ret = IPOD_CP_PowerControl(hGXPDevice, CP_POWERCTL_ON );
                    if ( 0 == ret ){
                        goto EXIT;
                    }
                    IPOD_OS_Sleep(0,20);
                    fAuth20CReset = 1;
                }else{
                    ret = IPOD_CP_PowerControl((int)hGXPDevice, CP_POWERCTL_OFF );
                    if ( 0 == ret ){
                        goto EXIT;
                    }
                    ret = IPOD_CP_ResetControl((int)hGXPDevice, CP_RESETCTL_LOW );
                    if ( 0 == ret ){
                        goto EXIT;
                    }
                    IPOD_OS_Sleep(0,delayMs);
                    ret = IPOD_CP_PowerControl((int)hGXPDevice, CP_POWERCTL_ON );
                    if ( 0 == ret ){
                        goto EXIT;
                    }
                    IPOD_OS_Sleep(0,delayMs);
                    ret = IPOD_CP_ResetControl((int)hGXPDevice, CP_RESETCTL_HI );
                    if ( 0 == ret ){
                        goto EXIT;
                    }
                    IPOD_OS_Sleep(0,delayMs);
                    ret = 1;
                }

                if(IPOD_CP_CheckVer(iPodIOHandle, 1) < 0){
                    ALOGE(" %s %d CP Init fail count %d",__FUNCTION__,__LINE__,i);
                    ret = 0;
                    continue;
                }else{
                    break;
                }
        }
    }
EXIT:    
    IAPCP_DBUG("%d %s end",__LINE__,__FUNCTION__);
    return ret;
}

void IPOD_CP_Set_State(int fd,int state)
{
    ioctl((int)fd, IOCTL_CP_SET_STATE, state);
    IAPCP_DBUG(" %s %d  CP set state %d",__FUNCTION__,__LINE__ ,state);
}

int IPOD_CP_Get_State(IPOD_AUTHCP_CONTEXT *piPodAuthCPHandle)
{
    int ret = 0;
    int state = CP_STATE_DISABLE;
    int fd = (int)piPodAuthCPHandle->gxp_handle;
    
    ret = ioctl(fd, IOCTL_CP_GET_STATE, &state);
    if( ret == 0){
        piPodAuthCPHandle->cpState = state;
    }else{
        piPodAuthCPHandle->cpState = CP_STATE_DISABLE;
        state = CP_STATE_DISABLE;
    }
    IAPCP_DBUG("%s %d   fd %d state %d ret %d\n",__FUNCTION__,__LINE__ ,fd, state,ret );
    return state;
}

void IPOD_CP_Clear()
{
    if( giPodIOHandle != NULL){
        IPOD_AUTHCP_CONTEXT *piPodAuthCPHandle = (IPOD_AUTHCP_CONTEXT *)giPodIOHandle->iPodAuthHandle;
        int hGXPDevice = (int)piPodAuthCPHandle->gxp_handle;

        ALOGI(" %s: hGXPDevice %d\n",__FUNCTION__,hGXPDevice);

        if(hGXPDevice){
            IPOD_CP_Set_State(hGXPDevice, CP_STATE_DISABLE);
        }
    }
}

void IPOD_CP_checkFd()
{
    if(gpiPodContext != NULL)
    {
        ALOGD(" %s \x1b[1;31m  [wskim] tcc-cp %d i2c %d \x1b[1;0m",__FUNCTION__,(int)gpiPodContext->gxp_handle,(int)gpiPodContext->i2c_handle);
        if((int)gpiPodContext->gxp_handle >= 0)
            close((int)gpiPodContext->gxp_handle);
        gpiPodContext->gxp_handle = (void *)-1;
        if((int)gpiPodContext->i2c_handle >= 0)
            close((int)gpiPodContext->i2c_handle);
        gpiPodContext->i2c_handle = (void *)-1;
        free(gpiPodContext);
        gpiPodContext = NULL;
    }
}

int IPOD_CP_CheckVer(IPOD_IO_HANDLE *iPodIOHandle,int setFlag)
{
    unsigned char devVer = 0;
    unsigned char fwVer = 0;
    unsigned char wantedDevVer = 0;
    unsigned char wantedFwVer = 0;
    
    IPOD_AUTHCP_CONTEXT *piPodAuthCPHandle = (IPOD_AUTHCP_CONTEXT *)iPodIOHandle->iPodAuthHandle;
    int hGXPDevice = (int)piPodAuthCPHandle->gxp_handle;
    if(mCPver == CP_CHIP_VERSION_20C){
        wantedDevVer = 0x05;
        wantedFwVer = 0x01;
    }else{
        wantedDevVer = 0x03;
        wantedFwVer = 0x01;
    }
    
    if(IPOD_CP_Read(iPodIOHandle, &devVer,0x00,1) < 0){
        ALOGE(" %s %d devVer (read fail)",__FUNCTION__,__LINE__);
        goto EXIT;
    }
    if(devVer != wantedDevVer){
        ALOGE(" %s %d  devVer %d(valid = 3)",__FUNCTION__,__LINE__ ,devVer);
        goto EXIT;
    }
    
    if(IPOD_CP_Read(iPodIOHandle, &fwVer,0x01,1) < 0){
        ALOGE(" %s %d  fwVer (read fail)",__FUNCTION__,__LINE__ );
        goto EXIT;
    }
    
    if(fwVer == wantedFwVer){
        ALOGI(" %s %d  CP Init success",__FUNCTION__,__LINE__ );
        if(setFlag)
            IPOD_CP_Set_State((int)hGXPDevice, CP_STATE_ENABLE);
        return 0;
    }else{
        ALOGE(" %s %d  fwVer %d(valid = 1)",__FUNCTION__,__LINE__ ,fwVer);
        goto EXIT;
    }
EXIT:
    IPOD_CP_Set_State(hGXPDevice,CP_STATE_DISABLE);
    return -1;
}

IPOD_ERROR IPOD_CP_Init(void *ExtInfo, IPOD_IO_HANDLE *iPodIOHandle)
{
    IPOD_AUTHCP_CONTEXT *piPodAuthCPHandle = (IPOD_AUTHCP_CONTEXT *)iPodIOHandle->iPodAuthHandle;
    IPOD_HID_CONTEXT *piPodHidHandle = NULL;
    int IsSuccess = FALSE;
    IPOD_ERROR ret;
    int hGXPDevice = (int)piPodAuthCPHandle->gxp_handle;
    int state = CP_STATE_DISABLE;

    piPodHidHandle= (IPOD_HID_CONTEXT *)iPodIOHandle->iPodHIDHandle;
    giPodIOHandle = iPodIOHandle;

    IAPCP_DBUG("%d %s start ExtInfo(%p) ",__LINE__,__FUNCTION__,ExtInfo);
    state = IPOD_CP_Get_State(piPodAuthCPHandle);
    if(state == CP_STATE_ENABLE){
        if(IPOD_CP_CheckVer(iPodIOHandle, 0) < 0){
            state = CP_STATE_DISABLE;
        }
    }

    if(state == CP_STATE_DISABLE){
        if ( hGXPDevice )
        {
            IsSuccess = IPOD_CP_SetReset(iPodIOHandle);
        }
        else
        {
            IAPCP_DBUG("IAP_CP_Init() > Invalid parameter");
            goto EXIT;
        }
    }else{
        IAPCP_DBUG(" %s %d  valid CP state",__FUNCTION__,__LINE__);
        IsSuccess= TRUE;
    }

    EXIT:

    if (IsSuccess == TRUE)
    {
        ret = IPOD_NO_ERROR;
    }
    else
    {
        ret = IPOD_AUTH_CP_INIT_ERROR;
        IPOD_CP_Set_State((int)hGXPDevice, CP_STATE_DISABLE);
    }
    IAPCP_DBUG("%d %s end",__LINE__,__FUNCTION__);

    return ret;
}

long IPOD_CP_Read(IPOD_IO_HANDLE *iPodIOHandle, unsigned char *ptr, unsigned char SUBADDR, unsigned long length)
{
    long ret_val = -1;
    IPOD_AUTHCP_CONTEXT *piPodAuthCPHandle = (IPOD_AUTHCP_CONTEXT *)iPodIOHandle->iPodAuthHandle;
    int fd = (int)piPodAuthCPHandle->i2c_handle;
    int I2C_ChannelNum = piPodAuthCPHandle->i2c_channelnumber;
    int i2c_addr = piPodAuthCPHandle->i2c_slaveaddr;

    IAPCP_DBUG("%d %s start addr 0x%x len %ld ",__LINE__,__FUNCTION__,SUBADDR,length);
    if (fd && ptr && length)
    {
        struct i2c_msg			i2c_msg[2];
        struct i2c_rdwr_ioctl_data	i2c_rw;
        unsigned char sndBuf[4];
        unsigned char rcvBuf[MAX_BUF_SIZE];
        IPOD_MemSet(i2c_msg, 0x00, sizeof(struct i2c_msg)*2);
        IPOD_MemSet(&i2c_rw, 0x00, sizeof(struct i2c_rdwr_ioctl_data));
        IPOD_MemSet(sndBuf, 0x00, 4);
        IPOD_MemSet(rcvBuf, 0x00, MAX_BUF_SIZE);
        sndBuf[0] = SUBADDR;
        
        i2c_msg[0].addr = i2c_addr;
        //if(mCPver == CP_CHIP_VERSION_20C)
            i2c_msg[0].flags = I2C_M_TCC_IPOD;
        i2c_msg[0].len = 1;
        i2c_msg[0].buf = (__u8 *)sndBuf;
        
        i2c_msg[1].addr = i2c_addr;
        //if(mCPver == CP_CHIP_VERSION_20C)
            i2c_msg[1].flags = I2C_M_RD | I2C_M_TCC_IPOD;
        //else
        //    i2c_msg[1].flags = I2C_M_RD;

        i2c_msg[1].len = length;
        i2c_msg[1].buf = (__u8 *)rcvBuf;
        
        i2c_rw.msgs = i2c_msg;
        i2c_rw.nmsgs = 2;
#ifdef _DEBUG
        ALOGE("[%s]fd=%d, cmd=0x%02x, len=%d\n",
#endif
        ret_val = ioctl(fd, I2C_RDWR, &i2c_rw);
        //(SUBADDR == 0x10 && length == 1) 
        // "Wait for process completion" of Authentication Data Flows 
        // before reading the Authentication Status on th 2.0B
        if((unsigned long)ret_val != i2c_rw.nmsgs && !(mCPver == CP_CHIP_VERSION_20B && SUBADDR == 0x10 && length == 1))
            ALOGE("%s failed to read %ld addr 0x%x len %ld",__FUNCTION__,ret_val,SUBADDR,length);
        IPOD_MemCopy(ptr,rcvBuf,length);
#ifdef _DEBUG
        ALOGE("[%s]ret=%d\n", __FUNCTION__, ret);
#endif
        if(mCPver == CP_CHIP_VERSION_20C){
            if(fAuth20CReset){
                IPOD_OS_Sleep(0,1);
                IPOD_CP_ResetControl((int)piPodAuthCPHandle->gxp_handle,CP_RESETCTL_HI);
                fAuth20CReset = 0;
            }
        }

        switch(SUBADDR){
            case 0x00:
                if(ptr[0] != 0x03){
                    IAPCP_DBUG("%s device version (0x%x)",__FUNCTION__,ptr[0]);
                }else{
                    IAPCP_DBUG("%s device version (0x%x)",__FUNCTION__,ptr[0]);
                }
                break;
            case 0x01:
                if(ptr[0] != 0x01){
                    IAPCP_DBUG("%s firmware version (0x%x)",__FUNCTION__,ptr[0]);
                }else{
                    IAPCP_DBUG("%s firmware version (0x%x)",__FUNCTION__,ptr[0]);
                }
                break;
            case 0x02:
                if(ptr[0] != 0x02){
                    IAPCP_DBUG("%s auth major (0x%x)",__FUNCTION__,ptr[0]);
                }else{
                    IAPCP_DBUG("%s auth major (0x%x)",__FUNCTION__,ptr[0]);
                }
                break;
            case 0x03:
                if(ptr[0] != 0x00){
                    IAPCP_DBUG("%s auth minor (0x%x)",__FUNCTION__,ptr[0]);
                }else{
                    IAPCP_DBUG("%s auth minor (0x%x)",__FUNCTION__,ptr[0]);
                }
                break;
        }

    }

    IAPCP_DBUG("%d %s end",__LINE__,__FUNCTION__);

    return ret_val;
}

long IPOD_CP_Write(IPOD_IO_HANDLE *iPodIOHandle, unsigned char *ptr, unsigned char SUBADDR, unsigned long length)
{
    IPOD_AUTHCP_CONTEXT *piPodAuthCPHandle = (IPOD_AUTHCP_CONTEXT *)iPodIOHandle->iPodAuthHandle;
    int fd = (int)piPodAuthCPHandle->i2c_handle;
    int i2c_addr = piPodAuthCPHandle->i2c_slaveaddr;
    int ret = 0;
    unsigned char  *nTemp;
    int i;
    long ret_val = -1;	
    struct i2c_msg			i2c_msg;
    struct i2c_rdwr_ioctl_data	i2c_rw;

    IAPCP_DBUG("%d %s start",__LINE__,__FUNCTION__);

    if (fd && ptr && length)
    {
        nTemp=(unsigned char*)IPOD_MemAlloc(length+1);

        if (nTemp)
        {
            nTemp[0]=SUBADDR;
            IPOD_MemCopy(&nTemp[1],ptr,length);
            IPOD_MemSet(&i2c_msg, 0x00, sizeof(struct i2c_msg));
            IPOD_MemSet(&i2c_rw, 0x00, sizeof(struct i2c_rdwr_ioctl_data));
            IPOD_MemSet(nTemp, 0x00, length);
            nTemp[0] = SUBADDR;
            IPOD_MemCopy(&nTemp[1], ptr, length);
            i2c_msg.addr = i2c_addr;
            //if(mCPver == CP_CHIP_VERSION_20C)
                i2c_msg.flags = I2C_M_TCC_IPOD;            
            i2c_msg.len = 1 + length;
            i2c_msg.buf = (__u8 *)nTemp;
            i2c_rw.msgs = &i2c_msg;
            i2c_rw.nmsgs = 1;
#ifdef _DEBUG_I2C
            ALOGE("[%s][%d] ", __FUNCTION__, i2c_msg.len);

            for ( i=0 ; i<i2c_msg.len ; i++ )
            {
                ALOGE("%02x ", sndBuf[i]);
            }

#endif
            ret_val = ioctl(fd, I2C_RDWR, &i2c_rw);
            if((unsigned long)ret_val != i2c_rw.nmsgs)
                ALOGE("%s failed to write %ld",__FUNCTION__,ret_val);
            IPOD_MemDealloc(nTemp);
        }
        if(mCPver == CP_CHIP_VERSION_20C){
            if(fAuth20CReset){
                IPOD_OS_Sleep(0,1);
                IPOD_CP_ResetControl((int)piPodAuthCPHandle->gxp_handle,CP_RESETCTL_HI);
                fAuth20CReset = 0;
            }
        }
    }

    IAPCP_DBUG("%d %s end",__LINE__,__FUNCTION__);
	
    return ret_val;
}

void IPOD_CP_InitDebug(void)
{
    //if(IAPCP_DEBUG == -1)
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.cp", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPCP_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPCP_DEBUG = 0;
                break;
        }
    }
}

void *IPOD_AuthCP_Init(void)
{
    void *retval=0;
    IPOD_AUTHCP_CONTEXT *piPodContext=(IPOD_AUTHCP_CONTEXT*)IPOD_MemAlloc(sizeof(IPOD_AUTHCP_CONTEXT));
    IPOD_CP_InitDebug();

    IPOD_CP_checkFd();
    gpiPodContext = piPodContext;
    piPodContext->gxp_handle = (void *)-1;
    piPodContext->i2c_handle = (void *)-1;
        
    IAPCP_DBUG("%d %s start",__LINE__,__FUNCTION__);
    fAuth20CReset = 0;
        
    if (piPodContext)
    {
        int ret;
        int fd;
        char buffer[20];
        int bytes;
        IPOD_MemSet(piPodContext,0,sizeof(IPOD_AUTHCP_CONTEXT));

        
        fd = open("/dev/tcc-cp", O_RDWR);

        if(fd < 0){
            retval = 0;
            ALOGE("%s end fd %d error %d %s",__FUNCTION__,fd,errno,strerror(errno));
        }else{
            piPodContext->gxp_handle=(void *)fd;

            ret = ioctl(fd, IOCTL_CP_GET_VERSION, &mCPver);
            if(ret >= 0){
                piPodContext->i2c_slaveaddr=IAP_CP_slaveaddr>>1;
                if(mCPver == CP_CHIP_VERSION_20C){
                    piPodContext->i2c_retries = 30;
                }else{
                    piPodContext->i2c_retries = 10;
                }

                ret = ioctl(fd, IOCTL_CP_GET_CHANNEL, &mCPI2cChannel);
                if(ret >= 0){
                    piPodContext->i2c_channelnumber=mCPI2cChannel;    

                    IPOD_MemSet(buffer,0,20);
                    bytes = sprintf(buffer, "/dev/i2c-%d", (int)piPodContext->i2c_channelnumber);

                    fd = open(buffer, O_RDWR);
                    if ( fd < 0 ) {
                        ALOGE("%s end ret %d buffer %s %d %s",__FUNCTION__,ret,buffer,errno,strerror(errno));
                        close((int)piPodContext->gxp_handle);
                        piPodContext->gxp_handle = (void *)-1;
                        retval = 0;
                    }
                    else
                    {
                        piPodContext->i2c_handle=(void *)fd;
                        ret = ioctl((int)piPodContext->i2c_handle, I2C_SLAVE, piPodContext->i2c_slaveaddr);

                        if ( ret < 0 ) {
                            ALOGE("%s i2c_handle  ret %d  %d %s",__FUNCTION__,ret,errno,strerror(errno));
                            close((int)piPodContext->i2c_handle);
                            close((int)piPodContext->gxp_handle);
                            piPodContext->i2c_handle = (void *)-1;
                            piPodContext->gxp_handle = (void *)-1;
                            retval = 0;
                        }
                        //if(mCPver == CP_CHIP_VERSION_20C)
                        {
                            ret = ioctl((int)piPodContext->i2c_handle, I2C_RETRIES, piPodContext->i2c_retries);

                            if ( ret < 0 ) {
                                close((int)piPodContext->i2c_handle);
                                close((int)piPodContext->gxp_handle);
                                piPodContext->i2c_handle = (void *)-1;
                                piPodContext->gxp_handle = (void *)-1;
                                retval = 0;
                            }

                            IAPCP_DBUG("%d %s CP version 0x%x i2c channgel %d",__LINE__,__FUNCTION__,mCPver,mCPI2cChannel);
                        }
                        retval = (void *)piPodContext;
                    }                                           
                }else{
                    close((int)piPodContext->gxp_handle);
                    piPodContext->gxp_handle = (void *)-1;
                    ALOGE("%s IOCTL_CP_GET_CHANNEL ret %d error %d %s",__FUNCTION__,ret,errno,strerror(errno));
                }
            }
            else
            {
                ALOGE("%s IOCTL_CP_GET_VERSION ret %d error %d %s",__FUNCTION__,ret,errno,strerror(errno));
            }
        }
        

    }

    if (retval==0)
    {
        IPOD_MemDealloc(piPodContext);
    }

    IAPCP_DBUG("%d %s end",__LINE__,__FUNCTION__);

    return retval;
}

IPOD_ERROR IPOD_AuthCP_Deinit(void *piPodAuthHandle)
{
    IAPCP_DBUG("%d %s start",__LINE__,__FUNCTION__);
    IPOD_ERROR retval=IPOD_NO_ERROR;
    IPOD_AUTHCP_CONTEXT *piPodContext=piPodAuthHandle;

    int IsSuccess = FALSE;
    IPOD_ERROR ret;
    int hGXPDevice = (int)piPodContext->gxp_handle;

    giPodIOHandle = NULL;

    IAPCP_DBUG("%d %s ",__LINE__,__FUNCTION__);
    if ( hGXPDevice )
    {
        //if(mCPver == CP_CHIP_VERSION_20C)
        if(IPOD_CP_Get_State(piPodAuthHandle) == CP_STATE_DISABLE)
        {
            IAPCP_DBUG("%d %s ",__LINE__,__FUNCTION__);
            IsSuccess = IPOD_CP_PowerControl(hGXPDevice, CP_POWERCTL_OFF );
            IsSuccess = IPOD_CP_ResetControl(hGXPDevice, CP_RESETCTL_LOW);
            IAPCP_DBUG("%d %s ",__LINE__,__FUNCTION__);
        }
    }
    else
    {
        IAPCP_DBUG("IPOD_CP_Deinit() > Invalid parameter");
    }
    
    close((int)piPodContext->i2c_handle);
    close((int)piPodContext->gxp_handle);
    IPOD_MemDealloc(piPodContext);
    gpiPodContext = NULL;
    fAuth20CReset = 0;
    IAPCP_DBUG("%d %s end",__LINE__,__FUNCTION__);
    return retval;
}

int IPOD_Check_WithIap2(void)
{
    int retval = 0;
    int fd;
    int ret = 0;
    unsigned int fWithIAP2 = 0;
    fd = open("/dev/tcc-cp", O_RDWR);

    if(fd < 0){
        goto EXIT;
    }

    ret = ioctl(fd, IOCTL_CP_GET_WITH_IAP2, &fWithIAP2);
    if(ret >= 0){
        retval = fWithIAP2;
    }
EXIT:
    if(fd >= 0)
        close(fd);
    ALOGI("%d %s retval %d",__LINE__,__FUNCTION__,retval);
    return retval;
}
