/****************************************************************************
 *   FileName    : iAP2Interface.h
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


#ifndef _IAP2_INTERFACE_H_
#define _IAP2_INTERFACE_H_

#include <stdarg.h>
#include "iAP2FileTransfer.h"
#include "iAP2LinkRunLoop.h"
#include "iAP2Packet.h"
#include "iAP2Defines.h"
#include "iAP2EventQueue.h"

namespace android{

#if defined(__cplusplus)
extern "C"
{
#endif

enum
{
    IAP2_NOTI_IDEN_ACCEPTED = 0,
    IAP2_NOTI_IDEN_CANCELED = 1,
    IAP2_NOTI_IDEN_REJECTED = 3,
    IAP2_NOTI_INIT_FAILED_IAP1 = 2,
    IAP2_NOTI_AUTH_FAILED = 4,
    IAP2_NOTI_AUTH_SUCCESS = 5,
    IAP2_NOTI_CERTIFICATE_CLEAR = 6,
    IAP2_NOTI_USB_EIO_ERR = 7,

    // Location
    IAP2_NOTI_LOCATION_START = 10,
    IAP2_NOTI_LOCATION_STOP = 11,
    IAP2_NOTI_LOCATION_GPRMC_DATA_STATUS = 12,

    // Vehicle status
    IAP2_NOTI_VEHICLE_STATUS_START = 15,
    IAP2_NOTI_VEHICLE_STATUS_STOP = 16,

    // Power
    IAP2_NOTI_POWER = 20,

    // Device Notification
    IAP2_NOTI_DEV_INFO_NOTI = 25,    
    IAP2_NOTI_DEV_LANGUAGE_NOTI = 26,    
    IAP2_NOTI_DEV_TIME_NOTI = 27,    
    IAP2_NOTI_DEV_UUID_NOTI = 28,    
    
    // Telephony
    IAP2_NOTI_TELEPHONY_CALL_STATE = 30,
    IAP2_NOTI_TELEPHONY_UPDATE = 31,
    IAP2_NOTI_TELEPHONY_VCARD = 32,
    IAP2_NOTI_TELEPHONY_UPDATE_LIST = 33,

    // Now Playing
    IAP2_NOTI_NOW_PLAYING_MEDIA_ITEM = 36,
    IAP2_NOTI_NOW_PLAYING_PLAYBACK = 37,
    IAP2_NOTI_NOW_PLAYING_ARTWORK = 38,
    IAP2_NOTI_NOW_PLAYING_ARTWORK_RAW = 39,

    // Bluetooth Connection
    IAP2_NOTI_BLUETOOTH_CONNECTION = 40,

    // EAP ( Session )
    IAP2_NOTI_EAP_START = 45,
    IAP2_NOTI_EAP_STOP = 46,
    IAP2_NOTI_EAP_DATA = 47,
    IAP2_NOTI_EAP_SENDCOMPLETE = 48,

    // timeout 
    IAP2_NOTI_TIMEOUT_WAITACK = 50,
    IAP2_NOTI_TIMEOUT_WAITDETECT = 51,
    IAP2_NOTI_TIMEOUT_SENDACK = 52,
    IAP2_NOTI_LIMIT_MAX_RESEND = 53,

    // Media Library Access
    IAP2_NOTI_MEDIA_LIB_INFO = 60,
    IAP2_NOTI_MEDIA_LIB_UPDATE = 61,
    IAP2_NOTI_MEDIA_LIB_PLAYLIST = 62,
    IAP2_NOTI_MEDIA_LIB_PLAYCK_LIST = 63,

    // EAP ( Native )
    IAP2_NOTI_EAP_NATIVE_USB_DISCONNECTED = 110,   //3 // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_USB_CONNECTED = 111, //3  // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_START = 112, //3  // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_OPENED = 113, //3 // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_DATA = 114,   //3 // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_IO_ERROR = 115, //3 // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_STOP = 116,   //3 // this enum must not modify.
    
    IAP2_NOTI_COUNT_MAX,
};

enum
{
    IAP2_PARAM_NUM8 = 0,
    IAP2_PARAM_NUM16 = 1,
    IAP2_PARAM_NUM32 = 2,
    IAP2_PARAM_NUM64 = 3,
    IAP2_PARAM_NUMU8 = 4,
    IAP2_PARAM_NUMU16=5,
    IAP2_PARAM_NUMU32 = 6,
    IAP2_PARAM_NUMU64 = 7,
    IAP2_PARAM_ENUM = 8,
    IAP2_PARAM_BOOLEAN  = 9,
    IAP2_PARAM_STRING = 10,
    IAP2_PARAM_BLOB = 11,
    IAP2_PARAM_NONE = 12,
    IAP2_PARAM_GROUP = 13,
    IAP2_PARAM_BLOBFD = 14,
};


// Location
enum
{
    IAP2_LOCATION_GPGGA = 1, //cIAP2LocInfoGlobalPosSystemFixData, // 1 
    IAP2_LOCATION_GPRMC = 2,//cIAP2LocInfoRecomMinSpecGPSTransitData,// 2
    IAP2_LOCATION_GPGSV = 3,//cIAP2LocInfoGPSSatellitesInView,// 3
    IAP2_LOCATION_PASCD = 4,//cIAP2LocInfoVehicleSpeedData,// 4
    IAP2_LOCATION_PAGCD = 5,//cIAP2LocInfoVehicleGyroData,//  5
    IAP2_LOCATION_PAACD = 6,//cIAP2LocInfoVehicleAccelData ,// 6
    IAP2_LOCATION_GPHDT = 7,//cIAP2LocInfoVehicleHeadingData,// 7    
    IAP2_LOCATION_INTERVAL_RSV = 32768, // reserved
    IAP2_LOCATION_INTERVAL_GPGGA = 32769, // uint32 #0/1 GPGGA
    IAP2_LOCATION_INTERVAL_GPRMC = 32770, // uint32 #0/1 GPRMC
    IAP2_LOCATION_INTERVAL_GPGSV = 32771, // uint32 #0/1 GPGSV
    IAP2_LOCATION_INTERVAL_PASCD = 32772, // uint32 #0/1 PASCD
    IAP2_LOCATION_INTERVAL_PAGCD = 32773, // uint32 #0/1 PAGCD
    IAP2_LOCATION_INTERVAL_PAACD = 32774, // uint32 #0/1 PAACD
    IAP2_LOCATION_INTERVAL_GPHDT = 32775, // uint32 #0/1 GPHDT
};

// Power
enum
{
    IAP2_NOTI_POWER_RESERVED = 0,
    IAP2_NOTI_POWER_LOW_POWER = 1,
    IAP2_NOTI_POWER_INTERMITTENT_HIGH_POWER = 2,
};

// Identification interface
enum
{
    IAP2_INTERF_IAP2 = 0,
    IAP2_INTERF_DIPO = 1,
};

// Identification Power
enum
{
    IAP2_PWR_SRC_NONE = 0,
    IAP2_PWR_SRC_RESERVED = 1,
    IAP2_PWR_SRC_SELF_POWER = 2,
};

// Vehicle information
enum
{
    IAP2_VEHICLE_ENGINE_GASOLINE = 0,
    IAP2_VEHICLE_ENGINE_DIESEL = 1,
    IAP2_VEHICLE_ENGINE_ELECTRIC = 2,
    IAP2_VEHICLE_ENGINE_CNG = 3,
};

// sesion type
enum
{
    IAP2_SESS_TYPE_CONTROL = kIAP2PacketServiceTypeControl,  //0 /* Control & Authentication */
    IAP2_SESS_TYPE_FILE = kIAP2PacketServiceTypeBuffer,  //  = 1, /* Buffer (ie. Artwork, Workout) */
    IAP2_SESS_TYPE_EA = kIAP2PacketServiceTypeEA, //       = 2, /* EA stream */
};

