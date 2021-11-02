
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include "iAP2Client.h"

#include "iAP2_jni.h"
#include "iAP2_utils.h"
#include "iap_audio_hw.h"
#include <media/AudioTrack.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <errno.h>

#include <cutils/properties.h>
#include <cutils/sockets.h>



using namespace android;
using namespace IAP2JNIH;



#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus
#define kUnknownErr                 -6700
typedef int32_t OSStatus;
IpodService *mIpodService;

#include <utils/Log.h>

#define LOGV1(...) __android_log_print(ANDROID_LOG_VERBOSE, "ppp", __VA_ARGS__)
#define LOGD1(...) __android_log_print(ANDROID_LOG_DEBUG , "ppp", __VA_ARGS__)
#define LOGI1(...) __android_log_print(ANDROID_LOG_INFO , "ppp", __VA_ARGS__)
#define LOGW1(...) __android_log_print(ANDROID_LOG_WARN , "ppp", __VA_ARGS__)
#define LOGE1(...) __android_log_print(ANDROID_LOG_ERROR , "ppp", __VA_ARGS__)


#define map_global_value_errno( TEST, VALUE )		( (TEST) ? 0 : global_value_errno(VALUE) )
#define global_value_errno( VALUE )				( (VALUE) ? ( (OSStatus)(VALUE) ) : kUnknownErr )
			
int sound_card = 0;

int tryTest = 0;
int g_sample_rate = 7;//default 44100
int g_play_mod = 2;//0,stop,1,play,2,pause
int g_record_mod = 0;//0,ipod,1,carlife
int bRecord = 0;
int g_startAudioThread = 0;
static const char * PCM_LOCAL_SOCKET_NAME = "loacalsoketautoipod";
static int pcmSocketID = -1;
static BOOL isStartNowPlaying = false;


IapAudioHw* g_captureHandle = NULL; 

void freeSoundCard();
void startRecord( int sound_cardNum);
void* startAudioThread2(void* param);
void startUSBDeviceModeAudio(uint8_t session);
void NowPlayingMediaItemAttributes(uint8_t *data,int dataLen);

void set_record_mode(int mode)
{
  g_record_mod = mode;

  if(mode==0)
  	{

		 pthread_t read_thead;
			 pthread_create(&read_thead, NULL, startAudioThread2,
							  NULL);
  	 
  	}
  else if(mode ==1)
  	{
  	  g_captureHandle->close();
  	}
}

int get_record_mode()
{
  return g_record_mod;
}


void setPlayMode(int mode)
{
  g_play_mod = mode;
}

int getPlayMode()
{
  return g_play_mod;
}


int audio_sampleRates[]={8000,11025,12000,16000,
						 22050,24000,32000,44100,48000} ;




void SetNowPlayingInformation(int ElapsTime,int PlayBackQueueIndex,int fileTransfer,int session)
{
	if(g_iap2handle==NULL)
		return;

  int len = 6;
    uint8_t data[30];
	
  if(ElapsTime >= 0)
  	{
  	  data[len]=0x00;
	  data[len+1]=0x08;
	   data[len+2]=0x00;
	  data[len+3]=0x00;
	   data[len+4]=((ElapsTime&0xff000000) >>24);
	  data[len+5]=((ElapsTime&0x00ff0000) >>16);
	   data[len+6]=((ElapsTime&0x0000ff00) >>8);
	  data[len+7]=(ElapsTime&0x000000ff);
  	  len+=8;
  	}
  if(PlayBackQueueIndex >= 0)
  	{
  	 data[len]=0x00;
	  data[len+1]=0x08;
	   data[len+2]=0x00;
	  data[len+3]=0x01;
	   data[len+4]=((PlayBackQueueIndex&0xff000000) >>24);
	  data[len+5]=((PlayBackQueueIndex&0x00ff0000) >>16);
	   data[len+6]=((PlayBackQueueIndex&0x0000ff00) >>8);
	  data[len+7]=(PlayBackQueueIndex&0x000000ff);
  	 len+=8;
  	}
   if(fileTransfer >= 0)
   	{
   	data[len]=0x00;
	  data[len+1]=0x08;
	   data[len+2]=0x00;
	  data[len+3]=0x02;
	   data[len+4]=((fileTransfer&0xff000000) >>24);
	  data[len+5]=((fileTransfer&0x00ff0000) >>16);
	   data[len+6]=((fileTransfer&0x0000ff00) >>8);
	  data[len+7]=(fileTransfer&0x000000ff);
   	 len+=8;
   	}
     data[0]=0x40;
	 data[1]=0x40;
     data[2]=((len&0xff00)>>8);
	 data[3]=((len&0x00ff));
	 data[4]=0x50;
	 data[5]=0x03;
     uint8_t *buf = (uint8_t*)malloc(len);
	 memcpy(buf,data,len);
  
  
   iAP2LinkQueueSendData (g_iap2handle->runloop->link,
								   buf,
								   len,
								   session,
								   NULL,
								   NULL);
   free(buf);
}









void PlayBackAttributes(uint8_t *data,int dataLen)
{
  int i = 4;
   int param_len = 0;
   int id;
  uint8_t  *s_char;
   int index2;
   int count2;
   int  position ;
   int status;
   int value  = 0;
  while(i<dataLen)
  {  
    param_len =	 ( data[ i ] << 8 ) | data[ i+1 ]; 
	id =  ( data[ i+2 ] << 8 ) | data[ i+3 ];
	//	LOGI("%s line:%d id=%d\n",__FUNCTION__,__LINE__,id);
	switch(id)
	{
	  case 0: //MediaItemPersistentIdentifier
	     status = data[ i+4 ];
		 mIpodService->notifyOnPlayBackStatus(status);
	  break;	  
	  case 1:
  	     position = ( data[ i+4 ] << 24 ) | (data[ i+1+4 ]<<16)  | (data[ i+2 +4] <<8) | data[ i+3+4 ];
		 mIpodService->notifyOnPlayBackPosition(position);
		break;		
	  case 2:	  	
		value = ( data[ i+4 ] << 24 ) | (data[ i+1+4 ]<<16)  | (data[ i+2 +4] <<8) | data[ i+3+4 ];
	  	mIpodService->notifyOnPlayBackQueueIndex(value);
		break;		
	  case 3:	  	
		value = ( data[ i+4 ] << 24 ) | (data[ i+1+4 ]<<16)  | (data[ i+2 +4] <<8) | data[ i+3+4 ];
		mIpodService->notifyOnPlayBackQueueCount(value);
		break;		
	  case 4:
	  		value = ( data[ i+4 ] << 24 ) | (data[ i+1+4 ]<<16)  | (data[ i+2 +4] <<8) | data[ i+3+4 ];
		mIpodService->notifyOnPlayBackQueueChapterIndex(value);
		break;
	  case 5:
	  	  value = data[ i+4 ];
		 mIpodService->notifyOnPlayBackShuffleMode(value);
		break;
	  case 6:
	  	value = data[ i+4 ];
		 mIpodService->notifyOnPlayBackRepeatMode(value);
		break;
	  case 7:
	  	 	s_char = (uint8_t *)malloc(param_len-4);
		   memcpy(s_char,&data[i+4],param_len-4);
		    mIpodService->notifyOnPlayBackAppName((uint8_t *)s_char,param_len-4);
		   free(s_char);
	  
		break;
	  case 8:
	   	s_char = (uint8_t *)malloc(param_len-4);
		   memcpy(s_char,&data[i+4],param_len-4);
		   
		    mIpodService->notifyOnPBMediaLibraryUniqueIdentifier((uint8_t *)s_char,param_len-4);
		   free(s_char);
		   
		break;
	  case 9:
	  	value = data[ i+4 ];
		 mIpodService->notifyOnPBAppleMusicRadioAd(value);
		break;
	  case 10:
	  s_char = (uint8_t *)malloc(param_len-4);
		   memcpy(s_char,&data[i+4],param_len-4);
		    mIpodService->notifyOnPBAppleMusicRadioStationName((uint8_t *)s_char,param_len-4);
		   free(s_char);
		break;
	  case 11:
	  	uint8_t data_array[8];
		for(int k=0;k<(param_len-4);k++)
			{
			  data_array[k] = data[i+4+k];
			}
	  
		    mIpodService->notifyOnPBAppleMusicRadioStationID(data_array,8);
		  
		break;
	  case 12:
	  	 value =  (data[ i+0 +4] <<8) | data[ i+1+4 ];
		 mIpodService->notifyOnPlayBackSpeed(value);
		break;
	  case 13:
	  	  	value = data[ i+4 ];
		 mIpodService->notifyOnSetElapsedTimeAvailable(value);
		break;
	  case 14:
	  	value = data[ i+4 ];
		 mIpodService->notifyOnPlaybackQueueListAvail(value);
	  	
		break;
	  case 15:
	  	  value = data[ i+4 ];
		   mIpodService->notifyOnPlayBackQueueListTransferID(value);
		// mIpodService->notifyOnPlayBackShuffleMode(value);
		break;
	  case 16:
	  	s_char = (uint8_t *)malloc(param_len-4);
		   memcpy(s_char,&data[i+4],param_len-4);
		    mIpodService->notifyOnPlayBackBundleID((uint8_t *)s_char,param_len-4);
		   free(s_char);
	  
		break;
	  case 17:
	
		   mIpodService->notifyOnPlayBackQueueListContentTransfer(0);
		break;
	
		
	}
	
	i+=param_len;
  }
}

void NowPlayingMediaItemAttributes(uint8_t *data,int dataLen)
{
   int i = 4;
   int param_len = 0;
   int id;
   int  duration;
   int value;
    uint8_t data_array[8];
  while(i<dataLen)
  {
    param_len =    ( data[ i ] << 8 ) | data[ i+1 ];
    id =  ( data[ i+2 ] << 8 ) | data[ i+3 ];
	uint8_t *s_char;
	 LOGI("%s line:%d id=%d param_len=%d\n",__FUNCTION__,__LINE__,id,param_len);
		
	switch(id)
	{

	  case 0: //MediaItemPersistentIdentifier
	  
	
	for(int k=0;k<(param_len-4);k++)
			{
			  data_array[k] = data[i+4+k];
			   LOGI("data_array[%d] :0x%02x\n",k,data_array[k]);
			}
		
	   mIpodService->notifyOnMediaItemPID(data_array,8);
      break;
	  case 1:
	  	
		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	  	  s_char = (uint8_t *)malloc(param_len-4);
		  		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		   memcpy(s_char,&data[i+4],param_len-4);
		   		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		    mIpodService->notifyOnMediaItemTile((uint8_t *)s_char,param_len-4);
				//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		   free(s_char);
		
	  	break;
	  case 2:
	  	 value = data[ i+4 ];
		 
		// LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		 mIpodService->notifyMediaItemMediaType(value);
		 //mIpodService->notifyOnPlayBackShuffleMode(value);
	  	break;
	  case 3:
	  	
		//LOGI("%s line:%d \n",__FUNCTION__,__LINE__);
	  		 value = data[ i+4 ];
		  mIpodService->notifyMediaItemRating(value);
		 //mIpodService->notifyMediaItemMediaType(value);
	  	break;
	  case 4:
	  	
	
	  	  duration = ( data[ i+4 ] << 24 ) | (data[ i+1+4 ]<<16)  | (data[ i+2 +4] <<8) | data[ i+3+4 ];
		  //	LOGI("%s line:%d duration=%d\n",__FUNCTION__,__LINE__,duration);
	     mIpodService->notifyOnDuration(duration);
	  	break;
	  case 5:
	  	
		for(int k=0;k<(param_len-4);k++)
			{
			  data_array[k] = data[i+4+k];
			}
	   mIpodService->notifyOnMediaItemAlbumPID(data_array,8);
	  	break;
	  case 6:
	  	 s_char = (uint8_t *)malloc(param_len-4);
		  memcpy(s_char,&data[i+4],param_len-4);
		    mIpodService->notifyOnMediaItemAlumTitle((uint8_t *)s_char,param_len-4);
		   free(s_char);
		  	 
	  	break;
	  case 7:
	  	
		value = 	 (data[ i+4] <<8) | data[ i+1+4 ];
		   mIpodService->notifyMediaItemAlbumTrackNumber(value);
	  	break;
	  case 8:
	  		value = 	 (data[ i+4] <<8) | data[ i+1+4 ];
		   mIpodService->notifyMediaItemAlbumTrackCount(value);
	  	break;
	  case 9:
	  	value = 	 (data[ i+4] <<8) | data[ i+1+4 ];
		   mIpodService->notifyMediaItemAlbumDiscNumber(value);
	  	break;
	  case 10:
	  		value = 	 (data[ i+4] <<8) | data[ i+1+4 ];
		   mIpodService->notifyMediaItemAlbumDiscCount(value);
	  	break;
	  case 11:
	  	
		for(int k=0;k<(param_len-4);k++)
			{
			  data_array[k] = data[i+4+k];
			}
	   mIpodService->notifyOnMediaItemArtistPID(data_array,8);
	  	break;
	  case 12:
	  	 s_char = (uint8_t *)malloc(param_len-4);
		  memcpy(s_char,&data[i+4],param_len-4);
			mIpodService->notifyOnMediaItemArtist((uint8_t *)s_char,param_len-4);
		   free(s_char);
	  	break;
	  case 13:
	  		
		for(int k=0;k<(param_len-4);k++)
			{
			  data_array[k] = data[i+4+k];
			}
	   mIpodService->notifyOnMediaItemAlbumArtistPID(data_array,8);
	  	break;
	  case 14:
	  	 s_char = (uint8_t *)malloc(param_len-4);
		  memcpy(s_char,&data[i+4],param_len-4);
			mIpodService->notifyOnMediaItemAlumArtist((uint8_t *)s_char,param_len-4);
		   free(s_char);
	  	break;
	  case 15:	  	
			
		 for(int k=0;k<(param_len-4);k++)
			 {
			   data_array[k] = data[i+4+k];
			 }
		mIpodService->notifyOnMediaItemGenrePID(data_array,8);
	  	break;
	  case 16:
	  	 s_char = (uint8_t *)malloc(param_len-4);
		  memcpy(s_char,&data[i+4],param_len-4);
			mIpodService->notifyOnMediaItemGenre((uint8_t *)s_char,param_len-4);
		   free(s_char);
	  	break;
	  case 17:
	  		
		 for(int k=0;k<(param_len-4);k++)
			 {
			   data_array[k] = data[i+4+k];
			 }
		mIpodService->notifyOnMediaItemComposerPID(data_array,8);
	  	break;
	  case 18:
	  	 	 s_char = (uint8_t *)malloc(param_len-4);
		  memcpy(s_char,&data[i+4],param_len-4);
			mIpodService->notifyOnMediaItemComposer((uint8_t *)s_char,param_len-4);
		   free(s_char);
	  	break;
	  case 19:
	  	 	value = data[ i+4 ];
			
		 mIpodService->notifyOnMediaItemIsPartOfCompilation(value);
		//	 mIpodService->notifyOnPBAppleMusicRadioAd(value);
	  	break;
	  case 20:
	  		
	  	break;
	  case 21:
		 value = data[ i+4 ];
			
		 mIpodService->notifyOnMediaItemIsLikeSupported(value);
	  	break;
	  case 22:
	  	 value = data[ i+4 ];
			
		 mIpodService->notifyOnMediaItemIsBeanSupported(value);
	  	break;
	  case 23:
	  	
		value = data[ i+4 ];
		   
		mIpodService->notifyOnMediaItemIsLiked(value);
	  	break;
	  case 24:
	  	value = data[ i+4 ];
		   
		mIpodService->notifyOnMediaItemIsBanned(value);
	  	break;
	  case 25:
	  	
	  	value = data[ i+4 ];
		   
		mIpodService->notifyOnMediaItemIsResidentOnDevice(value);
	  	break;
	  case 26:
	  		 value = data[ i+4 ];			 
		mIpodService->notifyMediaItemArtWorkFileTransferID(value);
		//  mIpodService->notifyMediaItemRating(value);
		 
	  	break;
	  case 27:
	  	value = 	 (data[ i+4] <<8) | data[ i+1+4 ];
		 mIpodService->notifyMediaItemChapterCount(value);
	  	break;
		
	}
    i+=param_len;
  }
}

