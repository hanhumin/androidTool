LOCAL_PATH := $(call my-dir)
INCLUDES := $(LOCAL_PATH)
############################################################################
##############################################################################
INCLUDES += $(LOCAL_PATH)/include

INCLUDES += external/libxml2/include


###############生成libipod so###########################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := libipod
#LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_TAGS := optional
#LOCAL_CPP_EXTENSION := .cc

LOCAL_SRC_FILES :=  iap2/iAP2Link/iAP2FileTransfer.c \
					iap2/iAP2Link/iAP2Link.c \
					iap2/iAP2Link/iAP2LinkAccessory.c \
					iap2/iAP2Link/iAP2LinkRunLoop.c \
					iap2/iAP2Link/iAP2Packet.c \
					iap2/iAP2Utility/iAP2BuffPool.c \
					iap2/iAP2Utility/iAP2FSM.c \
					iap2/iAP2Utility/iAP2ListArray.c \
					iap2/iAP2UtilityImplementation/iAP2BuffPoolImplementation.c \
					iap2/iAP2UtilityImplementation/iAP2Log.c \
					iap2/iAP2UtilityImplementation/iAP2Time.c \
					iap2/iAP2UtilityImplementation/iAP2TimeImplementation.c \
					iap2/iAP2Dev/iAP2Transport.c \
					iap2/iAP2Dev/iAP2Callback.c \
					iap2/iAP2Dev/iAP2Client.cpp \
					iap2/iAP2Dev/iAP2Coprocessor.cpp \
					iap2/iAP2Dev/iAP2EventQueue.cpp \
					iap2/iAP2Dev/iAP2EventSemaphore.cpp \
					iap2/iAP2Dev/iAP2ImplLinkRunLoop.c \
					iap2/iAP2Dev/iAP2Rsd.c \
					iap2/iAP2Dev/iAP2TimeImpl.c \
					iap2/iAP2Dev/iAP2Timer.c \
					iap2/jni/iAP2_jni.cpp \
					iap2/jni/iAP2_utils.cpp \
					iap2/ipod/src/iap_audio_hw.cpp \
					iap2/ipod/src/IPOD/testmain.cpp	\
					iap2/ipod/src/IPOD/IIpodService.cpp	\
					iap2/ipod/src/IPOD/IpodService.cpp	\
					iap2/ipod/src/usbhost.cpp	\
					iap2/ipod/src/IPOD/IIapServiceCallback.cpp	\
					iap2/ipod/src/IPOD/IIapServiceNotify.cpp	\
					iap2/ipod/src/IPOD/IPcmServiceNotify.cpp	\
					iap2/ipod/src/IPOD/PcmServiceNotify.cpp	\
					iap2/ipod/src/IPOD/iPodOnMeidaLibraryInfoMation.cpp	\
					iap2/ipod/src/IPOD/iPodOnMeidaLibraryUpdate.cpp	\
					iap2/ipod/src/IPOD/iPodOnCallback.cpp	\
					iap2/ipod/src/IPOD/iPod_jni.cpp	\
					iap2/ipod/src/IPOD/iPod_interface.c	\
					iap2/ipod/src/IPOD/porting_layer/iPod_HID.c \
					iap2/ipod/src/IPOD/porting_layer/usb.c \
					iap2/ipod/src/IPOD/porting_layer/linux.c \
					iap2/ipod/src/IPOD/porting_layer/error.c \
					iap2/ipod/src/IPOD/porting_layer/descriptors.c
					
    
# \
#					iap2/ipod/src/IPOD/porting_layer/iPod_Timer.c 					
LOCAL_C_INCLUDES := $(LOCAL_PATH)/iap2 $(LOCAL_PATH)/iap2/iAP2Link $(LOCAL_PATH)/iap2/iAP2Utility $(LOCAL_PATH)/iap2/iAP2UtilityImplementation \
					$(LOCAL_PATH)/iap2/iAP2Session $(LOCAL_PATH)/iap2/iAP2Dev $(LOCAL_PATH)/iap2/jni $(LOCAL_PATH)/iap2/iAP2Dev/libiap2 \
					$(LOCAL_PATH)/iap2/ipod/include $(LOCAL_PATH)/iap2/ipod/src $(LOCAL_PATH)/iap2/ipod/src/IPOD/porting_layer $(LOCAL_PATH)/iap2/ipod/src/IPOD \
					$(LOCAL_PATH)/iap2/ipod/src$(JNI_H_INCLUDE) external/tinyalsa/include
					

LOCAL_C_INCLUDES += $(TOP)/frameworks/base/core/jni
LOCAL_CFLAGS += -DJAVA_JNI -Wno-error=return-type -fpermissive -Wno-error=macro-redefined -Wno-error=unused-parameter -Wno-error=missing-declarations -Wno-error=unused-variable \
							-Wno-error=unused-value -Wno-error=sign-compare -Wno-error=unused-private-field -Wno-error=writable-strings -Wno-error=unused-function
LOCAL_LDLIBS    := -lm -llog 

#$(info  " ############## KERNEL_HEADERS =  $(KERNEL_HEADERS)") 
					