// Vehicle status
enum
{
    IAP2_VEHICLE_RANGE = 3,            // none #0/1
    IAP2_VEHICLE_OUTSIDE_TEMP = 4, // none #0/1
    IAP2_VEHICLE_INSIDE_TEMP = 5,   // none #0/1
    IAP2_VEHICLE_RANGE_WARNING = 6, // none #0/1
};

// Telephony
enum
{
    // Telephony Call State Information
    IAP2_CALL_STATE_PHONENUM = 0,
    IAP2_CALL_STATE_CALLERNAME = 1,
    IAP2_CALL_STATE_STATUS = 2,
    IAP2_CALL_STATE_DIRECTION = 3,
    IAP2_CALL_STATE_UNIQUECALLID = 4,
    IAP2_CALL_STATE_VCARD_FILEID = 5,
    IAP2_CALL_STATE_VCARD_DATA = 6,
    
    // Call State Status Value
    IAP2_CALL_STATE_STATUS_DISCONNECTED = 0,
    IAP2_CALL_STATE_STATUS_ACTIVE = 1,
    IAP2_CALL_STATE_STATUS_HOLDING = 2,
    IAP2_CALL_STATE_STATUS_CONNECTING = 3,

    // Call State Direction Value
    IAP2_CALL_DIRECTION_INCOMING = 0,
    IAP2_CALL_DIRECTION_OUTGOING = 1,
    IAP2_CALL_DIRECTION_UNKNOWN = 2,