BOOL getMediaItemPid(uint8_t *data,int dataLen,uint8_t *PidData)
{
   int i = 4;
   int param_len = 0;
   int id;
   int  duration;
   int value;
   
	BOOL hasPid = false;

  while(i<dataLen)
  {
    param_len =    ( data[ i ] << 8 ) | data[ i+1 ];
    id =  ( data[ i+2 ] << 8 ) | data[ i+3 ];
	uint8_t *s_char;
	// LOGI("%s line:%d id=%d param_len=%d\n",__FUNCTION__,__LINE__,id,param_len);
		
	switch(id)
	{

	  case 0: //MediaItemPersistentIdentifier
	  hasPid = true;
	
	  for(int k=0;k<(param_len-4);k++)
			{
			  PidData[k] = data[i+4+k];
			  // LOGI("data_array[%d] :0x%02x\n",k,PidData[k]);
			}
		
	  
      break;
	  default:	
		
	  	break;
	}
    i+=param_len;
  }
    return hasPid;
}
BOOL isPidValid(uint8_t *data,int dataLen)
{
  if(dataLen!=8)
  	return false;
  for(int i=0;i<8;i++)
  	if(data[i]!=0)
		return true;
  return false;
}
void NowPlayingUpdates(uint8_t *data,int dataLen,int session)
{
   if(dataLen<10)
	return ;

   int i = 0;
   int j = 6;
	 int param_len0 = 0;
	 int data_len1 = 0;
   int param_len = 0;
  int k1 = 0,k2=0,k3=0;
  uint8_t mediaItemPidData[8] = {0}; 
  BOOL  hasPid = false;
 // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
  while(j<dataLen)
	{
	
	 param_len0 =  ( data[ j ] << 8 ) | data[ j+1 ];
	 if(data[ j +2]==0x00&&data[ j+3 ]==0x00)
	 	{
	 	
		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	 	 // NowPlayingMediaItemAttributes(&data[j],param_len0);
	 	 hasPid = getMediaItemPid(&data[j],param_len0,mediaItemPidData);
		if(hasPid && !isPidValid(mediaItemPidData,8) )
			return;
	 	 
		  mIpodService->notifyOnNowPlayingMediaItemAttributes(&data[j],param_len0);
	 	}
	  else if(data[ j +2]==0x00&&data[ j+3 ]==0x01)
	 	{
	 	
		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	 	  PlayBackAttributes(&data[j],param_len0);
	 	}
	 

	 j+=param_len0;
	}
   
}


#if 0
void MediaLibraryInformationProcess(uint8_t *data,int dataLen,int session)
{



   if(dataLen<10)
	return ;

   int i = 0;
   int j = 6;
   	 int param_len0 = 0;
	 int data_len1 = 0;
   int param_len = 0;
  int k1 = 0,k2=0,k3=0;
  	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
  while(j<dataLen)
  	{
  	 param_len0 =  ( data[ j ] << 8 ) | data[ j+1 ];
     i=4;
	 LOGI("%s line:%d\n",__FUNCTION__,__LINE__);

	 //start here
	 MediaLibraryInformation *pInfo = (MediaLibraryInformation *)malloc(sizeof(MediaLibraryInformation));
	 pInfo->pNext = NULL;
      while(i<param_len0)
	{
	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	 param_len =  ( data[ j+i ] << 8 ) | data[ j+i+1 ];
	  if((data[j+i+2]==0)&& (data[j+i+3]==0 ))//MeidaLibraryName
     	{
     	  char * dataChar = NULL;
     	 
		   dataChar = (char *)malloc(param_len);
		    
		   memcpy(dataChar,&data[j+i+4],param_len);
		   	LOGI("%s line:%d MeidaLibraryName=%s\n",__FUNCTION__,__LINE__,dataChar);
		    mIpodService->notifyOnMediaLibraryName((uint8_t *)dataChar,param_len);
			pInfo->name = (uint8_t *)dataChar;
			pInfo->nameSize = param_len;
			//free(dataChar);
		  
		  k1++;
     	}
     else if((data[j+i+2]==0)&& (data[j+i+3]==1 ))//MeidaLibraryUniqueIdentifier
     	{
     	  char * dataCharId = NULL;
     	 
		   dataCharId = (char *)malloc(param_len);
		   memcpy(dataCharId,&data[j+i+4],param_len);
		   mIpodService->notifyOnMeidaLibraryUID((uint8_t *)dataCharId,param_len);
		   	LOGI("%s line:%d MeidaLibraryUniqueIdentifier=%s\n",__FUNCTION__,__LINE__,dataCharId);
			pInfo->uniqueueIdentifier = (uint8_t *)dataCharId;
			pInfo->uidSize = param_len;
		   // free(dataCharId);
		  // p->uniqueueIdentifier= dataCharId;
		  k2++;
     	}
	   else if((data[j+i+2]==0)&& (data[j+i+3]==2 ))//type
     	{
		 //  p->type = data[j+i+4];
		 LOGI("%s line:%d MediaLibraryInformation type=%d\n",__FUNCTION__,__LINE__,data[j+i+4]);
		 mIpodService->notifyMediaLibraryType( data[j+i+4]);
		 pInfo->type= data[j+i+4];
		 //mIpodService->notifyMediaItemMediaType( data[j+i+4]);
		   k3++;
     	}
 
	 i+=param_len;

	 
	 
	}
	   LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	  addMediaLibraryInformation(pInfo);
	 j+=param_len0;
  	}
   
}

#endif

void iAP2StopMediaLibraryInformation(int sessionId)
{ 
	if(g_iap2handle==NULL)
		return;

   const uint8_t data[] = {0x40,0x40,0x00,0x06,0x4c,0x02};
   //int MessageLen = sizeof(data);

   iAP2LinkQueueSendData (g_iap2handle->runloop->link,
								   data,
								   sizeof(data),
								   sessionId,
								   NULL,
								   NULL);

}

void iAP2StartMediaLibraryInformation(int sessionId)
{ 
	LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);

	if(g_iap2handle==NULL)
		return;
	LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);

   const uint8_t data[] = {0x40,0x40,0x00,0x06,0x4c,0x00};
   //int MessageLen = sizeof(data);

   iAP2LinkQueueSendData (g_iap2handle->runloop->link,
								   data,
								   sizeof(data),
								   sessionId,
								   NULL,
								   NULL);

}

void writeEpIAp2(u8 *data, u32 len){
		
}







unsigned short iAp2GetMessageId(uint8_t* data, 
                            uint32_t dataLen)
{
    unsigned short ret = 0x0;
   if(dataLen < 6)
   	return ret;
    return (data[4]<<8)|data[5];
		
   
}

