/****************************************************************************
 *   FileName    : iAP2EaNativeReceiver.h
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
//#include <arpa/inet.h>

#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <poll.h>
#include <fcntl.h>
#include <linux/netlink.h>
#include <net/if_arp.h>
#include <utils/Log.h>

#include "iAP2EaNativeCustom.h"
#include "iAP2EaNativeTransport.h"
#include "iAP2EaNativeReceiver.h"

//iAP2EaNativeReceiver *iAP2EaNativeReceiver::instance;
bool iAP2EaNativeReceiver::mDebug;

#define LOG_TAG "iAP2EaNative"

iAP2EaNativeNotify_callback iAP2EaNativeReceiver::mNotify_cb;
int iAP2EaNativeReceiver::mNetlinkSocket;
int iAP2EaNativeReceiver::mNetLinkControlPipe[2];
struct sockaddr_nl iAP2EaNativeReceiver::mSockAddr;
char iAP2EaNativeReceiver::mReadBuff[UEVENT_READ_BUFFER_SIZE];
pthread_mutex_t iAP2EaNativeReceiver::mSyncMutex;
pthread_cond_t  iAP2EaNativeReceiver::mSyncCond;      

iAP2EaNativeReceiver::iAP2EaNativeReceiver(iAP2EaNativeNotify_callback notify_cb,bool debug)
{
    mDebug = debug;
    if(mDebug) ALOGI( "%s start", __FUNCTION__);
    mNotify_cb = notify_cb;
    pthread_mutex_init(&mSyncMutex, NULL);
    pthread_cond_init(&mSyncCond, NULL);    
    mNetlinkSocket = -1;
    mNetLinkControlPipe[0] =-1;
    mNetLinkControlPipe[1] =-1;
    mSockAddr.nl_family=AF_NETLINK;
    
    mSockAddr.nl_groups= 1; // TODO : check linux 1 .. android RTMGRP_LINK ?
    if(mDebug) ALOGI( "%s end", __FUNCTION__);
}

iAP2EaNativeReceiver::~iAP2EaNativeReceiver()
{
    if(mDebug) ALOGI( "%s start", __FUNCTION__);
    cleanup();
    pthread_cond_destroy(&mSyncCond);
    pthread_mutex_destroy(&mSyncMutex);
    if(mDebug) ALOGI( "%s end", __FUNCTION__);
}

void iAP2EaNativeReceiver::setDebug(bool debug)
{
    mDebug = debug;
}


#if 0
iAP2EaNativeReceiver *iAP2EaNativeReceiver::getInstance(notify_callback notify_cb) 
{
    if (instance == NULL)
    {
        instance = new iAP2EaNativeReceiver();
    }
    instance->mNotify_cb = notify_cb;
    return instance;
}
#endif

void iAP2EaNativeReceiver::cleanup()
{
    mNotify_cb = NULL;
}

int iAP2EaNativeReceiver::setFdCloexec()
{
    int flags;

#if defined(FD_CLOEXEC)
    flags = fcntl (mNetlinkSocket, F_GETFD);
    if (0 > flags) {
    return -1;
    }

    if (!(flags & FD_CLOEXEC)) 
    {
        fcntl (mNetlinkSocket, F_SETFD, flags | FD_CLOEXEC);
    }
#endif

    flags = fcntl (mNetlinkSocket, F_GETFL);
    if (0 > flags) 
    {
        return -1;
    }

    if (!(flags & O_NONBLOCK)) 
    {
        fcntl (mNetlinkSocket, F_SETFL, flags | O_NONBLOCK);
    }

return 0;
}

void iAP2EaNativeReceiver::closePipe()
{
    if(mNetLinkControlPipe[0] >= 0)
        close(mNetLinkControlPipe[0]);
    if(mNetLinkControlPipe[1] >= 0)
        close(mNetLinkControlPipe[1]);
    mNetLinkControlPipe[0] = -1;
    mNetLinkControlPipe[1] = -1;
}

int iAP2EaNativeReceiver::netLinkPipe()
{
    int ret = pipe(mNetLinkControlPipe);
    if (ret != 0) 
    {
        ALOGE( "ERROR: %s, pipe error: %d\n\n", __FUNCTION__, errno);
        return ret;
    }
    ret = fcntl(mNetLinkControlPipe[1], F_GETFL);
    if (ret == -1) 
    {
        ALOGE( "ERROR: %s, Failed to get pipe fd flags: %d\n", __FUNCTION__, errno);
        goto err_close_pipe;
    }
    ret = fcntl(mNetLinkControlPipe[1], F_SETFL, ret | O_NONBLOCK);
    if (ret != 0) 
    {
        ALOGE( "ERROR: %s, Failed to set non-blocking on new pipe: %d\n", __FUNCTION__, errno);
        goto err_close_pipe;
    }

err_close_pipe:
    if(ret != 0) 
    {
        closePipe();
    }
    return ret;
}
    
int iAP2EaNativeReceiver::create(void)
{
    int ret = -1;
    int socktype = SOCK_RAW;
#if defined(SOCK_CLOEXEC)
    socktype |= SOCK_CLOEXEC;
#endif
#if defined(SOCK_NONBLOCK)
    socktype |= SOCK_NONBLOCK;
#endif

    if(mDebug) 
        ALOGI( "%s in ", __FUNCTION__);

    mNetlinkSocket= socket(PF_NETLINK, socktype, NETLINK_KOBJECT_UEVENT);
    if (mNetlinkSocket == -1&& EINVAL == errno) 
    {
        mNetlinkSocket = socket(PF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
    }

    if (mNetlinkSocket == -1) 
    {
        ALOGE( "ERROR: %s, could not create netlink socket\n", __FUNCTION__);
        return -1;
    }

    ret = setFdCloexec();
    if (0 != ret) 
    {
        ALOGE( "ERROR: %s, set_fd_cloexec_nb error\n", __FUNCTION__);
        ret = -1;
        goto err_close_socket;
    }

    ret = bind(mNetlinkSocket, (struct sockaddr *) &mSockAddr, sizeof(mSockAddr));
    if (0 != ret) 
    {
        ALOGE( "ERROR: %s, bind error\n", __FUNCTION__);
        ret = -2;
        goto err_close_socket;
    }

    ret = netLinkPipe();
    if (ret) 
    {
        ALOGE( "ERROR: %s, could not create netlink control pipe\n", __FUNCTION__);
        ret = -3;
        goto err_close_socket;
    }

    pthread_mutex_lock(&mSyncMutex);
    ret = pthread_create(&mNetLinkEventThread, NULL, eventThreadRun, NULL);
    pthread_cond_wait(&mSyncCond, &mSyncMutex);
    pthread_mutex_unlock(&mSyncMutex);
    if (0 != ret) 
    {
        ret = -4;
        goto err_close_pipe;
    }

    if(mDebug)
        ALOGI( "%s: start iap2audio netlink thread!!!\n", __FUNCTION__);

err_close_pipe:
    if(ret < 0) 
    {
        closePipe();
    }
err_close_socket:
    if(ret < 0) 
    {
        close (mNetlinkSocket);
        mNetlinkSocket = -1;
    }
    ALOGI( "%s: end (%d)\n", __FUNCTION__, ret);
    return ret;
}

int iAP2EaNativeReceiver::destroy(void)
{
    int r;
    char dummy = 1;

    if(mDebug) ALOGI( "%s in ", __FUNCTION__);

    if (-1 == mNetlinkSocket) 
    {
        /* already closed. nothing to do */
        return 0;
    }

    /* Write some dummy data to the control pipe and
     * wait for the thread to exit */
    r = write(mNetLinkControlPipe[1], &dummy, sizeof(dummy));
    if (r <= 0) 
    {
        fprintf(stderr, "ERROR: %s, netlink control pipe signal failed\n", __FUNCTION__);
    }
    
    pthread_join(mNetLinkEventThread, NULL);

    close(mNetlinkSocket);
    mNetlinkSocket = -1;

    /* close and reset control pipe */
    closePipe();

    if(mDebug) ALOGI( "%s out ", __FUNCTION__);

    return 0;
}