    // Telephony Signal Strength Value
    IAP2_TELEPHONY_STRENGTH_0 = 0,
    IAP2_TELEPHONY_STRENGTH_1 = 1,
    IAP2_TELEPHONY_STRENGTH_2 = 2,
    IAP2_TELEPHONY_STRENGTH_3 = 3,
    IAP2_TELEPHONY_STRENGTH_4 = 4,
    IAP2_TELEPHONY_STRENGTH_5 = 5,

    // Telephony Registration Status Value
    IAP2_TELEPHONY_REG_UNKNOWN = 0,
    IAP2_TELEPHONY_REG_NOT_REG= 1,
    IAP2_TELEPHONY_REG_SEARCHING = 2,
    IAP2_TELEPHONY_REG_DENIED = 3,
    IAP2_TELEPHONY_REG_REG_HOME = 4,
    IAP2_TELEPHONY_REG_ROAMING = 5,
    IAP2_TELEPHONY_REG_EMERGENCY = 6,
};

// Now Playing
enum
{
    // Media Attributes
    IAP2_NOWPLAY_MEDIA_PERSISTID = 0,
    IAP2_NOWPLAY_MEDIA_TITLE = 1,
    IAP2_NOWPLAY_MEDIA_MEDIATYPE = 2,
    IAP2_NOWPLAY_MEDIA_RATING = 3,
    IAP2_NOWPLAY_MEDIA_PLAYBACK_DURATION = 4,
    IAP2_NOWPLAY_MEDIA_ALBUM_PERSISTID = 5,
    IAP2_NOWPLAY_MEDIA_ALBUM_TITLE = 6,
    IAP2_NOWPLAY_MEDIA_ALBUM_TRACKNUM = 7,
    IAP2_NOWPLAY_MEDIA_ALBUM_TRACKCNT = 8,
    IAP2_NOWPLAY_MEDIA_ALBUM_DISCNUM = 9,
    IAP2_NOWPLAY_MEDIA_ALBUM_DISCCNT = 10,
    IAP2_NOWPLAY_MEDIA_ARTIST_PERSISTID = 11,
    IAP2_NOWPLAY_MEDIA_ARTIST = 12,
    IAP2_NOWPLAY_MEDIA_ALBUMARTIST_PERSISTID = 13,
    IAP2_NOWPLAY_MEDIA_ALBUMARTIST = 14,
    IAP2_NOWPLAY_MEDIA_GENRE_PERSISTID = 15,
    IAP2_NOWPLAY_MEDIA_GENRE = 16,
    IAP2_NOWPLAY_MEDIA_COMPOSE_PERSISTID = 17,
    IAP2_NOWPLAY_MEDIA_COMPOSER = 18,
    IAP2_NOWPLAY_MEDIA_PARTOFCOMPILATION = 19,

    IAP2_NOWPLAY_MEDIA_PROPLIKESUPPORTED = 21,
    IAP2_NOWPLAY_MEDIA_PROP_BANSUPPORTED = 22,
    IAP2_NOWPLAY_MEDIA_PROP_LIKED = 23,
    IAP2_NOWPLAY_MEDIA_PROP_BANNED = 24,
    IAP2_NOWPLAY_MEDIA_RESIDENTONDEVICE = 25,
    IAP2_NOWPLAY_MEDIA_ARTWORK_FILETRANSID = 26,
    IAP2_NOWPLAY_MEDIA_CHAPTER_COUNT = 27,
    IAP2_NOWPLAY_MEDIA_ARTWORK_DATA = 28,
    IAP2_NOWPLAY_MEDIA_ARTWORK_DATA_FD = 29,
    IAP2_NOWPLAY_MEDIA_ARTWORK_DATA_LEN = 30,
	