void iAP2HIDComponent(uint8_t *buff,int *length)
{
   int MessageLen = 0;
   int len = 0;
   uint8_t  payload[128];
  
   payload[2] = 0x00; //USBDeviceTransportComponent id msb
   payload[3] = 18;  //USBDeviceTransportComponent id lsb
   MessageLen+=4;

// TransportComponentIdentifier    
	payload[MessageLen] = 0x00;
	payload[MessageLen+1] = 0x06;
    payload[MessageLen+2] = 0x00;
	payload[MessageLen+3] = 0x00;
	payload[MessageLen+4] = 0x00;
	payload[MessageLen+5] = 0x00;
	MessageLen+=6;

	// TranportcomponentName 
    const char * TranportcomponentName = "Remote";
    len = strlen(TranportcomponentName)+4+1;
	payload[MessageLen]= (uint8_t)(((len & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[MessageLen+1]= (uint8_t)(len & 0x00ff);// Message Length LSB
	
    payload[MessageLen+2] = 0x00;
	payload[MessageLen+3] = 0x01;
	memcpy(&payload[MessageLen+4],TranportcomponentName,strlen(TranportcomponentName));
	MessageLen+=len;
	payload[MessageLen-1]='\0';

	
	payload[MessageLen] = 0x00;
	payload[MessageLen+1] = 0x05;
	payload[MessageLen+2] = 0x00;
	payload[MessageLen+3] = 0x02;
	payload[MessageLen+4] = 1;
	MessageLen+=5;

	
	payload[0]= (uint8_t)(((MessageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[1]= (uint8_t)(MessageLen & 0x00ff);// Message Length LSB
    // &length = MessageLen;
	//memcpy(len,&MessageLen,sizeof(int));
	//buff = (uint8_t *)malloc(MessageLen);
	memcpy(length,&MessageLen,sizeof(int));
	
	memcpy(buff,payload,MessageLen);
//	ll
   
}



void USBDeviceTransportcomponent(uint8_t *buff,int *lenth)
{
   int MessageLen = 0;
   int len = 0;
   uint8_t  payload[128]={0};
   // payload[0] = 0x00; //USBDeviceTransportComponent  length msb
  // payload[03] = 0x15;  //USBDeviceTransportComponent length lsb
   payload[2] = 0x00; //USBDeviceTransportComponent id msb
   payload[3] = 15;  //USBDeviceTransportComponent id lsb
   MessageLen+=4;

// TransportComponentIdentifier    
	payload[MessageLen] = 0x00;
	payload[MessageLen+1] = 0x06;
    payload[MessageLen+2] = 0x00;
	payload[MessageLen+3] = 0x00;
	payload[MessageLen+4] = 0x00;
	payload[MessageLen+5] = 0x00;
	MessageLen+=6;

	// TranportcomponentName 
    const char * TranportcomponentName = "iAP2";
    len = strlen(TranportcomponentName)+4+1;
	payload[MessageLen]= (uint8_t)(((len & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[MessageLen+1]= (uint8_t)(len & 0x00ff);// Message Length LSB
	
    payload[MessageLen+2] = 0x00;
	payload[MessageLen+3] = 0x01;
	memcpy(&payload[MessageLen+4],TranportcomponentName,strlen(TranportcomponentName));
	MessageLen+=len;
	payload[MessageLen-1]='\0';

	
	payload[MessageLen] = 0x00;
	//payload[MessageLen+1] = 0x05;
	payload[MessageLen+1] = 0x04;
	payload[MessageLen+2] = 0x00;
	payload[MessageLen+3] = 0x02;
	//payload[MessageLen+4] = 0x1;
	//MessageLen+=5;
	MessageLen+=4;
	

	payload[MessageLen] = 0x00;
	payload[MessageLen+1] = 0x05;
	payload[MessageLen+2] = 0x00;
	payload[MessageLen+3] = 0x03;
	payload[MessageLen+4] = 6;
	
	payload[MessageLen] = 0x00;
	payload[MessageLen+1] = 0x05;
	payload[MessageLen+2] = 0x00;
	payload[MessageLen+3] = 0x03;
	payload[MessageLen+4] = 7;
	
	payload[MessageLen] = 0x00;
	payload[MessageLen+1] = 0x05;
	payload[MessageLen+2] = 0x00;
	payload[MessageLen+3] = 0x03;
	payload[MessageLen+4] = 8;
	//payload[MessageLen+4] = 0x07;
	//payload[MessageLen+5] = 7;
	//payload[MessageLen+6] = 8;
	//MessageLen+=7;
	
	payload[0]= (uint8_t)(((MessageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
		payload[1]= (uint8_t)(MessageLen & 0x00ff);// Message Length LSB

	
//	memcpy(len,&MessageLen,sizeof(int));
	//buff = (uint8_t *)malloc(MessageLen);

	memcpy(lenth,&MessageLen,sizeof(int));
	memcpy(buff,payload,MessageLen); 
   
}


const uint8_t messagesArraySend[]={
	0x54,0x00,0x54,0x01,0x54,0x02,0x54,0x04,
	0x68,0x00,0x68,0x02,0x68,0x03,/*0xea,0x02,0xea,0x03,0xDA,0x00,0xda,0x02*/
	0x4c,0x00,0x4c,0x02,0x4c,0x03,0x4c,0x05,0x4c,0x06,0x4c,0x07,0x4c,0x08,
	0x4c,0x09,0x50,0x00,0x50,0x02,0x50,0x03,0xae,0x00,0xae,0x02,0xae,
	0x03/*,0xda,0x00,0xda,0x02*/


};

const uint8_t messagesArrayReceived[]={
	0x54,0x03,0x4e,0x09,0x4e,0x0a,0x4e,0x0b,0x4e,0x0c,0x4e,0x0d,
	0x68,0x01,/*,0xDA,0x010xea,0x00,0xea,0x01,*/
	0x4c,0x01,0x4c,0x04,0x50,0x01,0xae,0x01/*,0xda,0x01*/
};


 uint8_t iAP2_identificationInformation [] = {

    0x40,0x40,0x00,0x00,0x1d,0x01,
    // Name
    0x00,0x17,0x00,0x00,'D','i','g','i','t','a','l',' ','A','u','d','i','o',' ','D','e','m','o',0x00,
    // ModelIdentifier
    0x00,0x0B,0x00,0x01,'S','1','0','0','8','6',0x00,
    // Manufacturer
    0x00,0x09,0x00,0x02,'H','S','A','E',0x00,
    // SerialNumber
    0x00,0x0B,0x00,0x03,'S','1','0','0','1','0',0x00,
    // FirmwareVersion
    0x00,0x0A,0x00,0x04,'1','0','0','0','0',0x00,
    // HardwareVersion
    0x00,0x0A,0x00,0x05,'R','e','v',' ','1',0x00,
    // MessagesSentByAccessory
     0x00,0x2a,0x00,0x06,
         0x68,0x00,0x68,0x02,0x68,0x03,
         0x4c,0x00,0x4c,0x02,0x4c,0x03,
         0x4c,0x05,0x4c,0x06,0x4c,0x07,
         0x4c,0x08,0x4c,0x09,0x50,0x00,
         0x50,0x02,0x50,0x03,0xDA,0x00,
         0xDA,0x02,0xae,0x00,0xae,0x02,
         0xae,0x03,
    // MessagesReceivedFromDevice
    0x00,0x18,0x00,0x07,
    		0x68,0x01,
    		0x4c,0x01,0x4c,0x004,
    		0x50,0x01,
    		0xDA,0x01,
    		0xae,0x01,
    		0x4e,0x09,
    		0x4e,0x0a,
    		0x4e,0x0b,
    		0x4e,0x0c,
    // PowerSourceType
    0x00,0x05,0x00,0x08,2,
    // MaximumCurrentDrawnFromDevice
    0x00,0x06,0x00,0x09,0, 0,
    // CurrentLanguage
    0x00,0x07,0x00,0x0C,'e','n',0x00,
   // SupportedLanguage
    0x00,0x07,0x00,0x0D,'e','n',0x00,
    // USBDeviceTransportComponent
    0x00,0x26,0x00,0x0F,
        // TransportComponentIdentifier
        0x00,0x06,0x00,0x00,0, 1,
        // TransportComponentName
        0x00,0x09,0x00,0x01,'i','A','P','2',0x00,
        // TransportSupportsiAP2Connection
        0x00,0x04,0x00,0x02,
        // USBDeviceSupportedAudioSampleRate
        0x00,0x05,0x00,0x03,0x06,
        // USBDeviceSupportedAudioSampleRate
        0x00,0x05,0x00,0x03,0x07,
        // USBDeviceSupportedAudioSampleRate
        0x00,0x05,0x00,0x03,0x08,
    // iAP2HIDComponent
    0x00,0x1A,0x00,0x12,
        // HIDComponentIdentifier
        0x00,0x06,0x00,0x00,0, 1,
        // HIDComponentName
        0x00,0x0B,0x00,0x01,'R','e','m','o','t','e',0x00,
        // HIDComponentFunction
        0x00,0x05,0x00,0x02,1,
};




void iAP2StartIndentificationInfo(int sessionId)
{
	if(g_iap2handle==NULL)
		return;

	int MessageLen = sizeof(iAP2_identificationInformation);

	iAP2_identificationInformation[2] = (uint8_t)(((MessageLen & 0xff00)>> 8)& 0x00ff);
	iAP2_identificationInformation[3] = (uint8_t)(MessageLen & 0x00ff);// Message Length LSB

	

iAP2LinkQueueSendData (g_iap2handle->runloop->link,
								   iAP2_identificationInformation,
								   sizeof(iAP2_identificationInformation),
								   sessionId,
								   NULL,
								   NULL);

}


uint8_t   reportDescriptor[] = {
		   0x05, 0x0C,		   // USAGE_PAGE (Consumer Devices)
		   0x09, 0x01,		   // USAGE (Consumer Control)
		   0xA1, 0x01,		   // COLLECTION (Application)
			   0x09, 0xCD,		   // USAGE (Play/Pause)
			   0x09, 0xB5,		   // USAGE (Scan Next Track)
			   0x09, 0xB6,		   // USAGE (Scan Previous Track)
			   0x09, 0xb1,		   // USAGE (Unassigned)
			   0x09, 0xb0,		   // USAGE (Unassigned)
			   0x09, 0x00,		   // USAGE (Unassigned)
			   0x09, 0x00,		   // USAGE (Unassigned)
			   0x09, 0x00,		   // USAGE (Unassigned)
			   0x15, 0x00,		   //	LOGICAL_MINIMUM (0)
			   0x25, 0x01,		   //	LOGICAL_MAXIMUM (1)
			   0x95, 0x08,		   //	REPORT_COUNT (8)
			   0x75, 0x01,		   //	REPORT_SIZE (1)
			   0x81, 0x42,		   //	INPUT (Data,Var,Abs,Null)
			   0xC0 			   // END_COLLECTION
	   };


	uint8_t   reportDescriptorMedia[] = {
		#if 0
				0x05,0x0c, // USAGE_PAGE (Consumer Devices)
				0x09,0x01,// USAGE (Consumer Control)
				0xa1,0x01,// COLLECTION (Application)
				0x15,0x00,// Logical Minnum 0
				0x25,0x01,// Logical Maxnum 0
				0x75,0x01,// report Size
				0x95,0x07,//report count
					0x09,0xcd, /*play pause 0x01*/
					0x09,0xb5, /*next 0x02*/
					0x09,0xb6, /*previous 0x04*/
					0x09,0xb0, /*play 0x08*/
					0x09,0xb1, /*pause 0x10*/
					0x09,0xb9, /*random play 0x20*/
					0x09,0xbc, /*repeat 0x40*/					
				0x81,0x02,
				0x75,0x03,
				0x95,0x01,
				0x81,0x03,
				0xc0
				#endif
				 0x05, 0x0C,		   // USAGE_PAGE (Consumer Devices)
		   0x09, 0x01,		   // USAGE (Consumer Control)
		   0xA1, 0x01,		   // COLLECTION (Application)
			   0x09, 0xCD,		   // USAGE (Play/Pause) 1
			   0x09, 0xB5,		   // USAGE (Scan Next Track) 2
			   0x09, 0xB6,		   // USAGE (Scan Previous Track) 4
			   0x09, 0xb1,		   // USAGE (Unassigned) 8
			   0x09, 0xb0,		   // USAGE (Unassigned) 10
			   0x09, 0xbc,		   // USAGE (Unassigned) 20
			   0x09, 0xb9,		   // USAGE (Unassigned) 40
			   0x09, 0x00,		   // USAGE (Unassigned)
			   0x15, 0x00,		   //	LOGICAL_MINIMUM (0)
			   0x25, 0x01,		   //	LOGICAL_MAXIMUM (1)
			   0x95, 0x08,		   //	REPORT_COUNT (8)
			   0x75, 0x01,		   //	REPORT_SIZE (1)
			   0x81, 0x42,		   //	INPUT (Data,Var,Abs,Null)
			   0xC0 			   // END_COLLECTION
};

   

   uint8_t	 keyboard_reportDescriptor[] = {
										  0x05,0x01,
										  0x09,0x06,
										  0xa1,0x01,
										  0x05,0x08,
										  0x15,0x00,
										  0x25,0x01,
										  0x09,0x02,
										  0x75,0x01,
										  0x95,0x01,
										  0x91,0x02,
										  0x75,0x07,
										  0x95,0x01,
										  0x91,0x03,
										  0x05,0x07,
										  0x19,0xe0,
										  0x29,0xe7,
										  0x75,0x01,
										  0x95,0x08,
										  0x81,0x02,
										  0x15,0x00,
										  0x26,0xff,0x00,
										  0x19,0x00,
										  0x2a,0xff,0x00,
										  0x75,0x08,
										  0x95,0x05,
										  0x81,0x00,
										  0x05,0x0c,
										  0x15,0x00,
										  0x25,0x01,
										  0x09,0x40,
										  0x0a,0x21,0x02,
										  0x0a,0xae,0x01,
										  0x09,0xb6,
										  0x09,0xcd,
										  0x09,0xb5,
										  0x09,0xe2,
										  0x09,0xea,
										  0x09,0xe9,
										  0x09,0x30,
										  0x71,0x01,
										  0x95,0x0a,
										  0x81,0x02,
										  0x75,0x06,
										  0x95,0x01,
										  0x81,0x03,
										  0xc0
										
										 
									  };


void playBackAccessoryHidReport(int session,int hidReport)//int hid report
{
	if(g_iap2handle==NULL)
		return;

		int messageLen = 0;
	 int len;
	// uint32_t payloadLen = messageLen;

	// uint32_t payloadLen = messageLen;
	 uint8_t payload[17];
		//  usleep(10000);//sleep 10ms
	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB


	payload[4]= 0x68;// message id MSB
	payload[5]= 0x02;// message id LSB
	messageLen +=6;


		len = 6;
	payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =	31+4
	payload[messageLen+2]= 0x0;// parameter id id MSB
	payload[messageLen+3]= 0x00;// parameter id LSB
	payload[messageLen+4]= 0x00;// 
	payload[messageLen+5]= 0x01;// 0
	
	messageLen+=len;

		len = 5;
	payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =	31+4
	payload[messageLen+2]= 0x0;// parameter id id MSB
	payload[messageLen+3]= 0x01;// parameter id LSB
	payload[messageLen+4]= hidReport;// 

	messageLen+=len;

	payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB
	
	  BOOL ret = iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							payload,
							messageLen,
							session,
							NULL,
							NULL);
}	
void pauseAccessoryHidReport(int session)
{
  
    
   playBackAccessoryHidReport(session,PAUSE_HID_REPORT_ID);
}

void repeatAccessoryHidReport(int session)
{
  

   playBackAccessoryHidReport(session,PLAY_REPEAT_HID_REPORT_ID);
}
void randomAccessoryHidReport(int session)
{
  

   playBackAccessoryHidReport(session,PLAY_RANDOM_HID_REPORT_ID);
}

void hid0AccessoryHidReport()
{
	playBackAccessoryHidReport(g_sessionId,PLAY_NONE_HID_REPORT_ID);

}
 

void playAccessoryHidReport(int session)
{
 
 playBackAccessoryHidReport(session,PLAY_HID_REPORT_ID);
}


void playPauseAccessoryHidReport(int session)
{
 
 
 playBackAccessoryHidReport(session,PLAY_PAUSE_HID_REPORT_ID);
}


void testAccessoryHidReport1(int session)
{
	if(g_iap2handle==NULL)
		return;

	 int messageLen = 0;
	  int len;
	 // uint32_t payloadLen = messageLen;
	 
	 // uint32_t payloadLen = messageLen;
	  uint8_t payload[17];
		 //  usleep(10000);//sleep 10ms
	 payload[0]= 0x40;//start of message MSB
	 payload[1]= 0x40;// start of message LSB
	 
   
	 payload[4]= 0x68;// message id MSB
	 payload[5]= 0x02;// message id LSB
	 messageLen +=6;


		 len = 6;
	 payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	 payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =  31+4
	 payload[messageLen+2]= 0x0;// parameter id id MSB
	 payload[messageLen+3]= 0x00;// parameter id LSB
	 payload[messageLen+4]= 0x00;// 
	 payload[messageLen+5]= 0x01;// 0
	 messageLen+=len;

		 len = 5;
	 payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	 payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =  31+4
	 payload[messageLen+2]= 0x0;// parameter id id MSB
	 payload[messageLen+3]= 0x01;// parameter id LSB
	 payload[messageLen+4]= 0x00;// 
	 
	 messageLen+=len;

	 payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	 payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB
	   BOOL ret = iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							 payload,
							 messageLen,
							 session,
							 NULL,
							 NULL);
}
void playNextTrackAccessoryHidReport(int session)
{
 
 playBackAccessoryHidReport(session,NEXT_HID_REPORT_ID);
}

void playProvTracAccessoryHidReport(int session)
{
 playBackAccessoryHidReport(session,PREVIOUS_HID_REPORT_ID);
}

	  //scan next track
void playPauseAccessoryHidReport2(int session)
{
		  if(g_iap2handle==NULL)
			  return;

	 int messageLen = 0;
	  int len;
	 // uint32_t payloadLen = messageLen;
	 
	 // uint32_t payloadLen = messageLen;
	  uint8_t payload[17];
		 //  usleep(10000);//sleep 10ms
	 payload[0]= 0x40;//start of message MSB
	 payload[1]= 0x40;// start of message LSB
	 
   
	 payload[4]= 0x68;// message id MSB
	 payload[5]= 0x02;// message id LSB
	 messageLen +=6;


		 len = 6;
	 payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	 payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =  31+4
	 payload[messageLen+2]= 0x0;// parameter id id MSB
	 payload[messageLen+3]= 0x00;// parameter id LSB
	 payload[messageLen+4]= 0x00;// 
	 payload[messageLen+5]= 0x01;//0
	 messageLen+=len;

		 len = 5;
	 payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	 payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =  31+4
	 payload[messageLen+2]= 0x0;// parameter id id MSB
	 payload[messageLen+3]= 0x01;// parameter id LSB
	// payload[messageLen+4]= 0x02;// scan next track
	// payload[messageLen+4]= 0x04;// scan prov track
	payload[messageLen+4]= 0x00;//none
	 
	 messageLen+=len;

	 payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	 payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB

	   BOOL ret = iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							 payload,
							 messageLen,
							 session,
							 NULL,
							 NULL);
}
	
	void startPcmSocketClient()
	{

/*
	 struct sockaddr_un addr= {};
      socklen_t socklen;
	    pcmSocketID = socket(AF_LOCAL, SOCK_STREAM, 0);
	   
      if(pcmSocketID < 0)
      {
      LOGI("startPcmSocketClient error 1\n");
          return ;
      }

	  
	  addr.sun_family = AF_LOCAL; 
	  strlcpy(addr.sun_path, PCM_LOCAL_SOCKET_NAME, sizeof(addr.sun_path)); 
	   socklen = strlen(PCM_LOCAL_SOCKET_NAME) + offsetof(sockaddr_un, sun_path) + 1;
	  if(connect(pcmSocketID, reinterpret_cast<sockaddr*>(&addr), socklen) < 0)
      {
       LOGI("startPcmSocketClient error 2 error = %s \n",strerror(errno));
	   pcmSocketID = -1;
         return ;
      }
	  */
	  if(pcmSocketID!=-1)
	  	 close(pcmSocketID);
	  pcmSocketID = socket_local_client( PCM_LOCAL_SOCKET_NAME ,
			ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
		if(pcmSocketID < 0)
			 LOGI("startPcmSocketClient error 3 error = %s \n",strerror(errno));
		LOGI("startPcmSocketClient error 4 error = %s \n",strerror(errno));

	
	}
	void startRecord( int sound_cardNum)
	{
	 
	   int gBufferSize = 2048;
	    int frameCount = 0;
		if(g_captureHandle !=NULL)
			{
		///	g_captureHandle->close();
		    //delete g_captureHandle;
			//g_captureHandle ==NULL;
			}
	  
	   // int fdfile;
		//fdfile=open("/storage/emulated/0/Download/test.pcm",O_WRONLY|O_CREAT,0666);
		

		uint8_t  gPcmBuff[gBufferSize];//= (uint8_t * )malloc(sizeof(uint8_t )*gBufferSize);
		
		while(1)
		{
             
		   // if(get_record_mode() == 1)//carlife for record;
		    	//{
		    //  break;
		    	//}
		   // 	LOGI("%s line:%d\n",__FILE__,__LINE__);
			//LOGI("start Record sound_card=%d\n",sound_card);

		   if(getPlayMode()== PLAY_MODE_PAUSE)
		   	{
		   	    sleep(1);
				  LOGI("%s line:%d\n",__FILE__,__LINE__);
			  	continue;
		   	}
		   else if(getPlayMode()==PLAY_MODE_STOP)
		   	{
		   	    	LOGI("%s line:%d\n",__FILE__,__LINE__);
					 g_captureHandle->close();
				
			      	LOGI("%s line:%d\n",__FILE__,__LINE__);
					//close(pcmSocketID);
		   	  break;
		   	}
	
		 
		int readBytes = g_captureHandle->read(gPcmBuff, gBufferSize);
		  
		if(readBytes>0 )
			{
				if(pcmSocketID!=-1)
				 write(pcmSocketID,gPcmBuff,readBytes);
		
			usleep(10*1000);
			}
		else
			{
			    if(readBytes == -19 || readBytes == -16){
					    	LOGI(" error %s line:%d\n",__FILE__,__LINE__);
					
                 
             }
				    
				sleep(1);
              
			}
		}
	
		if(g_captureHandle != NULL)
		  g_captureHandle->close();
		    
		if(g_captureHandle != NULL)
		delete g_captureHandle;
		   
		g_captureHandle = NULL;
		    	
        g_startAudioThread = 0;
		    	
	}

   void iAP2StopUSBDeviceModeAudio(uint8_t session)
	   {
	   	if(g_iap2handle==NULL)
		return;
		 int messageLen = 0;
		 int len;
		 uint8_t payload[6];
		 usleep(10000);//sleep 10ms
		 payload[0]= 0x40;//start of message MSB
		  payload[1]= 0x40;// start of message LSB
		  payload[2]= 0x00;//start of message MSB
		  payload[3]= 0x06;// start of message LSB
		  payload[4]= 0xda;//start of message MSB
		  payload[5]= 0x02;// start of message LSB
   
		  BOOL ret = iAP2LinkQueueSendData (g_iap2handle->runloop->link,
										   payload,
										   6,
										   session,
										   NULL,
										   NULL);
	   }

void PowerSourceUpdates(uint8_t session)
	{
		if(g_iap2handle==NULL)
	return;
	  int messageLen = 0;
	int len;
	uint8_t payload[26]={0x40,0x40,0x00,0x11,0xae,0x03,
	0x00,0x06,0x00,0x00,0x03,0xe8,
	0x00,0x05,0x00,0x01,0x01};
	
	

	BOOL ret = iAP2LinkQueueSendData (g_iap2handle->runloop->link,
									payload,
									17,
									session,
									NULL,
									NULL);
	}
   
  void startPowerUpdates(uint8_t session)
	{
		if(g_iap2handle==NULL)
	return;
	  int messageLen = 0;
	int len;
	uint8_t payload[26]={0x40,0x40,0x00,0x1a,0xae,0x00,
	0x00,0x04,0x00,0x00,
	0x00,0x04,0x00,0x01,
	0x00,0x04,0x00,0x04,
	0x00,0x04,0x00,0x05,
	0x00,0x04,0x00,0x06};
	
	

	BOOL ret = iAP2LinkQueueSendData (g_iap2handle->runloop->link,
									payload,
									26,
									session,
									NULL,
									NULL);
	}
   void startUSBDeviceModeAudio(uint8_t session)
   	{
   		if(g_iap2handle==NULL)
		return;
   	  int messageLen = 0;
	  int len;
	  uint8_t payload[6];
   	 // usleep(10000);//sleep 10ms
   	  payload[0]= 0x40;//start of message MSB
	   payload[1]= 0x40;// start of message LSB
	   payload[2]= 0x00;//start of message MSB
	   payload[3]= 0x06;// start of message LSB
	   payload[4]= 0xda;//start of message MSB
	   payload[5]= 0x00;// start of message LSB

	   BOOL ret = iAP2LinkQueueSendData (g_iap2handle->runloop->link,
										payload,
										6,
										session,
										NULL,
										NULL);
   	}

 void startHid(uint8_t session)
{
	 if(g_iap2handle==NULL)
		 return;

	 int messageLen = 0;
	 int len;
	// uint32_t payloadLen = messageLen;
	usleep(10000);
	 uint8_t payload[1024];
	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB
	//payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	//payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB
  
	payload[4]= 0x68;// message id MSB
	payload[5]= 0x00;// message id LSB
	messageLen +=6;

	//HIDComponentIdentifier
	len = 6;
	payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =	31+4
	payload[messageLen+2]= 0x0;// parameter id id MSB
	payload[messageLen+3]= 0x00;// parameter id LSB
	payload[messageLen+4]= 0x00;// 
	payload[messageLen+5]= 0x01;// 
	messageLen+=len;

	//vendorIdentifier
	len = 6;
	payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =	31+4
	payload[messageLen+2]= 0x0;// parameter id id MSB
	payload[messageLen+3]= 0x01;// parameter id LSB
	//payload[messageLen+4]= 0x05;// 
	//payload[messageLen+5]= 0xac;// 
	payload[messageLen+4]= 0x18;// 
	payload[messageLen+5]= 0xd1;// 
	messageLen+=len;

	//productIdentifier
	len = 6;
	payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =	31+4
	payload[messageLen+2]= 0x0;// parameter id id MSB
	payload[messageLen+3]= 0x02;// parameter id LSB
	//payload[messageLen+4]= 0x12;//0x00;// 
	//payload[messageLen+5]= 0xa8;//0x00;// 
	payload[messageLen+4]= 0x4e;//0x00;// 
	payload[messageLen+5]= 0x42;//0x00;//
	messageLen+=len;

	//playback hid report
	len = 4+sizeof(reportDescriptor);
	payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =	31+4
	payload[messageLen+2]= 0x0;// parameter id id MSB
	payload[messageLen+3]= 0x04;// parameter id LSB
    memcpy(&payload[messageLen+4],reportDescriptor,sizeof(reportDescriptor));
	messageLen+=len;

	
	payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB
	
	 uint8_t * challengReciveBuf = (uint8_t*)malloc(messageLen);
	
	 memcpy(challengReciveBuf,payload,messageLen);
	BOOL ret =  iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							challengReciveBuf,
							messageLen,
							session,
							NULL,
							NULL);
	
	 free(challengReciveBuf);


}
void startHid2(uint8_t session)
{
	if(g_iap2handle==NULL)
		return;

		int messageLen = 0;
		int len;
	   // uint32_t payloadLen = messageLen;
	   usleep(10000);
		uint8_t payload[1024];
	   payload[0]= 0x40;//start of message MSB
	   payload[1]= 0x40;// start of message LSB
	   //payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	   //payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB
	 
	   payload[4]= 0x68;// message id MSB
	   payload[5]= 0x00;// message id LSB
	   messageLen +=6;

	   //HIDComponentIdentifier
	   len = 6;
	   payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	   payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =  31+4
	   payload[messageLen+2]= 0x0;// parameter id id MSB
	   payload[messageLen+3]= 0x00;// parameter id LSB
	   payload[messageLen+4]= 0x00;// 
	   payload[messageLen+5]= 0x01;// 
	   messageLen+=len;

	   //vendorIdentifier
	   len = 6;
	   payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	   payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =  31+4
	   payload[messageLen+2]= 0x0;// parameter id id MSB
	   payload[messageLen+3]= 0x01;// parameter id LSB
	   payload[messageLen+4]= 0x05;// 
	   payload[messageLen+5]= 0xac;// 
	   //payload[messageLen+4]= 0x18;// 
	  // payload[messageLen+5]= 0xd1;// 
	   messageLen+=len;

	   //productIdentifier
	   len = 6;
	   payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	   payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =  31+4
	   payload[messageLen+2]= 0x0;// parameter id id MSB
	   payload[messageLen+3]= 0x02;// parameter id LSB
	   payload[messageLen+4]= 0x12;//0x00;// 
	   payload[messageLen+5]= 0xa8;//0x00;// 
	// payload[messageLen+4]= 0x00;//0x00;// 
	  // payload[messageLen+5]= 0x00;//0x00;//
	   messageLen+=len;

	   //playback hid report
	   len = 4+sizeof(reportDescriptorMedia);
	   payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	   payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =  31+4
	   payload[messageLen+2]= 0x0;// parameter id id MSB
	   payload[messageLen+3]= 0x04;// parameter id LSB
	   memcpy(&payload[messageLen+4],reportDescriptorMedia,sizeof(reportDescriptorMedia));
	   messageLen+=len;

	   //////////////////////////////////////////////////////
	   payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	   payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB
				 
		uint8_t * challengReciveBuf = (uint8_t*)malloc(messageLen);
	   
		memcpy(challengReciveBuf,payload,messageLen);
	   BOOL ret =  iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							   challengReciveBuf,
							   messageLen,
							   session,
							   NULL,
							   NULL);
	  
		free(challengReciveBuf);


}


 void iAP2StopHid(int session)
{	
if(g_iap2handle==NULL)
		return;

  iAP2StopNowPlayingUpdates( session);
  iAP2StopMediaLibraryInformation( session);
 // iAP2StopMediaLibraryUpdates(int session,uint8_t * mediaLibraryUid,int sizeUid)

	 int messageLen = 0;
	 int len;
	// uint32_t payloadLen = messageLen;
	usleep(10000);
	 uint8_t payload[1024];
	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB
	//payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	//payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB
  
	payload[4]= 0x68;// message id MSB
	payload[5]= 0x03;// message id LSB
	messageLen +=6;

	//HIDComponentIdentifier
	len = 6;
	payload[messageLen]= (uint8_t)(((len & 0xff00) >> 8)& 0xff);// parameter length MSB
	payload[messageLen+1]= (uint8_t)(len & 0x00ff);// parameter lenth LSB 35 =	31+4
	payload[messageLen+2]= 0x0;// parameter id id MSB
	payload[messageLen+3]= 0x00;// parameter id LSB
	payload[messageLen+4]= 0x00;// 
	payload[messageLen+5]= 0x01;// 
	messageLen+=len;

	payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB
			  
	 uint8_t * challengReciveBuf = (uint8_t*)malloc(messageLen);
	
	 memcpy(challengReciveBuf,payload,messageLen);
	 iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							challengReciveBuf,
							messageLen,
							session,
							NULL,
							NULL);
	 free(challengReciveBuf);


}




void USBDeviceModeAudioInformation(uint8_t* data, 
                            uint32_t dataLen, 
                            uint8_t session)
{
  
   if(dataLen<11)
   	return ;

   int i = 6;
   int param_len = 0;
  
   while(i<dataLen)
   	{
   	 param_len =  ( data[ i ] << 8 ) | data[ i+1 ];
   if((param_len==5)&&(data[i+2]==0)&& (data[i+3]==0 ))
   	{
   	  g_sample_rate = data[i+4];

	  mIpodService->notifyUSBDeviceModeAudioInformation(g_sample_rate);
	  //OnUSBDeviceModeAudioInformation(g_sample_rate);
   	  return;
   	}
     i+=param_len;
   	}
}

 typedef struct SoundCard{	 /* 定义单链表结点类型 */
	 char *element;
	 int type;
	 struct SoundCard *next;
 }SoundCard;
 
 SoundCard *pSoundCard = NULL;

 void freeSoundCard()
 	{
 
	  SoundCard * p = pSoundCard;
	  while(p!=NULL)
	  	{
	  	  SoundCard *pt = p;		  
	  	  p = p->next;

		  free(pt->element);
		  free(pt);
	  	}
	  pSoundCard = NULL;
 	}
 void addToSoundCard (const char * name,int type)
 	{
 	  if(pSoundCard==NULL)
 	  	{
	  	pSoundCard = (SoundCard *)malloc(sizeof(SoundCard));
		char * element = (char *)malloc(strlen(name));
		strcpy(element,name);
		pSoundCard->element = element;
		pSoundCard->type = type;
		pSoundCard->next = NULL;
		
 	  	}
	    else
	    	{
	    	  SoundCard *	tempSoundCard = (SoundCard *)malloc(sizeof(SoundCard));
		char * element = (char *)malloc(strlen(name));
		strcpy(element,name);
		tempSoundCard->element = element;
		tempSoundCard->type = type;
		tempSoundCard->next = NULL;
		SoundCard * p = pSoundCard;

		while(p->next!=NULL)
			{
			  p = p->next;
			}
		p->next = tempSoundCard;
	    	}

		
 	}

int is_begin_with(const char * str1,char *str2)
{
  if(str1 == NULL || str2 == NULL)
    return -1;
  int len1 = strlen(str1);
  int len2 = strlen(str2);
  if((len1 < len2) || (len1 == 0 || len2 == 0))
    return -1;
  char *p = str2;
  int i = 0;
  while(*p != '\0')
  {
    if(*p != str1[i])
      return 0;
    p++;
    i++;
  }
  return 1;
}


 
int readFileListTwo(char *basePath)
{
 DIR *dir;
 struct dirent *ptr;
 char base[1000];
  char link_path[256];
    char sound_path[256];
  if ((dir=opendir(basePath)) == NULL)
   {
       //LOGI("######### NO PATH");
	   return 0;
   	  }
    
   	  while ((ptr=readdir(dir)) != NULL)
   	  {
       		  if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
   			  continue;
   		 
		  if((is_begin_with(ptr->d_name,"iPod")==1 ||is_begin_with(ptr->d_name,"iPhone")==1 ) 
		  	&& ptr->d_type == 10)
		  	{
		  	LOGI("##### readFileListTwo ptr->d_name=%s\n",ptr->d_name);
			   if(pSoundCard ==NULL)
			   	{
			   	  memset(link_path,0,256);
				  memset(sound_path,0,256);
				  sprintf(sound_path,"%s/%s",basePath,ptr->d_name);
				  LOGI("sound_path=%s\n",sound_path);
				  readlink(sound_path, link_path, 256);
				   LOGI("link_path=%s\n",link_path);
				  if(is_begin_with(link_path,"card")==1)
				  	{
				  	   char c = link_path[4];
					     LOGI("111 c=%c\n",c);
					    sound_card = c - '0'; 
					   //sound_card = 2;
						closedir(dir);
						return 0;
				  	}
			   	}
			     else
			   	{
			   SoundCard * p = pSoundCard;
			   int bNew = 0;
			   while(p!=NULL)
			   	{
			   	  if(strcmp(p->element,ptr->d_name)!=0)
			   	  	{
			   	  	 
					 memset(link_path,0,256);
									  memset(sound_path,0,256);
									 sprintf(sound_path,"%s/%s",basePath,ptr->d_name);
									  LOGI("222 sound_path=%s\n",sound_path);
									  readlink(sound_path, link_path, 256);
									     LOGI("222 link_path=%s\n",link_path);
									  if(is_begin_with(link_path,"card")==1)
										{
										   char c = link_path[4];
										    LOGI("222 c=%c\n",c);
										    sound_card = c - '0';
										   // sound_card = 2;
											closedir(dir);
											return 0;
										}

			   	  	}
			   	  p = p->next;
			   	}
			   	}
		  	}
   	  }
   	  closedir(dir);
   	  return 1;
    }

int readFileListOne(char *basePath)
{
 DIR *dir;
 struct dirent *ptr;
 char base[1000];
  if ((dir=opendir(basePath)) == NULL)
   {
       //LOGI("######### NO PATH");
	   return 0;
   	  }
   	  while ((ptr=readdir(dir)) != NULL)
   	  {
       		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
   			  continue;
		
		  if(is_begin_with(ptr->d_name,"sound")==1 && ptr->d_type == 10)
		  	{
			   addToSoundCard(ptr->d_name,ptr->d_type);
		  	}
   	  }
	  
   	  closedir(dir);
   	  return 1;
    }

void* startAudioThread2(void* param) 
{
	
	  sleep(1);
	  readFileListTwo("/proc/asound");
	  freeSoundCard();
	    int  ret = g_captureHandle->open(16384, 10000, 20000,sound_card);
	   while(ret < 0)
	   	{
	   	  sleep(1);
		  ret = g_captureHandle->open(16384, 10000, 20000,sound_card);
	   	}
	   return NULL;
   //   startRecord(sound_card);
}


void* startAudioThread(void* param) 
{
	 setPlayMode( PLAY_MODE_PAUSE);
	//  sleep(1);
	  //for test
	 
	 // 
	 // mIpodService->StartMediaLibraryUpdates();
	// mIpodService->StartNowPlayingUpdates();
	// mIpodService->StartMediaLibraryInformation();
	 //  sleep(1);
	   
	   //
	  readFileListTwo("/proc/asound");
	  freeSoundCard();
	  LOGI("sound_card sound_card=%d \n",sound_card);
	   int  ret = g_captureHandle->open(16384, 10000, 20000,sound_card);
	   while(ret < 0)
	   	{
	   	  sleep(1);
		  ret = g_captureHandle->open(16384, 10000, 20000,sound_card);
	   	}
	  startPcmSocketClient();
      startRecord(sound_card);
	  return NULL;
}

/*

void playProvTrack()
{

	//startHid2(g_sessionId);
//	
	//testAccessoryHidReport2(g_sessionId);
	playProvTracAccessoryHidReport(g_sessionId);
	playPauseAccessoryHidReport(g_sessionId);
   // stopHid(g_sessionId);
	
	
}
*/


void iAP2StopNowPlayingUpdates(int session)
{	
    if(g_iap2handle==NULL)
		return;
	isStartNowPlaying = false;

	 int messageLen = 0;
	 int para_len = 0;
	 int len;
	// uint32_t payloadLen = messageLen;
	//usleep(10000);
	 uint8_t payload[6];
	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB
	payload[2]= 0x00;// Message Length MSB
	payload[3]= 0x06;// Message Length LSB
	payload[4]= 0x50;// message id MSB
	payload[5]= 0x02;// message id LSB
	//iAP2LinkRunLoop_t* ttt = g_iap2handle->runloop;
	//iAP2Link_t* ttt2 = g_iap2handle->runloop->link;

    iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							payload,
							6,
							session,
							NULL,
							NULL);
	// free(challengReciveBuf);


}

void iAP2StartMediaLibraryItems(int session,uint8_t * itemsPids, int pidsSize,int itemStartIndex,
											uint8_t *mediaLibraryUid,int uidSize)
{
	if(g_iap2handle==NULL)
		return;
	
LOGI("iAP2StartMediaLibraryItems\n");
	 int messageLen = 0;
	 int para_len = 0;
	 int len;
	 int dataBaseSize = 6;
	// uint32_t payloadLen = messageLen;
	//usleep(10000);
	 para_len += dataBaseSize;
	 uint8_t payload[4+pidsSize+4+uidSize+6 + 8+1];

	 memset(payload,0,sizeof(payload));
		
	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB


	payload[4]= 0x4c;// message id MSB
	payload[5]= 0x07;// message id LSB

	payload[para_len+0]= (uint8_t)((((pidsSize+4) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[para_len+1]= (uint8_t)(((pidsSize+4)) & 0x00ff);// Message Length LSB
	payload[para_len+2]= 0x00;
	payload[para_len+3]= 0x00;
	memcpy(&payload[para_len+4],itemsPids,pidsSize);
	para_len = para_len+4+pidsSize;

	payload[para_len+0]= 0x00;
	payload[para_len+1]= 8;
	payload[para_len+2]= 0x00;
	payload[para_len+3]= 0x01;
	payload[para_len+4]= (uint8_t)(((itemStartIndex & 0xff000000)>> 24)& 0x000000ff);// Message Length MSB
	payload[para_len+5]= (uint8_t)(((itemStartIndex & 0x00ff0000)>> 16)& 0x000000ff);// Message Length MSB
	payload[para_len+6]= (uint8_t)(((itemStartIndex & 0x0000ff00)>> 8)& 0x000000ff);// Message Length MSB
	payload[para_len+7]= (uint8_t)(itemStartIndex & 0x000000ff);

	para_len = para_len +8;

	if(mediaLibraryUid[uidSize-1]!=0x00)
	{
		payload[para_len+0]= (uint8_t)((((uidSize+5) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
		payload[para_len+1]= (uint8_t)(((uidSize+5)) & 0x00ff);// Message Length LSB
		payload[para_len+2]= 0x00;
		payload[para_len+3]= 0x02;
		memcpy(&payload[para_len+4],mediaLibraryUid,uidSize);
		payload[para_len+4+uidSize] = 0x00;
		para_len = para_len+5+uidSize;
	}
	else
	{
		payload[para_len+0]= (uint8_t)((((uidSize+4) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
		payload[para_len+1]= (uint8_t)(((uidSize+4)) & 0x00ff);// Message Length LSB
		payload[para_len+2]= 0x00;
		payload[para_len+3]= 0x02;
		memcpy(&payload[para_len+4],mediaLibraryUid,uidSize);
		para_len = para_len+4+uidSize;
	}

	
	payload[2]= (uint8_t)(((para_len & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(para_len & 0x00ff);// Message Length LSB

	// memcpy(challengReciveBuf+6,MediaItemProperties,sizeof(MediaItemProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties),MediaPlayListProperties,sizeof(MediaPlayListProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties)+sizeof(MediaPlayListProperties),
				   //	MediaPlayListTransferInfoRequest,sizeof(MediaPlayListTransferInfoRequest));
	 iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							payload,
							para_len,
							session,
							NULL,
							NULL);
	


}


void iAP2PlayMediaLibrarySpecial(int session,int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,bool isPlayAll,
		int startMediaItemPidSize,uint8_t *bufMediaItemPid)

{
	if(g_iap2handle==NULL)
		return;
	
//LOGI("iAP2StartMediaLibraryItems\n");
	 int messageLen = 0;
	 int para_len = 0;
	 int len;
	 int dataBaseSize = 6;
	// uint32_t payloadLen = messageLen;
	//usleep(10000);
	 para_len += dataBaseSize;
	 uint8_t payload[1024];

	 memset(payload,0,sizeof(payload));
		
	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB


	payload[4]= 0x4c;// message id MSB
	payload[5]= 0x09;// message id LSB

  if(bufMediaLibraryUId[mediaLibraryUidSize-1]!=0)
  	{
  	  
	  payload[para_len+0]= (uint8_t)((((mediaLibraryUidSize+5) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	  payload[para_len+1]= (uint8_t)(((mediaLibraryUidSize+5)) & 0x00ff);// Message Length LSB
	  payload[para_len+2]= 0x00;
	  payload[para_len+3]= 0x00;
	  payload[para_len+4+mediaLibraryUidSize] = 0x00;
	  memcpy(&payload[para_len+4],bufMediaLibraryUId,mediaLibraryUidSize);
	  para_len = para_len+5+mediaLibraryUidSize;
  	}
  	else
  		{
	payload[para_len+0]= (uint8_t)((((mediaLibraryUidSize+4) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[para_len+1]= (uint8_t)(((mediaLibraryUidSize+4)) & 0x00ff);// Message Length LSB
	payload[para_len+2]= 0x00;
	payload[para_len+3]= 0x00;
	memcpy(&payload[para_len+4],bufMediaLibraryUId,mediaLibraryUidSize);
	para_len = para_len+4+mediaLibraryUidSize;
  		}

	payload[para_len+0]= 0x00;
	payload[para_len+1]= 4;
	payload[para_len+2]= 0x00;
	payload[para_len+3]= 0x01;
	//payload[para_len+4] = type;
	//payload[para_len+4]= (uint8_t)(((itemStartIndex & 0xff000000)>> 24)& 0x000000ff);// Message Length MSB
	//payload[para_len+5]= (uint8_t)(((itemStartIndex & 0x00ff0000)>> 16)& 0x000000ff);// Message Length MSB
	//payload[para_len+6]= (uint8_t)(((itemStartIndex & 0x0000ff00)>> 8)& 0x000000ff);// Message Length MSB
	//payload[para_len+7]= (uint8_t)(itemStartIndex & 0x000000ff);
	para_len = para_len +4;

	



    if(startMediaItemPidSize > 0)
    {
		payload[para_len+0]= (uint8_t)((((startMediaItemPidSize+4) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
		payload[para_len+1]= (uint8_t)(((startMediaItemPidSize+4)) & 0x00ff);// Message Length LSB
		payload[para_len+2]= 0x00;
		payload[para_len+3]= 0x02;
		memcpy(&payload[para_len+4],bufMediaItemPid,startMediaItemPidSize);
		
		para_len = para_len+5+startMediaItemPidSize;
	}
	
	payload[2]= (uint8_t)(((para_len & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(para_len & 0x00ff);// Message Length LSB
	

	// memcpy(challengReciveBuf+6,MediaItemProperties,sizeof(MediaItemProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties),MediaPlayListProperties,sizeof(MediaPlayListProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties)+sizeof(MediaPlayListProperties),
				   //	MediaPlayListTransferInfoRequest,sizeof(MediaPlayListTransferInfoRequest));
	 iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							payload,
							para_len,
							session,
							NULL,
							NULL);
	


}

void iAP2PlayMediaLibraryCollection(int session,int pid_size,uint8_t * bufPid,int type,int startIndex,int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,
		int startMediaItemPidSize,uint8_t *bufMediaItemPid)
	{
	if(g_iap2handle==NULL)
		return;
	
//LOGI("iAP2StartMediaLibraryItems\n");
	 int messageLen = 0;
	 int para_len = 0;
	 int len;
	 int dataBaseSize = 6;
	// uint32_t payloadLen = messageLen;
	//usleep(10000);
	 para_len += dataBaseSize;
	 uint8_t payload[1024];

	 memset(payload,0,sizeof(payload));
		
	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB


	payload[4]= 0x4c;// message id MSB
	payload[5]= 0x08;// message id LSB

	payload[para_len+0]= (uint8_t)((((pid_size+4) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[para_len+1]= (uint8_t)(((pid_size+4)) & 0x00ff);// Message Length LSB
	payload[para_len+2]= 0x00;
	payload[para_len+3]= 0x00;
	memcpy(&payload[para_len+4],bufPid,pid_size);
	para_len = para_len+4+pid_size;

	payload[para_len+0]= 0x00;
	payload[para_len+1]= 5;
	payload[para_len+2]= 0x00;
	payload[para_len+3]= 0x01;
	payload[para_len+4] = type;
	//payload[para_len+4]= (uint8_t)(((itemStartIndex & 0xff000000)>> 24)& 0x000000ff);// Message Length MSB
	//payload[para_len+5]= (uint8_t)(((itemStartIndex & 0x00ff0000)>> 16)& 0x000000ff);// Message Length MSB
	//payload[para_len+6]= (uint8_t)(((itemStartIndex & 0x0000ff00)>> 8)& 0x000000ff);// Message Length MSB
	//payload[para_len+7]= (uint8_t)(itemStartIndex & 0x000000ff);
	para_len = para_len +5;

	
	payload[para_len+0]= 0x00;
	payload[para_len+1]= 8;
	payload[para_len+2]= 0x00;
	payload[para_len+3]= 0x02;
	
	payload[para_len+4]= (uint8_t)(((startIndex & 0xff000000)>> 24)& 0x000000ff);// Message Length MSB
	payload[para_len+5]= (uint8_t)(((startIndex & 0x00ff0000)>> 16)& 0x000000ff);// Message Length MSB
	payload[para_len+6]= (uint8_t)(((startIndex & 0x0000ff00)>> 8)& 0x000000ff);// Message Length MSB
	payload[para_len+7]= (uint8_t)(startIndex & 0x000000ff);

	para_len = para_len +8;

	if(bufMediaLibraryUId[mediaLibraryUidSize-1]!=0x00)
	{
		payload[para_len+0]= (uint8_t)((((mediaLibraryUidSize+5) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
		payload[para_len+1]= (uint8_t)(((mediaLibraryUidSize+5)) & 0x00ff);// Message Length LSB
		payload[para_len+2]= 0x00;
		payload[para_len+3]= 0x03;
		memcpy(&payload[para_len+4],bufMediaLibraryUId,mediaLibraryUidSize);
		payload[para_len+4+mediaLibraryUidSize] = 0x00;
		para_len = para_len+5+mediaLibraryUidSize;
	}
	else
	{
		payload[para_len+0]= (uint8_t)((((mediaLibraryUidSize+4) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
		payload[para_len+1]= (uint8_t)(((mediaLibraryUidSize+4)) & 0x00ff);// Message Length LSB
		payload[para_len+2]= 0x00;
		payload[para_len+3]= 0x03;
		memcpy(&payload[para_len+4],bufMediaLibraryUId,mediaLibraryUidSize);
		para_len = para_len+4+mediaLibraryUidSize;
	}

    if(startMediaItemPidSize > 0)
    {
		payload[para_len+0]= (uint8_t)((((startMediaItemPidSize+4) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
		payload[para_len+1]= (uint8_t)(((startMediaItemPidSize+4)) & 0x00ff);// Message Length LSB
		payload[para_len+2]= 0x00;
		payload[para_len+3]= 0x04;
		memcpy(&payload[para_len+4],bufMediaItemPid,startMediaItemPidSize);
		para_len = para_len+4+startMediaItemPidSize;
	}
	
	payload[2]= (uint8_t)(((para_len & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(para_len & 0x00ff);// Message Length LSB
	

	// memcpy(challengReciveBuf+6,MediaItemProperties,sizeof(MediaItemProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties),MediaPlayListProperties,sizeof(MediaPlayListProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties)+sizeof(MediaPlayListProperties),
				   //	MediaPlayListTransferInfoRequest,sizeof(MediaPlayListTransferInfoRequest));
	 iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							payload,
							para_len,
							session,
							NULL,
							NULL);
	


}


void iAP2PlayMediaLibraryCurrentSelection(int session,uint8_t * mediaLibraryUid, int sizeUid)//规定每次只能停止一个
{
	if(g_iap2handle==NULL)
		return;

	 int messageLen = 0;
	 int para_len = 0;
	 int len;
	 int dataBaseSize = 6;
	// uint32_t payloadLen = messageLen;
	//usleep(10000);
	 para_len += dataBaseSize;
	 uint8_t payload[1024];
	// LOGI("iAP2StartMediaLibraryUpdates sizeUid=%d	sizeRevision=%d\n",sizeUid,sizeRevision);

	 if(sizeUid > 0)
	 {
	
	   int uidLen = 4+sizeUid;
	   payload[para_len+0] = (uint8_t)((((uidLen+1) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	   payload[para_len+1] = (uint8_t)((uidLen+1) & 0x00ff);// Message Length LSB
	   payload[para_len+2] = 0x00;
	   payload[para_len+3] = 0x00;
	   memcpy(&payload[para_len+4],mediaLibraryUid,sizeUid);
		 
		LOGI("iAP2StartMediaLibraryUpdates mediaLibraryUid=%s \n",mediaLibraryUid);
	   para_len += uidLen;
	   payload[para_len]=0x00;
	   para_len++;
	 }


	 // messageLen = 6+sizeof(MediaItemProperties)+sizeof(MediaPlayListProperties)+ sizeof(MediaLibraryUpdateProgress)+
		//(MediaLibraryIsHidingRemoteItems) + sizeof(PlayAllSongsCapable)+ sizeof(MediaPlayListTransferInfoRequest);
		  
		
	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB
	payload[2]= (uint8_t)(((para_len & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(para_len & 0x00ff);// Message Length LSB



	payload[4]= 0x4c;// message id MSB
	payload[5]= 0x06;// message id LSB

	 uint8_t * challengReciveBuf = (uint8_t*)malloc(para_len);

	 memcpy(challengReciveBuf,payload,para_len);
	// memcpy(challengReciveBuf+6,MediaItemProperties,sizeof(MediaItemProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties),MediaPlayListProperties,sizeof(MediaPlayListProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties)+sizeof(MediaPlayListProperties),
				   //	MediaPlayListTransferInfoRequest,sizeof(MediaPlayListTransferInfoRequest));
	 iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							challengReciveBuf,
							para_len,
							session,
							NULL,
							NULL);
	 free(challengReciveBuf);


}

void iAP2StopMediaLibraryUpdates(int session,uint8_t * mediaLibraryUid, int sizeUid)//规定每次只能停止一个
{
	if(g_iap2handle==NULL)
		return;

	 int messageLen = 0;
	 int para_len = 0;
	 int len;
	 int dataBaseSize = 6;
	// uint32_t payloadLen = messageLen;
	//usleep(10000);
	 para_len += dataBaseSize;
	 uint8_t payload[1024];
	// LOGI("iAP2StartMediaLibraryUpdates sizeUid=%d	sizeRevision=%d\n",sizeUid,sizeRevision);

	 if(sizeUid > 0)
	 {
	
	   int uidLen = 4+sizeUid;
	   payload[para_len+0] = (uint8_t)((((uidLen+1) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	   payload[para_len+1] = (uint8_t)((uidLen+1) & 0x00ff);// Message Length LSB
	   payload[para_len+2] = 0x00;
	   payload[para_len+3] = 0x00;
	   memcpy(&payload[para_len+4],mediaLibraryUid,sizeUid);
		 
		LOGI("iAP2StartMediaLibraryUpdates mediaLibraryUid=%s \n",mediaLibraryUid);
	   para_len += uidLen;
	   payload[para_len]=0x00;
	   para_len++;
	 }


	 // messageLen = 6+sizeof(MediaItemProperties)+sizeof(MediaPlayListProperties)+ sizeof(MediaLibraryUpdateProgress)+
		//(MediaLibraryIsHidingRemoteItems) + sizeof(PlayAllSongsCapable)+ sizeof(MediaPlayListTransferInfoRequest);
		  
		
	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB
	payload[2]= (uint8_t)(((para_len & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(para_len & 0x00ff);// Message Length LSB



	payload[4]= 0x4c;// message id MSB
	payload[5]= 0x05;// message id LSB

	 uint8_t * challengReciveBuf = (uint8_t*)malloc(para_len);

	 memcpy(challengReciveBuf,payload,para_len);
	// memcpy(challengReciveBuf+6,MediaItemProperties,sizeof(MediaItemProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties),MediaPlayListProperties,sizeof(MediaPlayListProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties)+sizeof(MediaPlayListProperties),
				   //	MediaPlayListTransferInfoRequest,sizeof(MediaPlayListTransferInfoRequest));
	 iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							challengReciveBuf,
							para_len,
							session,
							NULL,
							NULL);
	 free(challengReciveBuf);


}

void iAP2StartMediaLibraryUpdates(int session,uint8_t * mediaLibraryUid, int sizeUid,
											uint8_t *lastKnownMediaLibraryRevision,int sizeRevision)
{
	if(g_iap2handle==NULL)
		return;

	 int messageLen = 0;
	 int para_len = 0;
	 int len;
	 int dataBaseSize = 6;
	// uint32_t payloadLen = messageLen;
	//usleep(10000);
	 para_len += dataBaseSize;
	 uint8_t payload[1024];
	 LOGI("iAP2StartMediaLibraryUpdates sizeUid=%d  sizeRevision=%d\n",sizeUid,sizeRevision);
	 if(sizeUid > 0)
	 {
	
	   int uidLen = 4+sizeUid;
	   payload[para_len+0] = (uint8_t)((((uidLen+1) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	   payload[para_len+1] = (uint8_t)((uidLen+1) & 0x00ff);// Message Length LSB
	   payload[para_len+2] = 0x00;
	   payload[para_len+3] = 0x00;
	   memcpy(&payload[para_len+4],mediaLibraryUid,sizeUid);
	     
	    LOGI("iAP2StartMediaLibraryUpdates mediaLibraryUid=%s \n",mediaLibraryUid);
	   para_len += uidLen;
	   payload[para_len]=0x00;
	   para_len++;
	 }

	  if(sizeRevision > 0)
	 {
	
	   int uidLen = 4+sizeRevision;
	   payload[para_len+0] = (uint8_t)((((uidLen+1) & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	   payload[para_len+1] = (uint8_t)((uidLen+1) & 0x00ff);// Message Length LSB
	   payload[para_len+2] = 0x00;
	   payload[para_len+3] = 0x01;
	   memcpy(&payload[para_len+4],lastKnownMediaLibraryRevision,sizeRevision);
	   	    LOGI("iAP2StartMediaLibraryUpdates lastKnownMediaLibraryRevision=%s \n",lastKnownMediaLibraryRevision);
	   para_len += uidLen;
	     payload[para_len]=0x00;
	   para_len++;
	 }
	 	
	 uint8_t MediaItemProperties[]={0x00,0x5c,0x00,0x02,
	 	                       0x00,0x04,0x00,0,
	 	                       0x00,0x04,0x00,1,
	 	                       0x00,0x04,0x00,2,
	 	                       0x00,0x04,0x00,3,
	 	                       0x00,0x04,0x00,4,
	 	                       0x00,0x04,0x00,5,
	 	                       0x00,0x04,0x00,6,
	 	                       0x00,0x04,0x00,7,
	 	                       0x00,0x04,0x00,8,
	 	                       0x00,0x04,0x00,9,
	 	                       0x00,0x04,0x00,10,
	 	                       0x00,0x04,0x00,11,
	 	                       0x00,0x04,0x00,12,
	 	                       0x00,0x04,0x00,13,
	 	                       0x00,0x04,0x00,14,
	 	                       0x00,0x04,0x00,15,
	 	                       0x00,0x04,0x00,16,
	 	                       0x00,0x04,0x00,17,
	 	                       0x00,0x04,0x00,18,
	 	                       0x00,0x04,0x00,19,
	 	                       0x00,0x04,0x00,25,
	 	                       0x00,0x04,0x00,27};

	  memcpy(&payload[para_len],MediaItemProperties,sizeof(MediaItemProperties));
	   para_len += sizeof(MediaItemProperties);

	 
	 uint8_t MediaPlayListProperties[]={0x00,36,0x00,0x03,
								   0x00,0x04,0x00,0,
								   0x00,0x04,0x00,1,
								   0x00,0x04,0x00,2,
								   0x00,0x04,0x00,3,
								   0x00,0x04,0x00,4,
								   0x00,0x04,0x00,5,
								   0x00,0x04,0x00,6,
								   0x00,0x04,0x00,7};
	   memcpy(&payload[para_len],MediaPlayListProperties,sizeof(MediaPlayListProperties));
	   para_len += sizeof(MediaPlayListProperties);

	  uint8_t MediaLibraryUpdateProgress[]={0x00,4,0x00,0x04};
	   memcpy(&payload[para_len],MediaLibraryUpdateProgress,sizeof(MediaLibraryUpdateProgress));
	   para_len += sizeof(MediaLibraryUpdateProgress);
	   
	  uint8_t MediaLibraryIsHidingRemoteItems[]={0x00,4,0x00,0x05};
	  memcpy(&payload[para_len],MediaLibraryIsHidingRemoteItems,sizeof(MediaLibraryIsHidingRemoteItems));
		   para_len += sizeof(MediaLibraryIsHidingRemoteItems);

	  
	  uint8_t PlayAllSongsCapable[]={0x00,4,0x00,0x06};
	    memcpy(&payload[para_len],PlayAllSongsCapable,sizeof(PlayAllSongsCapable));
		   para_len += sizeof(PlayAllSongsCapable);

	 uint8_t MediaPlayListTransferInfoRequest[]={0x00,0x20,0x00,0x07,
								   0x00,0x04,0x00,0,
								   0x00,0x04,0x00,1,
								   0x00,0x04,0x00,6,
								   0x00,0x04,0x00,12,
								   0x00,0x04,0x00,14,
								   0x00,0x04,0x00,16,
								   0x00,0x04,0x00,18};
	    memcpy(&payload[para_len],MediaPlayListTransferInfoRequest,sizeof(MediaPlayListTransferInfoRequest));
		   para_len += sizeof(MediaPlayListTransferInfoRequest);

	 // messageLen = 6+sizeof(MediaItemProperties)+sizeof(MediaPlayListProperties)+ sizeof(MediaLibraryUpdateProgress)+
	  	//(MediaLibraryIsHidingRemoteItems) + sizeof(PlayAllSongsCapable)+ sizeof(MediaPlayListTransferInfoRequest);
		  
	  	
	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB
	payload[2]= (uint8_t)(((para_len & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(para_len & 0x00ff);// Message Length LSB



	payload[4]= 0x4c;// message id MSB
	payload[5]= 0x03;// message id LSB

	 uint8_t * challengReciveBuf = (uint8_t*)malloc(para_len);

	 memcpy(challengReciveBuf,payload,para_len);
	// memcpy(challengReciveBuf+6,MediaItemProperties,sizeof(MediaItemProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties),MediaPlayListProperties,sizeof(MediaPlayListProperties));
	// memcpy(challengReciveBuf+6+sizeof(MediaItemProperties)+sizeof(MediaPlayListProperties),
	  	           //   MediaPlayListTransferInfoRequest,sizeof(MediaPlayListTransferInfoRequest));
	 iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							challengReciveBuf,
							para_len,
							session,
							NULL,
							NULL);
	 free(challengReciveBuf);


}

void iAP2SetPQListContentTransferStartIndex(int index)
{		 
  // LOGI("xx %s %s %d position=%d\n",__FILE__,__FUNCTION__,__LINE__,position);
  // int positionMsSec = index;
   int messageLen = 14;
   uint8_t payload[14];
   payload[0]= 0x40;//start of message MSB
   payload[1]= 0x40;// start of message LSB
   payload[2]= 0x00;
   payload[3]= 14;


   payload[4]= 0x50;// message id MSB
   payload[5]= 0x03;// message id LSB

   payload[6]= 0x00;// message id MSB
   payload[7]= 0x08;// message id LSB
   
   payload[8]= 0x00;// message id MSB
   payload[9]= 0x02;// message id LSB

  
   payload[10]= (uint8_t)(index>>24);// message id MSB
   payload[11]= (uint8_t)(index>>16);// message id LSB
   payload[12]= (uint8_t)(index>>8);// message id MSB
   payload[13]= (index&0x000000ff);// message id LSB


iAP2LinkQueueSendData (g_iap2handle->runloop->link,
						payload,
						14,
						g_sessionId,
						NULL,
						NULL);



}
void iAP2StartPlaybackQueueIndex(int index)
{		 
	  // LOGI("xx %s %s %d position=%d\n",__FILE__,__FUNCTION__,__LINE__,position);
	  // int positionMsSec = position;
	   int messageLen = 14;
	   uint8_t payload[14];
	   payload[0]= 0x40;//start of message MSB
	   payload[1]= 0x40;// start of message LSB
	   payload[2]= 0x00;
	   payload[3]= 14;
   
   
	   payload[4]= 0x50;// message id MSB
	   payload[5]= 0x03;// message id LSB

	   payload[6]= 0x00;// message id MSB
	   payload[7]= 0x08;// message id LSB
	   
	   payload[8]= 0x00;// message id MSB
	   payload[9]= 0x01;// message id LSB

	  
	   payload[10]= (uint8_t)(index>>24);// message id MSB
	   payload[11]= (uint8_t)(index>>16);// message id LSB
	   payload[12]= (uint8_t)(index>>8);// message id MSB
	   payload[13]= (index&0x000000ff);// message id LSB


	iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							payload,
							14,
							g_sessionId,
							NULL,
							NULL);



}




void iAP2StartPlayPosition(int position)
{        
      // LOGI("xx %s %s %d position=%d\n",__FILE__,__FUNCTION__,__LINE__,position);
       int positionMsSec = position;
       int messageLen = 14;
       uint8_t payload[14];
       payload[0]= 0x40;//start of message MSB
	   payload[1]= 0x40;// start of message LSB
	   payload[2]= 0x00;
	   payload[3]= 14;
   
   
	   payload[4]= 0x50;// message id MSB
	   payload[5]= 0x03;// message id LSB

	   payload[6]= 0x00;// message id MSB
	   payload[7]= 0x08;// message id LSB
	   
	   payload[8]= 0x00;// message id MSB
	   payload[9]= 0x00;// message id LSB

	  
	   payload[10]= (uint8_t)(positionMsSec>>24);// message id MSB
	   payload[11]= (uint8_t)(positionMsSec>>16);// message id LSB
	   payload[12]= (uint8_t)(positionMsSec>>8);// message id MSB
	   payload[13]= (positionMsSec&0x000000ff);// message id LSB


    iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							payload,
							14,
							g_sessionId,
							NULL,
							NULL);



}

void iAP2StartNowPlayingUpdates(int session)
{
	if(g_iap2handle==NULL)
		return;

	 int messageLen = 0;
	 int para_len = 0;
	 int len;
	 ALOGD("iAP2StartNowPlayingUpdates");
	// uint32_t payloadLen = messageLen;
	//usleep(10000);
	 uint8_t payload[1024];
	 uint8_t MediaItemAttribute[]={0x00,0x00,0x00,0x00,
	 	                            0x00,0x04,0x00,0,
	 	                            0x00,0x04,0x00,1,
	 	                            0x00,0x04,0x00,4,
	 	                            0x00,0x04,0x00,6,
	 	                            0x00,0x04,0x00,7,
	 	                            0x00,0x04,0x00,8,
	 	                            0x00,0x04,0x00,9,
	 	                            0x00,0x04,0x00,10,
	 	                            0x00,0x04,0x00,12,
	 	                            0x00,0x04,0x00,16,
	 	                            0x00,0x04,0x00,18,
	 	                            0x00,0x04,0x00,21,
	 	                            0x00,0x04,0x00,22,
	 	                            0x00,0x04,0x00,23,
	 	                            0x00,0x04,0x00,24,
	 	                            0x00,0x04,0x00,26/*,
	 	                            0x00,0x04,0x00,27*/};
	 MediaItemAttribute[1] = sizeof(MediaItemAttribute);

	 
	 uint8_t PlaybackAttribute[]={0x00,0x00,0x00,0x01,
								   0x00,0x04,0x00,0,
								   0x00,0x04,0x00,1,
								   0x00,0x04,0x00,2,
								   0x00,0x04,0x00,3,
								   0x00,0x04,0x00,4,
								   0x00,0x04,0x00,5,
								   0x00,0x04,0x00,6,
								   0x00,0x04,0x00,7,
								   0x00,0x04,0x00,8,
								   0x00,0x04,0x00,9,
								   0x00,0x04,0x00,10,
								   0x00,0x04,0x00,11,
								   0x00,0x04,0x00,12,
								   0x00,0x04,0x00,13,
								   0x00,0x04,0x00,14,
								   0x00,0x04,0x00,15,
								   0x00,0x04,0x00,16/*,								   
								   0x00,0x08,0x00,17,0x00,0x00,0x00,0x00*/};
	 PlaybackAttribute[1] = sizeof(PlaybackAttribute);

	 uint8_t PlaybackQueue[]={0x00,0x00,0x00,0x02,
							   0x00,0x04,0x00,0,
							   0x00,0x04,0x00,1,
							   0x00,0x04,0x00,6,
							   0x00,0x04,0x00,12,
							   0x00,0x04,0x00,14,
							   0x00,0x04,0x00,16,
							   0x00,0x04,0x00,18};
	  PlaybackQueue[1]=sizeof(PlaybackQueue);

	  messageLen = 6+sizeof(MediaItemAttribute)+sizeof(PlaybackAttribute)+sizeof(PlaybackQueue);

	payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB
	payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB



	payload[4]= 0x50;// message id MSB
	payload[5]= 0x00;// message id LSB


	 uint8_t * challengReciveBuf = (uint8_t*)malloc(messageLen);

	 memcpy(challengReciveBuf,payload,6);
	 memcpy(challengReciveBuf+6,MediaItemAttribute,sizeof(MediaItemAttribute));
	 memcpy(challengReciveBuf+6+sizeof(MediaItemAttribute),PlaybackAttribute,sizeof(PlaybackAttribute));
	  memcpy(challengReciveBuf+6+sizeof(MediaItemAttribute)+sizeof(PlaybackAttribute),PlaybackQueue,sizeof(PlaybackQueue));

	 iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							challengReciveBuf,
							messageLen,
							session,
							NULL,
							NULL);
	 free(challengReciveBuf);
	 isStartNowPlaying = true;


}



void iAP2SessCtrlDispatchMsg(struct iAP2Link_st* link, 
							uint8_t* data, 
                            uint32_t dataLen, 
                            uint8_t session)
{

  // iap2_sessCtrlDispatch_object 


  bool ret = false;
  unsigned short messageId = iAp2GetMessageId(data,dataLen);
  int infoSize = 0;
  uint32_t sampleRate;
  int channels;
  //LOGI("iAP2SessCtrlDispatchMsg messageId=0x%04x\n",messageId);
  switch(messageId)
  	{
  	   case message_RequestAuthenticationCertificate:
	   	accessoryAuthenticationCertificate(session);
	   	break;
	  
		case message_RequestAuthenticationChallengResponse:
			accessoryAuthenticationResponse(dataLen,data,session);
			break;
		case message_RequestAuthenticationSuceessFail:
			break;
		case message_RequestAuthenticationSuceessfull:
			break;
		case message_StartIdentification:
			iAP2StartIndentificationInfo(session);
			tryTest++;
			break;
		case message_IdentificationRejected:
			if(tryTest<2)
				{
			tryTest++;
			iAP2StartIndentificationInfo(session);
				}
			break;
		case message_USBDeviceModeAudioInformation:
			 g_sessionId = session;
			USBDeviceModeAudioInformation(data,dataLen,session);
			 startHid2(session);
			
		
			  sampleRate = audio_sampleRates[g_sample_rate];
		 // size_t frameCount = 0;
		   channels = 2;
		  if(g_startAudioThread == 0)
		  	{
               g_startAudioThread = 1;
              if(g_captureHandle==NULL)
              	{
			    g_captureHandle = new IapAudioHw(1);
				g_captureHandle->setSampleRate(sampleRate);
			    g_captureHandle->setChannelCount(channels);
			    g_captureHandle->setFrameSize(channels*2);
				  

			//stopHid(session);

			 pthread_t read_thead;
			 pthread_create(&read_thead, NULL, startAudioThread,
							  NULL);
              	}
		  	}

		  mIpodService->notifyOnIpodStartReady();

			//StartMediaLibraryInformation(session);
			break;
		
		case message_IdentificationAccepted:
              //sleep(5);
			
			// pthread_t read_thead;
			//int  ret = pthread_create(&read_thead, NULL, threadTest,
					//		  NULL);
			//startHid(session);
			// stopUSBDeviceModeAudio(session);
			 // sleep(5);
			  startUSBDeviceModeAudio(session);
			
			// testAccessoryHidReport(session);
			//usleep(10000);
			
			//usleep(100000);
			//sleep(5);

			//startRecord();
			//sleep(5);

			//testAccessoryHidReport2(session);
			//sleep(5);

			//stopHid(session);
			
			break;
		case message_MediaLibraryInformation:
			
			LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
			//MediaLibraryInformationProcess(data,dataLen,session);
			 mIpodService->notifyOnMediaLibraryInformation(data,dataLen);
			
			/* infoSize = getMediaLibraryInformationSize();
			  LOGI("%s line:%d infoSize=%d\n",__FUNCTION__,__LINE__,infoSize);
			if(infoSize > 0) //for test
				{
				 // for(int k=0;k<infoSize;k++)
				 // {
				  LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
				  //	MediaLibraryInformation *pInfo = getMediaLibraryInformation(k+1);
				  MediaLibraryInformation *pInfo = getMediaLibraryInformation(1);
					LOGI("%s line:%d  \n",__FUNCTION__,__LINE__);
			        mIpodService->StartMediaLibraryUpdates(pInfo->uniqueueIdentifier,pInfo->uidSize,NULL,0);
					LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
				  //}
				}
			 sleep(1);
	    mIpodService->play();
		*/
			break;
	    case message_NowPlayingUpdates:
		//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			NowPlayingUpdates(data,dataLen,session);			
			//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			break;
		case message_MediaLibraryUpdate:
			//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			//MediaLibraryUpdate(data,dataLen,session);
			 mIpodService->notifyOnMediaLibraryUpdate(data,dataLen);
			//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			break;
		case message_DeviceUUIDUpdate:
			startPowerUpdates(session);
			break;
		case message_DeviceInformationUpdate:
			break;
		case message_DeviceLanguageUpdate:
			break;
        case message_deviceTimeUpdate:
			PowerSourceUpdates(session);
			break;
  	}
 

   


}

static int readI2C(int inFD,uint8_t	inRegister,uint8_t *inReadBuf, size_t inReadLen)
{
	uint64_t		deadline;
	int				tries;
	ssize_t			n;

   int err = -1;
   for( tries = 1; ; ++tries )
		{
			n= write( inFD, &inRegister, 1 );
			///LOGI("################# readI2C  write fail!!! error = %s\n",strerror(errno));
			err = map_global_value_errno( n == 1, n );
			
			if( !err ) break;

			
			usleep( kMFiAuthRetryDelayMics );
			if(tries == 400)
				return -1;
			
		}
		for( tries = 1; ; ++tries )
		{
			n = read( inFD, inReadBuf, inReadLen );
			
			err = map_global_value_errno( n == ( (ssize_t) inReadLen ), n );
			
			if( !err ) break;
			
			

			usleep( kMFiAuthRetryDelayMics );
			if(tries == 400)
				return -1;
			
		}
		
		return 0;
}


static int writeI2c( 
		int				inFD, 
		uint8_t			inRegister, 
		const uint8_t *	inWritePtr, 
		size_t			inWriteLen)
{
	int		err = -1;
	uint64_t		deadline;
	int				tries;
	ssize_t			n;
	uint8_t			buf[ 1 + inWriteLen ];
	size_t			len;
	
	
	
	
		// Gather the register and data so it can be done as a single write transaction.
		
		buf[ 0 ] = inRegister;
		memcpy( &buf[ 1 ], inWritePtr, inWriteLen );
		len = 1 + inWriteLen;
		for( tries = 1; ; ++tries )
		{
			n = write( inFD, buf, len );
			
				err = map_global_value_errno( n == ( (ssize_t) len ), n );
			if( !err ) break;

			//if( err == 0 ) break;
			usleep( kMFiAuthRetryDelayMics );
			if(tries == 400)
				return -1;
		}
	
	return( err );
}

 void iAP2SessCtrlCreateControlSession(uint8_t version,uint8_t type,uint8_t session)
 {

 	if(g_iap2handle==NULL)
		return;
 	   uint32_t payloadLen = 41;
        uint8_t payload[41];
		//uint8_t session = 0xAA06
       // payload [kiAP2FileTransferHdrIdxID]      = fileXfer->bufferID;
       // payload [kiAP2FileTransferHdrIdxControl] = pckType;

		payload[0]= 0x01;//start of message MSB
		payload[1]= 0x05;// start of message LSB
		payload[2]= 0x10;// Message Length MSB
		payload[3]= 0x00;// Message Length LSB
		payload[4]= 0x04;// message id MSB
		payload[5]= 0x0b;// message id LSB
		payload[6]= 0x00;// parameter length MSB
		payload[7]= 0x17;// parameter lenth LSB 35 =  31+4
		payload[8]= 0x03;// parameter id id MSB
		payload[9]= 0x03;// parameter id LSB
		payload[10]= type;// parameter id id MSB
		payload[11]= version;// parameter id LSB
		payload[12]= session;
        iAP2LinkQueueSendData (g_iap2handle->runloop->link,
                               payload,
                               payloadLen,
                               session,
                               NULL,
                               NULL);
 }

 

 int accessoryAuthenticationCertificate	 (uint8_t session)
{

	 if(g_iap2handle==NULL)
		 return -1;

	 int			 fd = -1;
	 int			 err = -1;
	 uint8_t		 buf[ 2 ];
	 uint8_t		 *certificateBuf;
	 int certificateLen;
	 
	 
	 fd = open( kMFiAuthDevicePath, O_RDWR);
	 err = ioctl(fd, I2C_TENBIT, 0); //not 10bit
	 err = ioctl( fd, I2C_SLAVE, kMFiAuthDeviceAddress );
	 if(err < 0)
	 	{
		 return err;
	 	}
	 err = readI2C(fd,kMFiAuthReg_DeviceCertificateSize,buf,2);
	 if(err < 0)
	 {
		 return err;
	 }
	certificateLen = ( buf[ 0 ] << 8 ) | buf[ 1 ];

	certificateBuf = (uint8_t *) malloc( certificateLen );
	memset(certificateBuf,0,certificateLen);

	err = readI2C(fd,kMFiAuthReg_DeviceCertificateData1,certificateBuf,certificateLen);
	 if(err < 0)
	 	{
	 	
	 	free(certificateBuf);
		 return err;
	 	}
	 close(fd);
	//  iAP2Packet_t* packet = iAP2PacketCreateEmptySendPacket();
	   unsigned short parameterLen = ( unsigned short)certificateLen+4;
		unsigned short messageLen = ( unsigned short)certificateLen+10;

		 uint32_t payloadLen = messageLen;
		 uint8_t payload[messageLen];
		payload[0]= 0x40;//start of message MSB
		 payload[1]= 0x40;// start of message LSB
		 payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
		 payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB
		  
		 payload[4]= 0xAA;// message id MSB
		 payload[5]= 0x01;// message id LSB
		 payload[6]= (uint8_t)(((parameterLen & 0xff00) >> 8)& 0xff);// parameter length MSB
		 payload[7]= (uint8_t)(parameterLen & 0x00ff);// parameter lenth LSB 35 =  31+4
		 payload[8]= 0x0;// parameter id id MSB
		 payload[9]= 0x0;// parameter id LSB
		 memcpy(&(payload[10]),certificateBuf,messageLen-10);
		 iAP2LinkQueueSendData (g_iap2handle->runloop->link,
								payload,
								payloadLen,
								session,
								NULL,
								NULL);

		 	free(certificateBuf);
	 return err;

}
 
int accessoryAuthenticationResponse	 (uint8_t dataLen,uint8_t *data,int session)
{

 int			 fd = -1;
 int			 err = -1;
 uint8_t		 buf[ 2 ];
 	if(g_iap2handle==NULL)
		return err;
 if(dataLen<10)
 	return -1;
 uint8_t * challengReciveBuf ;
 int challengLen = ((data[6]&0x00ff) <<8)|((data[7]&0x00ff));
 	if(challengLen > 4)
 		{
		challengReciveBuf = (uint8_t *)malloc(challengLen-4);
		memcpy(challengReciveBuf,&data[10],challengLen-4);
 		}
		else
			return -1;
		   chmod(kMFiAuthDevicePath, 0666);

		   fd = open( kMFiAuthDevicePath, O_RDWR);
		   err = ioctl(fd, I2C_TENBIT, 0); //not 10bit
		   err = ioctl( fd, I2C_SLAVE, kMFiAuthDeviceAddress );
   
			
		   if(err < 0)
		   	{
		   	
		   	free(challengReciveBuf);
			close(fd);
			   return err;
		   	}
			   buf[ 0 ] = (uint8_t)( ( (challengLen-4) >> 8 ) & 0xFF );
			   buf[ 1 ] = (uint8_t)(   (challengLen	-4)	  & 0xFF );
 err = writeI2c(fd,kMFiAuthReg_ChallengeSize,buf,2);
 	 if(err < 0)
 	 	{
 	 	close(fd);
 	 	free(challengReciveBuf);
			   return err;
 	 	}
	  err = writeI2c(fd,kMFiAuthReg_ChallengeData,challengReciveBuf,challengLen-4);
 	 if(err < 0)
 	 	{
 	 	close(fd);
 	 	free(challengReciveBuf);
			   return err;
 	 	}

  buf[ 0 ] = kMFiAuthControl_GenerateSignature;
			 
 err = writeI2c(fd,kMFiAuthReg_AuthControlStatus,&buf[0],1);
  if(err < 0)
  	{
  	close(fd);
  	free(challengReciveBuf);
			   return err;
  	}

	 	
  buf[ 0 ] = 0;
			 
 err = readI2C(fd,kMFiAuthReg_AuthControlStatus,&buf[0],1);
  if(err < 0)
  	{
  	close(fd);
  	free(challengReciveBuf);
			   return err;
  	}
     uint8_t staus = (uint8_t)(buf[0]>>4)& 0x0f;
	 if(kMFiAuthControl_GenerateSignature!=staus)
	 	{
	 	close(fd);
	 	free(challengReciveBuf);
	 	return -1;
	 	}

				buf[ 0 ] = 0;
				buf[ 1 ] = 0;
				  err = readI2C(fd,kMFiAuthReg_SignatureSize,buf,2);
					  if(err < 0)
						{
						close(fd);
						free(challengReciveBuf);
								return err;
						}

					 int  signatureLen = ( buf[ 0 ] << 8 ) | buf[ 1 ];

					buf[ 0 ] = 0;
					buf[ 1 ] = 0;
	  err = readI2C(fd,kMFiAuthReg_ChallengeSize,buf,2);
		  if(err < 0)
		  	{
		  	close(fd);
		  	free(challengReciveBuf);
					return err;
		  	}
		  if(buf[1]==0 && buf[0]==0)
		  	{
		  	close(fd);
		  	free(challengReciveBuf);
		  	return -1;
		  	}
			     challengLen = ( buf[ 0 ] << 8 ) | buf[ 1 ];
		  // challengLen = ((buf[1]&0x00ff) <<8)|((buf[0]&0x00ff));
		  	free(challengReciveBuf);
		  challengReciveBuf = (uint8_t *)malloc(signatureLen);
			err = readI2C(fd,kMFiAuthReg_SignatureData,challengReciveBuf,signatureLen);
							if(err < 0)
								{
								close(fd);
									free(challengReciveBuf);
									  return err;
								}


	   close(fd);
//  iAP2Packet_t* packet = iAP2PacketCreateEmptySendPacket();
   int parameterLen = signatureLen+4;
	int messageLen = signatureLen+10;

	 uint32_t payloadLen = messageLen;
	 uint8_t payload[messageLen];
	
    payload[0]= 0x40;//start of message MSB
	payload[1]= 0x40;// start of message LSB
	payload[2]= (uint8_t)(((messageLen & 0xff00)>> 8)& 0x00ff);// Message Length MSB
	payload[3]= (uint8_t)(messageLen & 0x00ff);// Message Length LSB
	payload[4]= 0xAA;// message id MSB
	payload[5]= 0x03;// message id LSB
	payload[6]= (uint8_t)(((parameterLen & 0xff00) >> 8)& 0xff);// parameter length MSB
	payload[7]= (uint8_t)(parameterLen & 0x00ff);// parameter lenth LSB 35 =  31+4
	payload[8]= 0x0;// parameter id id MSB
	payload[9]= 0x0;// parameter id LSB

	 memcpy(&(payload[10]),challengReciveBuf,messageLen-10);
	 iAP2LinkQueueSendData (g_iap2handle->runloop->link,
							payload,
							payloadLen,
							session,
							NULL,
							NULL);
	
	 
 free(challengReciveBuf);
 return err;

 
 
}	 

 int AccessoryAuthenticationDoChallengeData(int dataLen,uint8_t *data,int session)
 	{

		int				fd = -1;
	int 			err = -1;
    uint8_t			buf[ 2 ];
	int challengSize = 0;
	uint8_t* challengData = NULL;
	chmod(kMFiAuthDevicePath, 0666);
	fd = open( kMFiAuthDevicePath, O_RDWR );
     err = ioctl(fd, I2C_TENBIT, 0); //not 10bit
	err = ioctl( fd, I2C_SLAVE, kMFiAuthDeviceAddress );

	 buf[ 0 ] = (uint8_t)( ( dataLen >> 8 ) & 0xFF );
		 buf[ 1 ] = (uint8_t)(	 dataLen		& 0xFF );

	 err = writeI2c(fd,kMFiAuthReg_ChallengeSize,buf,2);
		 if(err < 0)
			 return err;

		  err = writeI2c(fd,kMFiAuthReg_ChallengeData,data,dataLen);
		 if(err < 0)
			 return err;
		
		buf[0]=kMFiAuthControl_GenerateSignature;
				buf[1]= 0;

	
	err = writeI2c(fd,kMFiAuthReg_AuthControlStatus,&(buf[0]),1);
		 if(err < 0)
			 return err;
		 buf[0]=0;
		 buf[1]= 0;
    
	err = readI2C( fd, kMFiAuthReg_AuthControlStatus, &(buf[0]), 1 );
	if(err < 0)
				return err;

	if(buf[0] == 1)//proc_result
		{
		    buf[0]=0;
		 buf[1]= 0;

		 err = readI2C( fd, kMFiAuthReg_SignatureSize, &(buf[0]), 1 );
		 if(err < 0)
				return err;
		 challengSize = ( buf[ 0 ] << 8 ) | buf[ 1 ];
		 challengData = (uint8_t*)malloc(challengSize);
		  err = readI2C( fd, kMFiAuthReg_SignatureData, challengData, challengSize );
		 if(err < 0)
		 	{
		 	free(challengData);
				return err;
		 	}

		 accessoryAuthenticationResponse(challengSize,challengData,session);
		 
		}
	free(challengData);
	
 	return 0;
	
 	}


 int AccessoryAuthenticationSerialNumber(uint8_t session)
{
  
   	int				fd = -1;
	int 			err = -1;
    uint8_t			buf[ 31 ];
	if(g_iap2handle==NULL)
		return err;
	chmod(kMFiAuthDevicePath, 0666);
	fd = open( kMFiAuthDevicePath, O_RDWR );
	 err = ioctl(fd, I2C_TENBIT, 0); //not 10bit
	err = ioctl( fd, I2C_SLAVE, kMFiAuthDeviceAddress );
	if(err < 0)
		return err;

	err = readI2C(fd,kMFiAuthReg_DeviceSerial,buf,31);
	if(err < 0)
		return err;

 //  iAP2Packet_t* packet = iAP2PacketCreateEmptySendPacket();

      close(fd);
        uint32_t payloadLen = 41;
        uint8_t payload[41];
		//uint8_t session = 0xAA06
       // payload [kiAP2FileTransferHdrIdxID]      = fileXfer->bufferID;
       // payload [kiAP2FileTransferHdrIdxControl] = pckType;

		payload[0]= 0x40;//start of message MSB
		payload[1]= 0x40;// start of message LSB
		payload[2]= 0x0;// Message Length MSB
		payload[3]= 0x29;// Message Length LSB
		payload[4]= 0xAA;// message id MSB
		payload[5]= 0x06;// message id LSB
		payload[6]= 0x0;// parameter length MSB
		payload[7]= 0x23;// parameter lenth LSB 35 =  31+4
		payload[8]= 0x0;// parameter id id MSB
		payload[9]= 0x0;// parameter id LSB
		memcpy(&(payload[10]),buf,31);
        iAP2LinkQueueSendData (g_iap2handle->runloop->link,
                               payload,
                               payloadLen,
                               session,
                               NULL,
                               NULL);
	
	return err;

	
	
}



#ifdef __cplusplus
}
#endif	// __cplusplus
