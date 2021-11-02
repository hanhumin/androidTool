#ifndef IAP2_JNI_H
#define IAP2_JNI_H


#include <unistd.h>
#include <nativehelper/JNIHelp.h>

#include "android_runtime/AndroidRuntime.h"

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;
typedef double d64;
typedef unsigned long long u64;
#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus

extern jobject iap2_ep_object;
extern jobject iap2_sessCtrlDispatch_object;
namespace IAP2JNIH{

//JNIEnv* getJNIEnv();


JNIEXPORT 
    void 
    JNICALL
    Java_com_ipod_carlife_manager_iap2manager_Iap2Service_iAP2ClientStart (JNIEnv* env,
												   jobject thiz/*iAP2Notify_callback noti_cb,
                                    void *pData,
                                    void *pCpData,
                                    iAP2ChkConn_callback chkConn_cb,
                                    int transportType,
                                    iAP2ExtWriteAttached_callback attached_cb,
                                    iAP2ExtDoWrite_callback doWrite_cb,
                                    iAP2ExtWriteDetached_callback detached_cb
                                    */);



JNIEXPORT 
    void 
    JNICALL
    Java_com_ipod_carlife_manager_iap2manager_Iap2Service_registerIap2EP(JNIEnv* env, jobject thiz,  jobject obj_ep);
JNIEXPORT 
	void 
	JNICALL
	Java_com_ipod_carlife_manager_iap2manager_Iap2Servicee_accessoryAuthenticationDoChallengeData(JNIEnv* env, jobject thiz,  jint dataLen,jbyteArray dataArray);



void writeEpIAp2(u8 *data, u32 len);

 void iAP2SessCtrlDispatchMsg(
                                                                struct iAP2Link_st* link, 
                                                                uint8_t* data, 
                                                                uint32_t dataLen, 
                                                                uint8_t session
                                                                );
 void iAP2SessCtrlCreateControlSession(uint8_t version,uint8_t type,uint8_t session);
  int AccessoryAuthenticationSerialNumber(uint8_t session);
  int AccessoryAuthenticationDoChallengeData(int dataLen,uint8_t *data,int session);

}
#ifdef __cplusplus
}
#endif	// __cplusplus

#endif