    //  Playback Attributes
    IAP2_NOWPLAY_PLAYBACK_STATUS = 0,
    IAP2_NOWPLAY_PLAYBACK_ELAPSED_TIME = 1,
    IAP2_NOWPLAY_PLAYBACK_QUEUE_INDEX = 2,
    IAP2_NOWPLAY_PLAYBACK_QUEUE_COUNT = 3,
    IAP2_NOWPLAY_PLAYBACK_QUEUE_CHAPTERINDEX = 4,
    IAP2_NOWPLAY_PLAYBACK_SHUFFLE = 5,
    IAP2_NOWPLAY_PLAYBACK_REPEAT = 6,
    IAP2_NOWPLAY_PLAYBACK_APP_NAME = 7,
    IAP2_NOWPLAY_PLAYBACK_MEDIA_LIB_UID = 8,
    IAP2_NOWPLAY_PLAYBACK_ITUNES_RADIO_AD = 9,
    IAP2_NOWPLAY_PLAYBACK_ITUNES_RADIO_STATIONNAME = 10,
    IAP2_NOWPLAY_PLAYBACK_ITUNES_RADIO_MEDIA_PLAYLISTID = 11,
    IAP2_NOWPLAY_PLAYBACK_PLAYBACK_SPEED = 12,
    IAP2_NOWPLAY_PLAYBACK_SET_ESLAPSED_TIME_AVAILALBE = 13,
    IAP2_NOWPLAY_PLAYBACK_PLAYBACK_QUEUE_LIST_AVAIL = 14,
    IAP2_NOWPLAY_PLAYBACK_PLAYBACK_QUEUE_LIST_TRANSFER_ID = 15,
    IAP2_NOWPLAY_PLAYBACK_PLAYBACK_APP_BUNDLEID= 16    
};

// playstatus
enum
{
    IAP2_PLAYSTATUS_STOPED = 0,
    IAP2_PLAYSTATUS_PLAYING = 1,
    IAP2_PLAYSTATUS_PAUSED = 2,
    IAP2_PLAYSTATUS_SEEK_FORWARD = 3,
    IAP2_PLAYSTATUS_SEEK_BACKWARD = 4,
};

// Bluetooth Component Profiles
enum
{
    IAP2_BT_HANDSFREE = 0,
    IAP2_BT_PHONEBOOKACCESS = 1,
    IAP2_BT_AV_REMOTECONTROL = 3,
    IAP2_BT_ADVANCED_AUDIODISTRIBUTION = 4,
    IAP2_BT_HID = 5,
    IAP2_BT_IAP2LINK = 7,
    IAP2_BT_PERSONALAREANETWORKACCESSPOINT = 8,
    IAP2_BT_MESSAGEACCESS = 9,
    IAP2_BT_PERSONALAREANETWORKCLIENT = 12,
};

typedef enum
{
    IAP2_IDEN_ACC_AUTH = 1,
    IAP2_IDEN_ACC_IDEN = 2,
    IAP2_IDEN_APP_LAUNCH = 3,
    IAP2_IDEN_ASS_TOUCH = 4,
    IAP2_IDEN_BT_CONNECTION = 5,
    IAP2_IDEN_DEVICE_AUTH = 6,
    IAP2_IDEN_DEVICE_NOTI = 7,
    IAP2_IDEN_EAPROTOCOL = 8,
    IAP2_IDEN_HID = 9,
    IAP2_IDEN_LOCATION = 10,
    IAP2_IDEN_MEDIA_LIB = 11,
    IAP2_IDEN_NOW_PLAYING = 12,
    IAP2_IDEN_POWER = 13,
    IAP2_IDEN_USB_DEV_AUDIO = 14,
    IAP2_IDEN_TELEPHONY = 15,
    IAP2_IDEN_VEHICLE_STATUS = 16,
    IAP2_IDEN_VOICE_OVER = 17,
    IAP2_IDEN_WIFI_INFO = 18,
    IAP2_IDEN_DEV_AUTH = 19,
}ControlSessionType_t;

typedef enum
{
    kiAP2MemLogTypeAlloc = 0,
    kiAP2MemLogTypeSendPck,
    kiAP2MemLogTypeCount

} iAP2MemDbgLog_t;

typedef int (*iAP2ConrolGps_callback)(int flag);
typedef int (*iAP2Notify_callback)(int type,int arg0,int arg1,void *obj);
typedef BOOL (*iAP2ChkConn_callback)(void);

typedef struct {
    /** set to sizeof(iap2_interface_callbacks_t) */
    int size;
    iAP2ConrolGps_callback iAP2ConrolGps_cb;
    iAP2Notify_callback iAP2Notify_cb;
} iap2_interface_callbacks_t;