#LOCAL_SHARED_LIBRARIES := libdl libstlport libutils libcutils libstdc++ libsysutils libbinder libtinyalsa libmedia libandroid_runtime
LOCAL_SHARED_LIBRARIES := libdl libutils libcutils libstdc++ libsysutils libbinder libtinyalsa libmedia libandroid_runtime
include $(BUILD_SHARED_LIBRARY)


###############生成libipod bin文件###########################################################################
include $(CLEAR_VARS)
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE := libipod
LOCAL_MODULE_TAGS := optional

#LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES :=  iap2/iAP2Link/iAP2FileTransfer.c \
					iap2/iAP2Link/iAP2Link.c \
					iap2/iAP2Link/iAP2LinkAccessory.c \
					iap2/iAP2Link/iAP2LinkRunLoop.c \
					iap2/iAP2Link/iAP2Packet.c \
					iap2/iAP2Utility/iAP2BuffPool.c \
					iap2/iAP2Utility/iAP2FSM.c \
					iap2/iAP2Utility/iAP2ListArray.c \
					iap2/iAP2UtilityImplementation/iAP2BuffPoolImplementation.c \
					iap2/iAP2UtilityImplementation/iAP2Log.c \
					iap2/iAP2UtilityImplementation/iAP2Time.c \
					iap2/iAP2UtilityImplementation/iAP2TimeImplementation.c \
					iap2/iAP2Dev/iAP2Transport.c \
					iap2/iAP2Dev/iAP2Callback.c \
					iap2/iAP2Dev/iAP2Client.cpp \
					iap2/iAP2Dev/iAP2Coprocessor.cpp \
					iap2/iAP2Dev/iAP2EventQueue.cpp \
					iap2/iAP2Dev/iAP2EventSemaphore.cpp \
					iap2/iAP2Dev/iAP2ImplLinkRunLoop.c \
					iap2/iAP2Dev/iAP2Rsd.c \
					iap2/iAP2Dev/iAP2TimeImpl.c \
					iap2/iAP2Dev/iAP2Timer.c \
					iap2/jni/iAP2_jni.cpp \
					iap2/jni/iAP2_utils.cpp \
					iap2/ipod/src/iap_audio_hw.cpp \
					iap2/ipod/src/IPOD/testmain.cpp	\
					iap2/ipod/src/IPOD/IIpodService.cpp	\
					iap2/ipod/src/IPOD/IpodService.cpp	\
					iap2/ipod/src/usbhost.cpp \
					iap2/ipod/src/IPOD/IIapServiceCallback.cpp	\
					iap2/ipod/src/IPOD/IIapServiceNotify.cpp	\
					iap2/ipod/src/IPOD/IPcmServiceNotify.cpp	\
					iap2/ipod/src/IPOD/iPod_interface.c	\
					iap2/ipod/src/IPOD/porting_layer/iPod_HID.c \
					iap2/ipod/src/IPOD/porting_layer/usb.c \
					iap2/ipod/src/IPOD/porting_layer/linux.c \
					iap2/ipod/src/IPOD/porting_layer/error.c \
					iap2/ipod/src/IPOD/porting_layer/descriptors.c
					
    
# \
#					iap2/ipod/src/IPOD/porting_layer/iPod_Timer.c 					
LOCAL_C_INCLUDES := $(LOCAL_PATH)/iap2 $(LOCAL_PATH)/iap2/iAP2Link $(LOCAL_PATH)/iap2/iAP2Utility $(LOCAL_PATH)/iap2/iAP2UtilityImplementation \
					$(LOCAL_PATH)/iap2/iAP2Session $(LOCAL_PATH)/iap2/iAP2Dev $(LOCAL_PATH)/iap2/jni $(LOCAL_PATH)/iap2/iAP2Dev/libiap2 \
					$(LOCAL_PATH)/iap2/ipod/include $(LOCAL_PATH)/iap2/ipod/src $(LOCAL_PATH)/iap2/ipod/src/IPOD/porting_layer $(LOCAL_PATH)/iap2/ipod/src/IPOD \
					$(LOCAL_PATH)/iap2/ipod/src$(JNI_H_INCLUDE) external/tinyalsa/include
					

LOCAL_C_INCLUDES += $(TOP)/frameworks/base/core/jni
LOCAL_CFLAGS += -DJAVA_JNI -Wno-error=return-type -fpermissive -Wno-error=macro-redefined -Wno-error=unused-parameter -Wno-error=missing-declarations -Wno-error=unused-variable \
							-Wno-error=unused-value -Wno-error=sign-compare -Wno-error=unused-private-field -Wno-error=writable-strings -Wno-error=unused-function
LOCAL_LDLIBS    := -lm -llog 

#$(info  " ############## KERNEL_HEADERS =  $(KERNEL_HEADERS)") 
					

#LOCAL_SHARED_LIBRARIES := libdl libstlport libutils libcutils libstdc++ libsysutils libbinder libtinyalsa libmedia libui libandroid_runtime
LOCAL_SHARED_LIBRARIES := libdl libutils libcutils libstdc++ libsysutils libbinder libtinyalsa libmedia libui libandroid_runtime
include $(BUILD_EXECUTABLE)

########################################################################################################

################生成libcarlifevehicle#######################################mm#####################