const char *iAP2EaNativeReceiver::parse (const char *buffer, size_t len, const char *key)
{
    size_t keylen = strlen(key);
    size_t offset;

    for (offset = 0 ; offset < len && '\0' != buffer[offset] ; offset += strlen(buffer + offset) + 1) 
    {
        if (0 == strncmp(buffer + offset, key, keylen) &&
            '=' == buffer[offset + keylen]) {
            return buffer + offset + keylen + 1;
        }
    }

    return NULL;
}

void iAP2EaNativeReceiver::parseMsg(char *buffer, size_t len) 
{
    const char *tmp;
    int state = -1;

    //ALOGI( "%s: buffer %s", __FUNCTION__,buffer);

    tmp = parse((const char *) buffer, len, "ACTION");
    if (tmp == NULL)
    {
        return;
    }
    
    if ( strcmp(tmp, "change") != 0) 
    {
        //ALOGE("ERROR: %s, unknown device action %s\n", __FUNCTION__, tmp);
        return;
    }

    #if 0
    /* check that this is a uac2 message */
    tmp = parse(buffer, len, "DRIVER");
    if (NULL == tmp || 0 != strcmp(tmp, "snd_uac2")) 
    {
        /* not iap2audio device. ignore */
        return -1;
    }
    #endif

    tmp = parse(buffer, len, "EA");
    if (NULL == tmp) 
    {
        return;
    }

    if (strcmp(tmp, "ALT0") == 0) 
    {
        state = IAP2_NOTI_EAP_NATIVE_STOP;
    }
    else if(strcmp(tmp, "ALT1") == 0) 
    {
        state = IAP2_NOTI_EAP_NATIVE_START;
    }

    switch(state)
    {
        case IAP2_NOTI_EAP_NATIVE_START:
            if(mDebug) ALOGI( "%s: netlink iap2ea state : start\n", __FUNCTION__);
            break;
        case IAP2_NOTI_EAP_NATIVE_STOP:
            if(mDebug) ALOGI( "%s: netlink iap2ea state : stop\n", __FUNCTION__);
            break;
        default:
            break;
    }

    if(state != (-1))
    {
        if(mNotify_cb != NULL)
        {
            mNotify_cb(state,0,0);
        }
    }
}