typedef struct {
    /** set to sizeof(iap2_interface_callbacks_t) */
    int size;
    int (*init)(iap2_interface_callbacks_t* callbacks );
} iap2_interface_t;

extern const iap2_interface_t* iAP2InterfaceGetCbFunc ();

#define IAP2_CBACK(P_CB, P_CBACK, ...)\
        if (P_CB && P_CB->P_CBACK) {            \
            P_CB->P_CBACK(__VA_ARGS__);         \
        }                                       \
        else {                                  \
        }


void iAP2InterfaceNotify(uint8_t type, int arg0, int arg1,void *pData);


typedef int (*ListArrayInitCB_t) (uint8_t* buff, uint8_t maxCount, uint8_t dataSize);
typedef uint32_t (*ListArrayGetBuffSizeCB_t) (uint8_t maxCount, uint8_t dataSize);
typedef uint8_t (*ListArrayAddItemAfterCB_t) (uint8_t* listArrayBuffer,
                                                                               uint8_t  prevItemIndex,
                                                                               void*    item);
typedef void (*ListArrayCleanupCB_t) (uint8_t* listArrayBuffer, piAP2ListArrayFunc itemCleanupFunc);
typedef void* (*ListArrayItemForIndexCB_t) (uint8_t* listArrayBuffer, uint8_t index);
typedef uint8_t (*ListArrayFindItemCB_t) (uint8_t*                  listArrayBuffer,
                                                                       void*                     item,
                                                                       piAP2ListArrayCompareFunc func);
typedef uint8_t (*ListArrayGetFirstItemIndexCB_t) (uint8_t* listArrayBuffer);
typedef uint8_t (*ListArrayGetLastItemIndexCB_t) (uint8_t* listArrayBuffer);
typedef uint8_t (*ListArrayGetNextItemIndexCB_t) (uint8_t* listArrayBuffer, uint8_t itemIndex);
typedef void (*ListArrayDeleteItemCB_t) (uint8_t*              listArrayBuffer,
                              uint8_t               index,
                              piAP2ListArrayFunc    itemCleanupFunc);
typedef uint8_t (*ListArrayGetCountCB_t) (uint8_t* listArrayBuffer);

typedef struct
{
    ListArrayInitCB_t ListArrayInit_cb;
    ListArrayAddItemAfterCB_t ListArrayAddItemAfter_cb;
    ListArrayCleanupCB_t ListArrayCleanup_cb;
    ListArrayItemForIndexCB_t ListArrayItemForIndex_cb;
    ListArrayFindItemCB_t ListArrayFindItem_cb;
    ListArrayGetFirstItemIndexCB_t ListArrayGetFirstItemIndex_cb;
    ListArrayGetLastItemIndexCB_t ListArrayGetLastItemIndex_cb;
    ListArrayGetNextItemIndexCB_t ListArrayGetNextItemIndex_cb;
    ListArrayDeleteItemCB_t ListArrayDeleteItem_cb;
    ListArrayGetCountCB_t ListArrayGetCount_cb;
    ListArrayGetBuffSizeCB_t ListArrayGetBuffSize_cb;
}iAP2ListCB_t;

typedef iAP2FileTransfer_st* (*FileTransferCreateCB_t)(iAP2Link_t*                 link,
                                            uint8_t                     session,
                                            uint8_t                     bufferID,
                                            iAP2FileTransferGotDataCB_t callback,
                                            void*                       userInfo,
                                            BOOL                        bRecvAsStream,
                                            iAP2FileTransfer_t*         fileXferBuff);
typedef void (*FileTransferCleanupCB_t) (iAP2FileTransfer_t* fileXfer);
typedef BOOL (*FileTransferHandleRecvCB_t) (iAP2FileTransfer_t*    fileXfer,
                                 const uint8_t*         data,
                                 uint32_t               dataLen);
typedef void (*FileTransferSuccessCB_t) (iAP2FileTransfer_t* fileXfer);
typedef void (*FileTransferResumeCB_t) (iAP2FileTransfer_t* fileXfer);

typedef struct
{
    FileTransferCreateCB_t FileTransferCreate_cb;
    FileTransferCleanupCB_t FileTransferCleanup_cb;
    FileTransferHandleRecvCB_t FileTransferHandleRecv_cb;
    FileTransferSuccessCB_t FileTransferSuccess_cb;
    FileTransferResumeCB_t FileTransferResume_cb;
}iAP2FileCB_t;

typedef void (*LogDbgCB_t) (const char* format, va_list args);
typedef void (*LogErrorCB_t) (const char* format, va_list args);
typedef void (*LogDumpCB_t) (const char* data, int dataLen);

