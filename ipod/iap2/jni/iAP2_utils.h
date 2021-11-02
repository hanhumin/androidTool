#ifndef IAP2_UTILS_H
#define IAP2_UTILS_H


#include <unistd.h>
#include <nativehelper/JNIHelp.h>

#include "android_runtime/AndroidRuntime.h"
#include "IpodService.h"

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;
typedef double d64;
typedef unsigned long long u64;

using namespace android;

#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus
//namespace IAP2JNIH{
enum MediaLibraryType {
   TYPE_LOCAL_DEVICE		   = 0,  
   TYPE_APPLE_MUSIC_RADIO		   = 2

							 
};




#define message_RequestAuthenticationCertificate 0xAA00
#define message_RequestAuthenticationChallengResponse 0xAA02
#define message_RequestAuthenticationSuceessFail 0xAA04
#define message_RequestAuthenticationSuceessfull 0xAA05
#define message_StartIdentification 0x1D00
#define message_IdentificationAccepted 0x1D02
#define message_USBDeviceModeAudioInformation 0xDA01
#define message_MediaLibraryInformation 0x4C01
#define message_MediaLibraryUpdate 0x4C04
#define message_DeviceUUIDUpdate 0x4e0c
#define message_DeviceInformationUpdate 0x4e09
#define message_DeviceLanguageUpdate 0x4e0a
#define message_deviceTimeUpdate 0x4e0b

#define message_NowPlayingUpdates 0x5001
#define message_IdentificationRejected 0x1D03

#define PLAY_MODE_STOP 0
#define PLAY_MODE_PLAY 1
#define PLAY_MODE_PAUSE 2


#define PLAY_NONE_HID_REPORT_ID 0x00
#define PLAY_PAUSE_HID_REPORT_ID 0x01
#define PLAY_HID_REPORT_ID 0x10
#define PAUSE_HID_REPORT_ID 0x08
#define NEXT_HID_REPORT_ID 0x02
#define PREVIOUS_HID_REPORT_ID 0x04
#define PLAY_RANDOM_HID_REPORT_ID 0x40
#define PLAY_REPEAT_HID_REPORT_ID 0x20









void setPlayMode(int mode);
int getPlayMode();

int accessoryAuthenticationCertificate	(uint8_t session);
int accessoryAuthenticationResponse	 (uint8_t dataLen,uint8_t *data,int session);
int get_record_mode();
void set_record_mode(int mode);

//void playPause();
//void playProvTrack();
//void playNextTrack();
void iAP2StopUSBDeviceModeAudio(uint8_t session);

void iAP2StopHid(int sessionId);
void iAP2StartMediaLibraryItems(int session,uint8_t * itemsPids, int pidsSize,int itemStartIndex,
											uint8_t *mediaLibraryUid,int uidSize);

void iAP2StartMediaLibraryUpdates(int session,uint8_t * mediaLibraryUid, int sizeUid,
											uint8_t *lastKnownMediaLibraryRevision,int sizeRevision);
void iAP2StopMediaLibraryUpdates(int session,uint8_t * mediaLibraryUid, int sizeUid);

void iAP2PlayMediaLibraryCurrentSelection(int session,uint8_t * mediaLibraryUid, int sizeUid);
void iAP2PlayMediaLibraryCollection(int session,int pid_size,uint8_t * bufPid,int type,int startIndex,int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,
		int startMediaItemPidSize,uint8_t *bufMediaItemPid);
void iAP2PlayMediaLibrarySpecial(int session,int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,bool isPlayAll,
		int startMediaItemPidSize,uint8_t *bufMediaItemPid);


void iAP2StartNowPlayingUpdates(int session);
void iAP2StartMediaLibraryInformation(int sessionId);
void iAP2StopMediaLibraryInformation(int sessionId);
void iAP2StopNowPlayingUpdates(int session);
void SetNowPlayingInformation(int ElapsTime,int PlayBackQueueIndex,int fileTransfer,int session);
void OnUSBDeviceModeAudioInformation(int sampleRate);
void OnMediaItemAlumTitle(u8 *data, u32 len);
void OnMediaItemAlumArtist(u8 *data, u32 len);
void OnMediaLibraryUpdateUID(u8 *data, u32 len);
void OnMediaLibraryUpdateRevision(uint8_t  *data, u32 len, uint8_t  *dataUid,u32 uidLen);
void OnMediaLibraryName(u8 *data, u32 len);
void OnMediaItemTile(u8 *data, u32 len);
void OnMediaItemArtist(u8 *data, u32 len);
void OnPcmDataReceive(u8 *data,u32 len);
void OnMediaLibraryInformation(u8 *data,u32 len);
void OnMediaLibraryUpdate(u8 *data,u32 len);
void OnNowPlayingMediaItemAttributes(uint8_t  *data, u32 dataLen);
void OnNowPlayingPlayBackAttributes(uint8_t  *data, u32 dataLen);


void OnPlayBackStatus(int status);
void OnPlayBackQueueIndex(int index);
void OnPlayBackQueueCount(int count);
void OnPlayBackQueueChapterIndex(int index);
void OnPlayBackShuffleMode(int mode);
void OnPlayBackRepeatMode(int mode);
void OnMediaLibraryUpdatePlaylistName(u8 *data, u32 len);
void OnPlayBackAppName(u8 *data, u32 len);
void OnPBMediaLibraryUID(u8 *data, u32 len);
void OnPBAppleMusicRadioAd(int status);
void OnPBAppleMusicRadioStationName(u8 *data, u32 len);
void OnMeidaLibraryUID(u8 *data,u32 len);
void OnMediaLibraryUpdatePlaylistID(u8 *data,u32 len);
void OnMediaLibraryUpdateParentPlaylistID(u8 *data,u32 len);
void OnPBAppleMusicRadioStationMediaPlaylistID(u8 *data,u32 len);
void OnSetElapsedTimeAvailable(int status);
void OnMediaUpdatePlaylistContentTransfer();
void OnMeidaNowPlayingArtworkStart();
void OnNowPlayingPBQContentsStart(uint8_t *data, u32 len);
void OnMediaItemNowPlayingPBQContentsFinish(uint8_t *data, u32 len);
void OnMediaItemNowPlayingPBQContentsStart(uint8_t *data, u32 len);
void OnMediaLibraryUpdatePlaylistContentsFinish( int fileName_len, const char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID);