int iAP2EaNativeReceiver::readMessage(void)
{
    char buffer[1024];
    struct iovec iov;// = {.iov_base = mReadBuff, .iov_len = UEVENT_READ_BUFFER_SIZE};
    struct msghdr meh; // = { .msg_iov=&iov, .msg_iovlen=1,
    		     //.msg_name=&sanl, .msg_namelen=sizeof(sanl) };
    size_t len;

    iov.iov_base = mReadBuff;
    iov.iov_len = UEVENT_READ_BUFFER_SIZE;

    meh.msg_iov = &iov;
    meh.msg_iovlen = 1;
    meh.msg_name = &mSockAddr;
    meh.msg_namelen = sizeof(mSockAddr);

    /* read netlink message */
    memset(mReadBuff, 0, UEVENT_READ_BUFFER_SIZE);
    len = recvmsg(mNetlinkSocket, &meh, 0);
    if (len < 32) 
    {
        if (errno != EAGAIN)
        {
            ALOGE( "ERROR: %s, error recieving message from netlink\n", __FUNCTION__);
        }
        return -1;
    }

    /* TODO -- authenticate this message is from the kernel or udevd */

    parseMsg(mReadBuff, len);

    return 0;
}


void * iAP2EaNativeReceiver::eventThreadRun(void *arg)
{
    char dummy;
    int r;
    #if 1
    struct pollfd fds[2];
    fds[0].fd = mNetLinkControlPipe[0];
    fds[0].events = POLLIN;
    fds[1].fd = mNetlinkSocket;
    fds[1].events = POLLIN;
    #else
    struct pollfd fds[] = {
                { .fd = mNetLinkControlPipe[0],
                    .events = POLLIN },
                {   .fd = mNetlinkSocket,
                    .events = POLLIN },
    };
    #endif

    /* silence compiler warning */
    (void) arg;

    pthread_mutex_lock(&mSyncMutex);
    pthread_cond_signal(&mSyncCond);
    pthread_mutex_unlock(&mSyncMutex);

    while (poll(fds, 2, -1) >= 0) 
    {
        if (fds[0].revents & POLLIN) 
        {
            /* activity on control pipe, read the byte and exit */
            r = read(mNetLinkControlPipe[0], &dummy, sizeof(dummy));
            if (r <= 0) 
            {
                ALOGE("ERROR: %s, netlink control pipe read failed\n", __FUNCTION__);
            }
            break;
        }
        if (fds[1].revents & POLLIN) 
        {
            readMessage();
        }
    }

    return NULL;
}