typedef struct
{
    BOOL enableLog;
    LogDbgCB_t LogDbg_cb;
    LogErrorCB_t LogError_cb;
    LogDumpCB_t LogDump_cb;
}iAP2LogCB_t;

typedef BOOL (*LinkQueueSendDataCB_t) (iAP2Link_t*            link,
                            const uint8_t*         payload,
                            uint32_t               payloadLen,
                            uint8_t                session,
                            void*                  context,
                            iAP2LinkDataSentCB_t   callback);
typedef BOOL (*LinkRunLoopRunCB_t) (iAP2LinkRunLoop_t* linkRunLoop);
typedef BOOL (*LinkRunLoopRunOnceCB_t) (iAP2LinkRunLoop_t* linkRunLoop, void* arg);
typedef uint32_t (*LinkGetMaxPayloadSizeCB_t) (iAP2Link_t*            link);

typedef struct
{
    LinkQueueSendDataCB_t LinkQueueSendData_cb;
    LinkRunLoopRunCB_t LinkRunLoopRun_cb;
    LinkRunLoopRunOnceCB_t LinkRunLoopRunOnce_cb;    
    LinkGetMaxPayloadSizeCB_t LinkGetMaxPayloadSize_cb;
}iAP2LinkCb_t;


typedef int32_t (*MFiCreateSignatureCB_t)(
                    		const void *	inDigestPtr, 
                    		size_t			inDigestLen, 
                    		uint8_t **		outSignaturePtr,
                    		size_t *		outSignatureLen );

typedef int32_t (*MFiCopyCertificateCB_t)(uint8_t **outCertificatePtr, size_t *outCertificateLen );

/*
typedef struct
{
    MFiCreateSignatureCB_t MFiCreateSignature_Cb;
    MFiCopyCertificateCB_t MFiCopyCertificate_Cb;
}iAP2AuthCB_t;
*/

typedef struct
{
    iAP2Notify_callback iAP2Notify_cb;
}iAP2NotiCB_t;

typedef  long (*IAP2_CPRead_t)(void *iPodIOHandle, uint8_t *ptr, uint8_t SUBADDR, unsigned long length);
typedef  long (*IAP2_CPWrite_t)(void *iPodIOHandle, uint8_t *ptr, uint8_t SUBADDR, unsigned long length);
typedef  void *(*iAP2_CPInit_t)(void);
typedef  void (*iAP2_CPDeinit_t)(void * data);
typedef  void (*iAP2_CPStateClear_t)(void * data);
typedef struct
{
    IAP2_CPRead_t IAP2_CPRead_cb;
    IAP2_CPWrite_t IAP2_CPWrite_cb;
    iAP2_CPInit_t iAP2_CPInit_cb;
    iAP2_CPDeinit_t iAP2_CPDeinit_cb;
    iAP2_CPStateClear_t iAP2_CPStateClear_cb;
}iAP2CPCB_t;


typedef struct
{
    iAP2ChkConn_callback iAP2ChkConn_cb;
}iAP2Java_t;

typedef iAP2Queue_t* (*eventQueueCreate_t)(void);
typedef void (*eventQueueDestroy_t)(iAP2Queue_t* queue);
typedef BOOL (*eventQueuePut_t)(iAP2Queue_t* queue, iAP2Event_t* event, BOOL priority);
typedef BOOL (*eventQueueGet_t)(iAP2Queue_t* queue, iAP2Event_t* event, uint32_t msec);
typedef struct
{
    eventQueueCreate_t create_cb;
    eventQueueDestroy_t destroy_cb;
    eventQueuePut_t put_cb;
    eventQueueGet_t get_cb;
}iAP2EventQueue_t;

typedef struct
{
    iAP2ListCB_t listInterf;
    iAP2FileCB_t fileInterf;
    iAP2LogCB_t logInterf;
    iAP2LinkCb_t linkInterf;
    //iAP2AuthCB_t authInterf;
    iAP2NotiCB_t  notiInterf;
    iAP2CPCB_t cpInterf;
    iAP2Java_t javaInterf;
    iAP2EventQueue_t eventQueueInterf;
}iAP2InterfaceCB_t;


typedef struct
{
    iAP2CPCB_t cpInterf;
    uint8_t *pCertificatePtr;
    size_t nCertificateLen;
}iAP2CpContext_t;

#if defined(__cplusplus)
};
#endif

}

#endif