void OnMediaLibraryUpdatePlaylistContentsStart(int fileName_len, const char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID);
void OnMediaItemListUpdatePlaylistContentsFinish(int fileName_len, const char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID);
void OnMediaItemListUpdatePlaylistContentsStart(int fileName_len, const char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID);
void OnNowPlayingPBQContentsFinish(uint8_t *data, u32 len);
void OnMeidaNowPlayingArtworkFinish();
void OnIPODStartReady();
void OnContainedMediaItemFsId(int id);
void OnMediaUpdatePlaylistIsRadioStation(int status);
void OnMediaUpdatePlaylistIsFolder(int status);
void OnMediaUpdatePlaylistIsGeniusMix(int status);
void OnPlayBackQueueListAvail(int status);
void OnPlayBackQueueListTransferID(int id);
void onPlayBackBundleID(u8 *data, u32 len);
void OnPlayBackQueueListContentTransfer();
void OnMediaItemPID(u8 *data,u32 len);
void OnMediaItemMediaType(int type);
void OnMediaLibraryType(int type);
void OnMediaItemRating(int rating);
void OnMediaPlayListDeleteUpdatePID(u8 *data,u32 len,uint8_t  *dataUid,u32 uidLen);
void OnMediaItemDeleteUpdatePID(u8 *data,u32 len,uint8_t  *dataUid,u32 uidLen);
void OnMediaItemAlbumPID(u8 *data,u32 len);
void OnMediaItemAlbumTrackNumber(int number);
void OnMediaItemAlbumTrackCount(int count);
void OnMediaItemAlbumDiscNumber(int number);
void OnMediaItemAlbumDiscCount(int count);
void OnMediaItemArtistPID(u8 *data,u32 len);
void OnMediaItemAlbumArtistPID(u8 *data,u32 len);
void OnMediaItemGenrePID(u8 *data,u32 len);
void OnMediaItemGenre(u8 *data,u32 len);
void OnMediaItemComposerPID(u8 *data,u32 len);
void OnMediaItemGenreComposer(u8 *data,u32 len);
void OnMediaItemIsPartOfCompilation(int status);
void OnPlayAllSongsCapable(int status,uint8_t  *dataUid, u32 uidLen);
void OnIsHidingRemoteItems(int status,uint8_t  *dataUid, u32 uidLen);
void OnMediaLibraryUpdateProgress(int progress,uint8_t  *dataUid, u32 uidLen);
void OnMediaLibraryReset(uint8_t  *dataUid, u32 uidLen);
void OnMediaItemIsLikeSupported(int status);
void OnMediaItemIsBeanSupported(int status);
void OnMediaItemIsLiked(int isLiked);
void OnMediaItemIsBanned(int IsBanned);
void OnMediaItemIsResidentOnDevice(int IsResident);
void OnMediaItemArtWorkFileTransferID(int id);
void OnMediaItemChapterCount(int count);


void OnDuration(int durantion);
void OnPlayBackSpeed(int speed);

void OnPlayBackPosition(int position);
void playAccessoryHidReport(int session);
void hid0AccessoryHidReport();
void playBackAccessoryHidReport(int session,int hidReport);//int hid report

void pauseAccessoryHidReport(int session);
void iAP2StartPlayPosition(int position);
void iAP2StartPlaybackQueueIndex(int index);
void iAP2SetPQListContentTransferStartIndex(int index);
void playPauseAccessoryHidReport(int session);
void repeatAccessoryHidReport(int session);
void randomAccessoryHidReport(int session);
void playPauseAccessoryHidReport2(int session);
void playNextTrackAccessoryHidReport(int session);
void playProvTracAccessoryHidReport(int session);
int readFileListTwo(char *basePath);
int readFileListOne(char *basePath);





extern jobject obj_OnPcmDataReceiver;
extern jobject obj_NowPlayingPlaybackQueueContents;
extern jobject obj_MediaItemNowPlayingPlaybackQueueContents;
extern jobject obj_MediaLibraryUpdatePlayListContents;
extern jobject obj_MediaItemListUpdatePlayListContents;

extern jobject obj_OnAudioSampleRate;
//extern jobject obj_OnPlayBackStatus;

//extern jobject obj_OnPlayBackPosition;
extern jobject obj_MediaLibraryInformation;
extern jobject obj_NowPlayingArtwork;
extern jobject obj_IpodStart;

extern jobject obj_MediaLibraryUpdate;
extern jobject obj_NowPlayingPlaybackAttributes;
extern jobject obj_NowPlayingMediaItemAttributes;
extern jobject obj_MediaLibraryUpdatePlayList ;
extern jclass  gClazzMediaLibraryInformation;
extern jclass  gClazzMediaItem;
extern jclass  gClazzMediaPlaylist;
//extern jobject obj_OnDuration;
//extern jobject obj_OnMediaItemTile;

extern IpodService *mIpodService;


//}


#ifdef __cplusplus
}
#endif	// __cplusplus

#endif
