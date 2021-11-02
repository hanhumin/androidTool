package com.autoipod;

import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Binder;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Process;
import android.provider.Settings;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.media.MediaBrowserCompat;
import android.support.v4.media.MediaBrowserServiceCompat;
import android.support.v4.media.session.MediaSessionCompat;
import android.util.Log;
import android.view.KeyEvent;

import com.autoipod.data.MediaItem;
import com.autoipod.data.MediaType;
import com.autoipod.jni.IAudioInformationCallBack;
import com.autoipod.jni.Ipod;
import com.autoipod.utils.AndroidUtil;
import com.autoipod.utils.IpodPcmLocalSocket;
import com.autoipod.utils.NowPlayingArtwork;
import com.autoipod.utils.NowPlayingUpdatesMediaItemAttributes;
import com.autoipod.utils.NowPlayingUpdatesPlaybackAttributes;
import com.google.gson.JsonObject;
import android.support.v4.content.LocalBroadcastManager;

import java.io.File;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import static com.autoipod.Constants.ID_IPOD_ALBUMS;
import static com.autoipod.Constants.ID_IPOD_ARTISTS;
import static com.autoipod.Constants.ID_IPOD_GENRES;
import static com.autoipod.Constants.ID_IPOD_PLAYLISTS;
import static com.autoipod.Constants.ID_IPOD_ROOT;
import static com.autoipod.Constants.ID_IPOD_SONGS;

public class IPodMediaBrowserPlayBackService extends MediaBrowserServiceCompat {

    public static String TAG = "IPodMediaBrowserPlayBackService";
    String ArtWorkUri = "";

    final String CoverageURI = Environment.getExternalStorageDirectory().getPath() + File.separator + Environment.DIRECTORY_DOWNLOADS+ "/ipod/APArtwork.jpeg";

    private Handler handler;

    private int position;

    private int status;

    public CallBack callBack;

    private LocalBinder localBinder = new LocalBinder();

    private IpodPcmLocalSocket mIpodPcmLocalSocket;
    private IPodMediaBrowserPlayBackService mIPodPlayBackService;
    private Context mContext;

    private IPodMediaSessionCallback mSessionCallback = null;
    private MediaSessionCompat mMediaSession = null;

    private IAudioInformationCallBack audioInformationCallBack = new IAudioInformationCallBack() {
        @Override
        public void OnAudioSampleRate(int samepleRate) {
            mIpodPcmLocalSocket.setmAutioSamepleRate(samepleRate);
        }
    };

    public interface CallBack{
        void updatePlayBackStatus();
        void updatePlayArtWork(String uri);
        void updateMediaItem(MediaItem mediaItem);
        void updatePodCastInfo(Object object,int index);
    }



    public class LocalBinder extends Binder
    {
        public IPodMediaBrowserPlayBackService getService()
        {
            return IPodMediaBrowserPlayBackService.this;
        }
    }





    @Override
    public void onCreate() {
        super.onCreate();
        mIPodPlayBackService = IPodMediaBrowserPlayBackService.this;
        mContext = IPodMediaBrowserPlayBackService.this;
        handler = new Handler();
        mIpodPcmLocalSocket = new IpodPcmLocalSocket(this);
        mIpodPcmLocalSocket.startPcmSocket();
        Ipod.getInstance().registerAudioInformationCallBack(audioInformationCallBack);
        Ipod.getInstance().registerNowPlayingPlaybackAttributesCallback(new PlayStatusUpdate());
        Ipod.getInstance().registerMediaNowPlayingArtworkDataCallback(new ArtWorkUpdate());
        Ipod.getInstance().registerNowPlayingMediaItemAttributesCallback(new MediaItemUpdate());
        Ipod.getInstance().registerNowPlayingMediaItemAttributesCallback(new MediaItemUpdate());
        initMediaSession();
    }

    public void StartNowPlayingUpdates() {
        Ipod.getInstance().iAP2StartNowPlayingUpdates();
    }
	 public void StopNowPlayingUpdates() {
		
        Ipod.getInstance().iAP2StopNowPlayingUpdates();
    }
	public void randMode() {
        Ipod.getInstance().random();
    }
	
   public void sigleMode() {
        Ipod.getInstance().repeat();
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
    if (SERVICE_INTERFACE.equals(intent.getAction())) {
        return super.onBind(intent);
    }
    return localBinder;

     //  return localBinder;
        //return binder.asBinder();
    }

    @Nullable
    @Override
    public BrowserRoot onGetRoot(@NonNull String clientPackageName, int clientUid, @Nullable Bundle rootHints) {
    
	Log.i(TAG,"onGetRoot clientPackageName= "+clientPackageName);
        return new BrowserRoot(ID_IPOD_ROOT, null);
    }



    @Override
    public void onLoadChildren(@NonNull final String parentId, @NonNull final Result<List<MediaBrowserCompat.MediaItem>> result) {
    
	Log.i(TAG,"onLoadChildren parentId= "+parentId);
        result.detach();

      //  final IStorageDevice device = LibMediaPlayerInstance.getActiveStorageDevice();
       // if(device != null){
            runBackground(new Runnable() {
                @Override
                public void run() {
                    try {
                        result.sendResult(BrowserProvider.loadChildren(/*device,*/parentId));
                    } catch (RuntimeException ignored) {
                        ignored.printStackTrace();
                    } //bitmap parcelization can fail
                }
            });
      //  }
    }

    public void onLoadItem(@NonNull final String itemId, @NonNull final Result<MediaBrowserCompat.MediaItem> result) {
		
		Log.i(TAG,"onLoadItem itemId= "+itemId);
        result.detach();

       // final IStorageDevice device = LibMediaPlayerInstance.getActiveStorageDevice();
      //  if(device != null){
           runBackground(new Runnable() {
                @Override
                public void run() {
                    try {
                        result.sendResult(BrowserProvider.loadItem(/*device,*/itemId));
                    } catch (RuntimeException ignored) {
                        ignored.printStackTrace();
                    } //bitmap parcelization can fail
               }
            });
       // }
    }

    public void onSearch(@NonNull final String query, final Bundle extras,
                         @NonNull final Result<List<MediaBrowserCompat.MediaItem>> result) {
        result.detach();
		Log.i(TAG,"onSearch 222 query ="+query);

      //  final IStorageDevice device = LibMediaPlayerInstance.getActiveStorageDevice();
      //  if(device != null){
           runBackground(new Runnable() {
                @Override
                public void run() {
                    try {
                        result.sendResult(BrowserProvider.search(/*device,*/query));
                    } catch (RuntimeException ignored) {
                        ignored.printStackTrace();
                    } //bitmap parcelization can fail
                }
            });
       // }
    }

    public void onCustomAction(@NonNull final String action, final Bundle extras,
                               @NonNull final Result<Bundle> result) {

        Log.i(TAG,"onCustomAction 222 action ="+action);
        result.detach();

      //  final IStorageDevice device = LibMediaPlayerInstance.getActiveStorageDevice();
     //   if(device != null){
          runBackground(new Runnable() {
                @Override
                public void run() {
                    try {
                        result.sendResult(BrowserProvider.customAction(/*device,*/action));
                    } catch (RuntimeException ignored) {
                        ignored.printStackTrace();
                    } //bitmap parcelization can fail
                }
            });
       // }
    }



    private void initMediaSession() {
        final Intent mediaButtonIntent = new Intent(Intent.ACTION_MEDIA_BUTTON);

        mediaButtonIntent.setClass(this, RemoteControlClientReceiver.class);


        final PendingIntent mbrIntent = PendingIntent.getBroadcast(this, 0, mediaButtonIntent, 0);
        final ComponentName mbrName = new ComponentName(this, RemoteControlClientReceiver.class);

        mSessionCallback = new IPodMediaSessionCallback();
        mMediaSession = new MediaSessionCompat(this, "IPodMediaBrowserPlayBackService", mbrName, mbrIntent);
        mMediaSession.setFlags(MediaSessionCompat.FLAG_HANDLES_MEDIA_BUTTONS
                | MediaSessionCompat.FLAG_HANDLES_TRANSPORT_CONTROLS);
        mMediaSession.setCallback(mSessionCallback);
        try {
            mMediaSession.setActive(true);
        } catch (NullPointerException e) {
            // Some versions of KitKat do not support AudioManager.registerMediaButtonIntent
            // with a PendingIntent. They will throw a NullPointerException, in which case
            // they should be able to activate a MediaSessionCompat with only transport
            // controls.
            mMediaSession.setActive(false);
            mMediaSession.setFlags(MediaSessionCompat.FLAG_HANDLES_TRANSPORT_CONTROLS);
            mMediaSession.setActive(true);
        }
        setSessionToken(mMediaSession.getSessionToken());
    }

    private final class IPodMediaSessionCallback extends MediaSessionCompat.Callback {
        @Override
        public boolean onMediaButtonEvent(Intent mediaButtonEvent) {
            final KeyEvent event = mediaButtonEvent.getParcelableExtra(Intent.EXTRA_KEY_EVENT);
			Log.i(TAG,"onMediaButtonEvent event.getKeyCode()="+event.getKeyCode());
          /*  if (!hasMedia() && event != null) {
                final int keyCode = event.getKeyCode();
                if (keyCode == KeyEvent.KEYCODE_MEDIA_PLAY || keyCode == KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE) {
                    if (event.getAction() == KeyEvent.ACTION_DOWN) {
                        Log.d(TAG, KeyEvent.keyCodeToString(keyCode));
                        loadLastAudioPlaylist();
                        return true;
                    } else {
                        return false;
                    }
                }
            }
            */
            return super.onMediaButtonEvent(mediaButtonEvent);
        }

        @Override
        public void onPlay() {
         /*   if (hasMedia())
                play();
            else
                loadLastAudioPlaylist();
            */
        }

        @Override
        public void onCustomAction(String action, Bundle extras) {

            Log.i(TAG,"onCustomAction 111 action="+action);
           /* if (TextUtils.equals(action, "shuffle")) {
                shuffle();
            } else if (TextUtils.equals(action, "repeat")) {
                switch (getRepeatType()) {
                    case Constants.REPEAT_NONE:
                        setRepeatType(Constants.REPEAT_ALL);
                        break;
                    case Constants.REPEAT_ALL:
                        setRepeatType(Constants.REPEAT_ONE);
                        break;
                    default:
                    case Constants.REPEAT_ONE:
                        setRepeatType(Constants.REPEAT_NONE);
                        break;
                }
            }
            */
        }

        @Override
        public void onPlayFromMediaId(String mediaId, Bundle extras) {
         /*   IStorageDevice device = LibMediaPlayerInstance.getActiveStorageDevice();
            if (mediaId.startsWith(ALBUM_PREFIX)) {
                if (device != null) {
                    MediaItem album = device.getAlbum(Integer.parseInt(mediaId.split("_")[1]));
                    if (album != null) {
                        load(device.getFiles(album), 0);
                    }
                }
            } else if (mediaId.startsWith(PLAYLIST_PREFIX)) {
                //todo
            } else
                try {
                    if (device != null) {
                        FileItem file = device.getFile(Integer.parseInt(mediaId));
                        if (file != null) {
                            load(file);
                        }
                    }
                } catch (NumberFormatException e) {
                    loadLocation(mediaId);
                }
                */
        }

        @Override
        public void onPlayFromUri(Uri uri, Bundle extras) {
          //  loadUri(uri);
        }

        @Override
        public void onPlayFromSearch(final String query, final Bundle extras) {
          /*  final IStorageDevice device = LibMediaPlayerInstance.getActiveStorageDevice();
            if (device == null) {
                return;
            }

            mMediaSession.setPlaybackState(new PlaybackStateCompat.Builder().setState(PlaybackStateCompat.STATE_CONNECTING, getTime(), 1.0f).build());
            LibMediaPlayerInstance.runBackground(new Runnable() {
                @Override
                public void run() {
                    final VoiceSearchParams vsp = new VoiceSearchParams(query, extras);
                    List<ArtistItem> artistItems = null;
                    List<AlbumItem> albumItems = null;
                    List<GenreItem> genreItems = null;
                    List<FileItem> fileItems = null;
                    FileGroup fileGroup = null;
                    int index = 0;
                    if (vsp.isAny) {
                        fileGroup = device.getFiles(MediaItem.CATEGORY_TYPE_AUDIO, true);
                        if (!isShuffling())
                            shuffle();
                    } else if (vsp.isArtistFocus) {
                        artistItems = device.searchArtist(vsp.artist);
                    } else if (vsp.isAlbumFocus) {
                        albumItems = device.searchAlbum(vsp.album);
                    } else if (vsp.isGenreFocus) {
                        genreItems = device.searchGenre(vsp.genre);
                    } else if (vsp.isSongFocus) {
                        fileItems = device.searchFile(MediaItem.CATEGORY_TYPE_AUDIO, vsp.song);
                    }

                    do {
                        if (fileGroup != null && !fileGroup.isEmpty()) {
                            break;
                        }

                        if (fileItems != null && !fileItems.isEmpty()) {
                            FileItem firstFileItem = fileItems.get(0);
                            fileGroup = device.getFiles(MediaItem.CATEGORY_TYPE_AUDIO, true);
                            for (FileItem fileItem : fileGroup.getAll()) {
                                if (fileItem.getId() == firstFileItem.getId()) {
                                    break;
                                }
                                index++;
                            }
                            break;
                        }

                        if (artistItems != null && !artistItems.isEmpty()) {
                            fileGroup = device.getFiles(artistItems.get(0));
                            break;
                        }

                        if (albumItems != null && !albumItems.isEmpty()) {
                            fileGroup = device.getFiles(albumItems.get(0));
                            break;
                        }

                        if (genreItems != null && !genreItems.isEmpty()) {
                            fileGroup = device.getFiles(genreItems.get(0));
                            break;
                        }
                    } while (false);

                    if (fileGroup != null && !fileGroup.isEmpty()) {
                        load(fileGroup, index);
                    }
                }
            });
            */
        }

        @Override
        public void onPause() {
        Log.i(TAG,"111 onPause");
         //   pause();
        }

        @Override
        public void onStop() {
        
        Log.i(TAG,"111 onStop");
          //  stop();
        }

        @Override
        public void onSkipToNext() {
        
        Log.i(TAG,"111 onSkipToNext");
        //    next_manual();
        }

        @Override
        public void onSkipToPrevious() {
        
        Log.i(TAG,"111 onSkipToPrevious");
         //   previous();
        }

        @Override
        public void onSeekTo(long pos) {
        
        Log.i(TAG,"111 onSeekTo");
         //   seek(pos);
        }

        @Override
        public void onFastForward() {
         //   seek(Math.min(getLength(), getTime() + 5000));
        }

        @Override
        public void onRewind() {
          //  seek(Math.max(0, getTime() - 5000));
        }

        @Override
        public void onSkipToQueueItem(long id) {
        //    playIndex((int) id);
        }
    }
    List<MediaBrowserCompat.MediaItem> processLoadChildren(String parentId)
    {

        List<MediaBrowserCompat.MediaItem> results = new ArrayList<>();
        //List<MediaItem> list = new ArrayList<>();
     //   Resources res = LibMediaPlayerInstance.getAppResources();
        switch (parentId)
        {
           // case ID_IPOD_ROOT:
              //  break;
            case ID_IPOD_ARTISTS:
                break;
            case ID_IPOD_ALBUMS:
                break;
            case ID_IPOD_SONGS:
                break;
            case ID_IPOD_GENRES:
                break;
            case ID_IPOD_PLAYLISTS:
                break;
        }
        return  null;
    }

    /*class PlayStatusUpdate extends NowPlayingUpdatesPlaybackAttributes {
        @Override
        public void onPlayBackPosition(int position) {
            IPodMediaBrowserPlayBackService.this.position = position;
          runOnUIThread(new Runnable() {
                @Override
                public void run() {
                    if(callBack!=null)
                    {
                        try{
                        callBack.updatePlayBackStatus();
                    }catch (Exception e)
                    {
                        e.printStackTrace();
                    }
                    }
                }
            });

        }

        @Override
        public void onPlayBackStatus(int status) {
            Log.d("IPO","onPlayBackStatus" + status);
            IPodMediaBrowserPlayBackService.this.status = status;
         runOnUIThread(new Runnable() {
                @Override
                public void run() {
                    if(callBack!=null)
                    {
                        try{
                        callBack.updatePlayBackStatus();
                    }catch (Exception e)
                    {
                        e.printStackTrace();
                    }
                    }
                }
            });

        }
    }
*/
    /*
    class ArtWorkUpdate extends NowPlayingArtwork
    {
        @Override
        public void onDataFinish() {
            File file = new File(CoverageURI);
            if(file.exists() && file.length()>0)
            {
                ArtWorkUri = CoverageURI;
            }else {
                ArtWorkUri = "";
            }
            Log.d("IPO","ArtWork data Finish" + ArtWorkUri);
           runOnUIThread(new Runnable() {
                @Override
                public void run() {
                    if(callBack!=null)
                    {
                        try {
                            callBack.updatePlayArtWork(ArtWorkUri);
                        }catch (Exception e)
                        {
                            e.printStackTrace();
                        }
                    }
                }
            });

        }
    }
*/
    private MediaItem currentPlayMedia = new MediaItem();
/*
    class MediaItemUpdate extends NowPlayingUpdatesMediaItemAttributes
    {
        @Override
        public void OnMediaItemPID(int datalen, byte[] data) {
            Log.d("PBS","OnMediaItemPID datalen "+datalen);
            Log.d("PBS","data : "+converPidArrayToStr(data));
            if(IPodDeviceMannager.getActiveDevice()!=null)
            {
                currentPlayMedia = IPodDeviceMannager.getActiveDevice().getMediaItemByPid(data);
                Log.d("PBS","currentPlayMedia " + currentPlayMedia);
                if(currentPlayMedia!=null && callBack!=null)
                {

                }
            }
        }
    }

*/
/*
    public String converPidArrayToStr(byte b[])
    {
        String str = "";
        if(b!=null)
        for(int i=0;i<b.length;i++)
            str = str+converByteToStr(b[i]);
        return str;
    }
    */
/*
    public String converByteToStr(byte b)
    {
        String str = Integer.toHexString(b);
        if(str.length() == 1)
            str = "0"+str;
        else if(str.length() > 2)
        {
            str = str.substring(str.length()-2,str.length());
        }
        str = "0x"+str;
        return str;
    }
    */

  /*  private class IIpodPlaybackServiceImp extends IIpodPlaybackService.Stub {


    }
    */


/*
     public void playPrev()
     {
         Ipod.getInstance().playProvTrack();
         Ipod.getInstance().send0Hid();
     }
     */

      public String getArtWorkURI() {
          return ArtWorkUri;
      }

      public void addCallBack(CallBack callBack)

      {
          mIPodPlayBackService.callBack = callBack;
      }
	  
     public void removeCallBack(CallBack callBack)
      {
        this.callBack = null;
      }
      public void pause()
      {
      Log.i(TAG,"111 pause");
          Ipod.getInstance().pause();
      }

      public void play()
      {
       Log.i(TAG,"111 play");
          Ipod.getInstance().play();
		   LocalBroadcastManager localBroadcastManager = LocalBroadcastManager.getInstance(mContext);
		 localBroadcastManager.sendBroadcast(new Intent(Constants.ACTION_IPOD_REQUEST_AUDIO_FOCUS));
      }

      public void playNext()
      {
        Log.i(TAG,"111 playNext");
          Ipod.getInstance().playNextTrack();
          Ipod.getInstance().send0Hid();
		  
			LocalBroadcastManager localBroadcastManager = LocalBroadcastManager.getInstance(mContext);
		  localBroadcastManager.sendBroadcast(new Intent(Constants.ACTION_IPOD_REQUEST_AUDIO_FOCUS));
      }

     public void playPrev() {
	 	  Log.i(TAG,"111 playPrev");
        Ipod.getInstance().playProvTrack();
        Ipod.getInstance().send0Hid();
		
		  LocalBroadcastManager localBroadcastManager = LocalBroadcastManager.getInstance(mContext);
		localBroadcastManager.sendBroadcast(new Intent(Constants.ACTION_IPOD_REQUEST_AUDIO_FOCUS));
      }
      public void playAblum(MediaItem mediaItem)
      {
          byte[] albumPid = mediaItem.getMediaItemAlbumPid();
          int albumPidSize = albumPid.length;
          byte[] uid = mediaItem.getUid();
          int uidSize = uid.length;
          byte[] pid = mediaItem.getMediaItemPid();
          int pidSize = pid.length;
          Ipod.getInstance().iAP2PlayMediaLibraryCollection(albumPidSize,albumPid,2,0,uidSize,uid,pidSize,pid);
		  
			LocalBroadcastManager localBroadcastManager = LocalBroadcastManager.getInstance(mContext);
		  localBroadcastManager.sendBroadcast(new Intent(Constants.ACTION_IPOD_REQUEST_AUDIO_FOCUS));
      }

      public void playMediaItems(List<MediaItem> mediaItems,int index)
      {
          Log.d("PBS",String.format("mediaSize %d index %d",mediaItems.size(),index));
          int pidsize = mediaItems.get(0).getMediaItemPid().length;
          ByteBuffer byteBuffer = ByteBuffer.allocate(mediaItems.size()*pidsize);
          for(MediaItem mi : mediaItems)
          {
              byteBuffer.put(mi.getMediaItemPid());
          }
          byte[] itemPid = byteBuffer.array();
          byte[] uid = mediaItems.get(0).getUid();
          int uidSize = uid.length;
          Ipod.getInstance().iAP2PlayMediaLibraryItems(itemPid.length,itemPid,index,uidSize,uid);
		  
			LocalBroadcastManager localBroadcastManager = LocalBroadcastManager.getInstance(mContext);
		  localBroadcastManager.sendBroadcast(new Intent(Constants.ACTION_IPOD_REQUEST_AUDIO_FOCUS));
      }

      public List<MediaItem> getALLSong()
      {
          if(IPodDeviceMannager.getActiveDevice()!=null)
          {
              return IPodDeviceMannager.getActiveDevice().getALLSongs();
          }
          return Collections.emptyList();
      }

      public List<String> getALLGennre()
      {
          if(IPodDeviceMannager.getActiveDevice()!=null)
          {
              return IPodDeviceMannager.getActiveDevice().getALLGennre();
          }
          Log.d("IPO","current IPodDevice null");
          return Collections.emptyList();
      }

      public List<String> getAristByGennre(String gennre)
      {
          if(IPodDeviceMannager.getActiveDevice()!=null)
          {
              return IPodDeviceMannager.getActiveDevice().getAristByGennre(gennre);
          }
          return Collections.emptyList();
      }

      public List<String> getAlbumByArist(String arist)
      {
          if(IPodDeviceMannager.getActiveDevice()!=null)
          {
              return IPodDeviceMannager.getActiveDevice().getAlbumByArist(arist);
          }
          return Collections.emptyList();
      }

      public List<MediaItem> getSongByAlbum(String album)
      {
          if(IPodDeviceMannager.getActiveDevice()!=null)
          {
              return IPodDeviceMannager.getActiveDevice().getSongByAlbum(album);
          }
          return Collections.emptyList();
      }

      public List<MediaItem> getPodCast()
      {
          if(IPodDeviceMannager.getActiveDevice()!=null)
          {
              return IPodDeviceMannager.getActiveDevice().getPodCast();
          }
          return Collections.emptyList();
      }

      public List<MediaItem> getAudioBook()
      {
          if(IPodDeviceMannager.getActiveDevice()!=null)
          {
              return IPodDeviceMannager.getActiveDevice().getAudioBook();
          }
          return Collections.emptyList();
      }

      public List<MediaType> getMediaTypes()
      {
          if(IPodDeviceMannager.getActiveDevice()!=null)
          {
              return IPodDeviceMannager.getActiveDevice().getMediaTypes();
          }
          return Collections.emptyList();
      }
	  
	   int lastSendPos;
    class PlayStatusUpdate extends NowPlayingUpdatesPlaybackAttributes {

        @Override
        public void OnPlayBackAppName(int len, String name) {
            Log.d("PBS","onAPPName "+name);
        }

        @Override
        public void onPlayBackPosition(int position) {
            Log.d("PBS","onPlayBackPosition:"+position);
            if(position-lastSendPos >=1000)
            {
                sendStatusInfo(currentPlayMedia);
                lastSendPos = position;
            }
            IPodMediaBrowserPlayBackService.this.position = position;
            runOnUIThread(new Runnable() {
                @Override
                public void run() {
                    //send info to meter
                    if (callBack != null) {
                        callBack.updatePlayBackStatus();
                    }
                }
            });
        }

        @Override
        public void onPlayBackStatus(int status) {
            Log.d("PBS", "onPlayBackStatus:" + status);
            IPodMediaBrowserPlayBackService.this.status = status;
			if(status == 1)//    1   for play
			{
			  mIpodPcmLocalSocket.getAudioPlayer().requestFocus();
			}
            sendStatusInfo(currentPlayMedia);
            runOnUIThread(new Runnable() {
                @Override
                public void run() {
                    if (callBack != null) {
                        callBack.updatePlayBackStatus();
                    }
                }
            });
        }
    }

    class ArtWorkUpdate extends NowPlayingArtwork {
        @Override
        public void onDataFinish() {
            File file = new File(CoverageURI);
            if (file.exists() && file.length() > 0) {
                ArtWorkUri = CoverageURI;
            } else {
                ArtWorkUri = "";
            }

            Log.d("IPO", "ArtWork data Finish" + ArtWorkUri);

            runOnUIThread(new Runnable() {
                @Override
                public void run() {
                    if (callBack != null) {
                        callBack.updatePlayArtWork(ArtWorkUri);
                    }
                }
            });
        }
    }

    //private MediaItem currentPlayMedia;

    private int mediaType;

    class MediaItemUpdate extends NowPlayingUpdatesMediaItemAttributes {
        @Override
        public void OnMediaItemRawData(int dataLen, byte[] data) {
            MediaItem mediaItem = new MediaItem();
            int i = 4;
            int param_len = 0;
            int id;
            int  duration;
            int value;
			int data_len1;
			int data_len2;
            byte[] data_array = new byte[8];
            boolean hasPid = false;
            while(i<dataLen)
            {
                 data_len1 = data[ i ];
				 data_len2 = data[ i+1 ];
                param_len =    ( (data_len1&0xff) << 8 ) | (data_len2&0xff);
                id =  ( (data[ i+2 ]&0xff) << 8 ) | (data[ i+3 ]&0xff);
                // uint8_t *s_char;
                // LOGI("%s line:%d id=%d param_len=%d\n",__FUNCTION__,__LINE__,id,param_len);
                Log.i(TAG,"OnMediaItemRawData id="+id+" param_len="+param_len);

                switch(id)
                {

                    case 0: //MediaItemPersistentIdentifier


                        for(int k=0;k<(param_len-4);k++)
                        {
                            data_array[k] = data[i+4+k];

                            //LOGI("data_array[%d] :0x%02x\n",k,data_array[k]);
                        }
                        OnMediaItemPID(8,data_array);
                        currentPlayMedia.setMediaItemPid(data_array);
                        hasPid = true;
                        // mIpodService->notifyOnMediaItemPID(data_array,8);
                        break;
                    case 1:
                    {
                        byte[] data_char = new byte[param_len-4];
                        System.arraycopy(data,i+4,data_char,0,param_len-4);
                        try {


                            String title = new String(data_char,"utf-8");

                            onMediaItemTile(title.length(),title);
                            currentPlayMedia.setMediaItemTitle(title);
                        }catch (Exception e)
                        {
                            e.printStackTrace();
                        }
                    }
/*
                    //LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
                    s_char = (uint8_t *)malloc(param_len-4);
                    //LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
                    memcpy(s_char,&data[i+4],param_len-4);
                    //LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
                    mIpodService->notifyOnMediaItemTile((uint8_t *)s_char,param_len-4);
                    //	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
                    free(s_char);
                    */

                    break;
                    case 2:
                        value = data[ i+4 ];
                        OnMediaItemMediaType(value);
                        currentPlayMedia.setMediaItemType(value);
                        // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
                        // mIpodService->notifyMediaItemMediaType(value);
                        //mIpodService->notifyOnPlayBackShuffleMode(value);
                        break;
                    case 3:

                        //LOGI("%s line:%d \n",__FUNCTION__,__LINE__);
                        value = data[ i+4 ];
                        OnMediaItemRating(value);
                        currentPlayMedia.setMediaItemRating(value);
                        // mIpodService->notifyMediaItemRating(value);
                        //mIpodService->notifyMediaItemMediaType(value);
                        break;
                    case 4:
                        int data1=data[ i+4 ];
                        int data2=data[ i+1+4 ];
                        int data3=data[ i+2 +4];
                        int data4= i+3+4;

					//	(high_h & 0xff) << 24 | (high_l & 0xff) << 16 | (low_h & 0xff) << 8 | low_l & 0xff;
						
                       // duration = ( data[ i+4 ] << 24 ) | (data[ i+1+4 ]<<16)  | (data[ i+2 +4] <<8) | data[ i+3+4 ];
                        duration = ( (data1&0xff) << 24 ) | ((data2&0xff)<<16)  | ((data3&0xff) <<8) | (data4&0xff);
					    Log.i("huanghanjing","############# duration = "+duration);
                        //	LOGI("%s line:%d duration=%d\n",__FUNCTION__,__LINE__,duration);
                        onDuration(duration);
                        currentPlayMedia.setMediaItemDuration(duration);
                        // mIpodService->notifyOnDuration(duration);
                        break;
                    case 5:

                        for(int k=0;k<(param_len-4);k++)
                        {
                            data_array[k] = data[i+4+k];
                        }
                        OnMediaItemAlbumPID(8,data_array);
                        currentPlayMedia.setMediaItemmAlbumPid(data_array);
                        //mIpodService->notifyOnMediaItemAlbumPID(data_array,8);
                        break;
                    case 6:

                    {
                        byte[] data_char = new byte[param_len-4];
                        System.arraycopy(data,i+4,data_char,0,param_len-4);
                        try {


                            String title = new String(data_char,"utf-8");
                            // OnMediaitemA
                            onMediaItemAlumTitle(title.length(),title);
                            currentPlayMedia.setMediaItemAlbumTitle(title);
                        }catch (Exception e)
                        {
                            e.printStackTrace();
                        }
                    }
                /*
                    s_char = (uint8_t *)malloc(param_len-4);
                    memcpy(s_char,&data[i+4],param_len-4);
                    mIpodService->notifyOnMediaItemAlumTitle((uint8_t *)s_char,param_len-4);
                    free(s_char);
                    */

                    break;
                    case 7:

                        value = 	 ((data[ i+4]&0xff) <<8) | (data[ i+1+4 ]&0xff);
                        OnMediaItemAlbumTrackNumber(value);
                        mediaItem.setMediaItemAlbumTrackNumber(value);
                        // mIpodService->notifyMediaItemAlbumTrackNumber(value);
                        break;
                    case 8:
                        value = 	 ((data[ i+4]&0xff) <<8) | (data[ i+1+4 ]&0xff);
                        OnMediaItemAlbumTrackCount(value);
                        currentPlayMedia.setMediaItemAlbumTrackCount(value);

                        // mIpodService->notifyMediaItemAlbumTrackCount(value);
                        break;
                    case 9:
                        value = 	( (data[ i+4]&0xff) <<8) | (data[ i+1+4 ]&0xff);
                        OnMediaItemAlbumDiscNumber(value);
                        currentPlayMedia.setMediaItemAlbumDiscNumber(value);
                        //  mIpodService->notifyMediaItemAlbumDiscNumber(value);
                        break;
                    case 10:
                        value = 	 ((data[ i+4] &0xff)<<8) | (data[ i+1+4 ]&0xff);
                        OnMediaItemAlbumDiscCount(value);
                        currentPlayMedia.setMediaItemAlbumDiscCount(value);
                        // mIpodService->notifyMediaItemAlbumDiscCount(value);
                        break;
                    case 11:

                        for(int k=0;k<(param_len-4);k++)
                        {
                            data_array[k] = data[i+4+k];
                        }
                        OnMediaItemArtistPID(8,data_array);
                        currentPlayMedia.setMediaItemArtistPid(data_array);
                        // mIpodService->notifyOnMediaItemArtistPID(data_array,8);
                        break;
                    case 12:
                    {
                        byte[] data_char = new byte[param_len-4];
                        System.arraycopy(data,i+4,data_char,0,param_len-4);
                        try {


                            String title = new String(data_char,"utf-8");
                            // OnMediaitemA
                            onMediaItemArtist(title.length(),title);
                            currentPlayMedia.setMediaItemArtist(title);
                            //  OnMediaItemArtist(title.length(),title);
                        }catch (Exception e)
                        {
                            e.printStackTrace();
                        }
                    }
                    /*s_char = (uint8_t *)malloc(param_len-4);
                    memcpy(s_char,&data[i+4],param_len-4);
                    mIpodService->notifyOnMediaItemArtist((uint8_t *)s_char,param_len-4);
                    free(s_char);
                    */
                    break;
                    case 13:

                        for(int k=0;k<(param_len-4);k++)
                        {
                            data_array[k] = data[i+4+k];
                        }
                        OnMediaItemAlbumArtistPID(8,data_array);
                        currentPlayMedia.setMediaItemArtistPid(data_array);
                        //   mIpodService->notifyOnMediaItemAlbumArtistPID(data_array,8);
                        break;
                    case 14:
                    {
                        byte[] data_char = new byte[param_len-4];
                        System.arraycopy(data,i+4,data_char,0,param_len-4);
                        try {


                            String title = new String(data_char,"utf-8");
                            // OnMediaitemA
                            onMediaItemAlumArtist(title.length(),title);
                            currentPlayMedia.setMediaItemAlbumArtist(title);
                            //  OnMediaItemArtist(title.length(),title);
                        }catch (Exception e)
                        {
                            e.printStackTrace();
                        }
                    }
                /*
                    s_char = (uint8_t *)malloc(param_len-4);
                    memcpy(s_char,&data[i+4],param_len-4);
                    mIpodService->notifyOnMediaItemAlumArtist((uint8_t *)s_char,param_len-4);
                    free(s_char);
                    */
                    break;
                    case 15:

                        for(int k=0;k<(param_len-4);k++)
                        {
                            data_array[k] = data[i+4+k];
                        }
                        OnMediaItemGenrePID(8,data_array);
                        currentPlayMedia.setMediaItemGenrePid(data_array);
                        // mIpodService->notifyOnMediaItemGenrePID(data_array,8);
                        break;
                    case 16:
                    {
                        byte[] data_char = new byte[param_len-4];
                        System.arraycopy(data,i+4,data_char,0,param_len-4);
                        try {


                            String title = new String(data_char,"utf-8");
                            // OnMediaitemA

                            OnMediaItemGenre(title.length(),title);
                            currentPlayMedia.setMediaItemGenre(title);
                            //  OnMediaItemArtist(title.length(),title);
                        }catch (Exception e)
                        {
                            e.printStackTrace();
                        }
                    }
                /*
                    s_char = (uint8_t *)malloc(param_len-4);
                    memcpy(s_char,&data[i+4],param_len-4);
                    mIpodService->notifyOnMediaItemGenre((uint8_t *)s_char,param_len-4);
                    free(s_char);
                    */
                    break;
                    case 17:

                        for(int k=0;k<(param_len-4);k++)
                        {
                            data_array[k] = data[i+4+k];
                        }
                        OnMediaItemComposerPID(8,data_array);
                        currentPlayMedia.setMediaItemComposerPid(data_array);
                        //  mIpodService->notifyOnMediaItemComposerPID(data_array,8);
                        break;
                    case 18:
                    {
                        byte[] data_char = new byte[param_len-4];
                        System.arraycopy(data,i+4,data_char,0,param_len-4);
                        try {


                            String title = new String(data_char,"utf-8");
                            // OnMediaitemA

                            OnMediaItemGenreComposer(title.length(),title);
                            currentPlayMedia.setMediaItemComposer(title);
                            //  OnMediaItemArtist(title.length(),title);
                        }catch (Exception e)
                        {
                            e.printStackTrace();
                        }
                    }
                  /*  s_char = (uint8_t *)malloc(param_len-4);
                    memcpy(s_char,&data[i+4],param_len-4);
                    mIpodService->notifyOnMediaItemComposer((uint8_t *)s_char,param_len-4);
                    free(s_char);
                    */
                    break;
                    case 19:
                        value = data[ i+4 ];

                        OnMediaItemIsPartOfCompilation(value==1?true:false);
                        currentPlayMedia.setIsPartOfCompilation(value==1?true:false);
                        // mIpodService->notifyOnMediaItemIsPartOfCompilation(value);
                        //	 mIpodService->notifyOnPBAppleMusicRadioAd(value);
                        break;
                    case 20:

                        break;
                    case 21:
                        value = data[ i+4 ];
                        OnMediaItemIsLikeSupported(value==1?true:false);
                        currentPlayMedia.setLikeSupported(value==1?true:false);
                        //mIpodService->notifyOnMediaItemIsLikeSupported(value);
                        break;
                    case 22:
                        value = data[ i+4 ];
                        OnMediaItemIsBeanSupported(value==1?true:false);
                        currentPlayMedia.setBaned(value==1?true:false);
                        // mIpodService->notifyOnMediaItemIsBeanSupported(value);
                        break;
                    case 23:

                        value = data[ i+4 ];
                        OnMediaItemIsLiked(value==1?true:false);
                        currentPlayMedia.setLiked(value==1?true:false);
                        //   mIpodService->notifyOnMediaItemIsLiked(value);
                        break;
                    case 24:
                        value = data[ i+4 ];
                        OnMediaItemIsBanned(value==1?true:false);
                        currentPlayMedia.setBaned(value==1?true:false);
                        // mIpodService->notifyOnMediaItemIsBanned(value);
                        break;
                    case 25:

                        value = data[ i+4 ];
                        OnMediaItemIsResidentOnDevice(value==1?true:false);
                        currentPlayMedia.setResidentOnDevice(value==1?true:false);
                        //mIpodService->notifyOnMediaItemIsResidentOnDevice(value);
                        break;
                    case 26:
                        value = data[ i+4 ];
                        OnMediaItemArtWorkFileTransferID(value);
                        currentPlayMedia.setArtWorkFileTransferId(value);
                        // mIpodService->notifyMediaItemArtWorkFileTransferID(value);
                        //  mIpodService->notifyMediaItemRating(value);

                        break;
                    case 27:
                        value = 	( (data[ i+4]&0xff) <<8) | (data[ i+1+4 ]&0xff);
                        OnMediaItemChapterCount(value);
                        currentPlayMedia.setChapterCount(value);
                        // mIpodService->notifyMediaItemChapterCount(value);
                        break;

                }
                i+=param_len;
            }

            if (currentPlayMedia != null && callBack != null && hasPid) {
                sendMediaInfo(currentPlayMedia);
                lastSendPos = 0;
                runOnUIThread(new Runnable() {
                    @Override
                    public void run() {
                    Log.i(TAG,"updateMediaItem 222");
                        callBack.updateMediaItem(currentPlayMedia);
                    }
                });
            }

        }
        @Override
        public void OnMediaItemPID(int datalen, byte[] data) {
            Log.d("PBS", "OnMediaItemPID datalen " + datalen);
            Log.d("PBS", "data : " + converPidArrayToStr(data));
            Log.d("PBS", "active devie" + IPodDeviceMannager.getActiveDevice());
        /*    currentPlayMedia = IPodDeviceMannager.getActiveDevice().getMediaItemByPid(data);
            Log.d("PBS", "currentPlayMedia " + currentPlayMedia);
            if (currentPlayMedia != null && callBack != null) {
                sendMediaInfo(currentPlayMedia);
                lastSendPos = 0;
                runOnUIThread(new Runnable() {
                    @Override
                    public void run() {
                        callBack.updateMediaItem(currentPlayMedia);
                    }
                });
            }
            */
        }

        @Override
        public void onMediaItemTile(int len, final String mediaItemTtile) {
            Log.d("PBS", "onMediaItemTile call " + mediaItemTtile);
            if (callBack != null) {
                runOnUIThread(new Runnable() {
                    @Override
                    public void run() {
                        callBack.updatePodCastInfo(mediaItemTtile, 0);
                    }
                });
            }
        }

        @Override
        public void onDuration(final int duration) {
            Log.d("PBS", "onDuration call " + duration);
            if (callBack != null) {
                runOnUIThread(new Runnable() {
                    @Override
                    public void run() {
                        callBack.updatePodCastInfo(duration, 1);
                    }
                });
            }
        }

        @Override
        public void onMediaItemAlumTitle(int len, final String title) {
            Log.d("PBS", "onMediaItemAlumTitle " + title);
            if (callBack != null) {
                runOnUIThread(new Runnable() {
                    @Override
                    public void run() {
                        callBack.updatePodCastInfo(title, 2);
                    }
                });
            }
        }

        @Override
        public void onMediaItemArtist(int len, final String artist) {
            Log.d("PBS", "onMediaItemArtist " + artist);
            if (callBack != null) {
                runOnUIThread(new Runnable() {
                    @Override
                    public void run() {
                        callBack.updatePodCastInfo(artist, 3);
                    }
                });
            }
        }
	}

      public int getPosition() {
          return position;
      }

      public int getStatus() {
          return status;
      }


      public MediaItem getCurrentPlayMedia() {
          return currentPlayMedia;
      }
/*
 void runOnUIThread(Runnable runnable) {
        handler.post(runnable);
    }
    */
     
      public void setIpodPlayPosition(int position)
      {
          Ipod.getInstance().setIpodPlayPosition(position);
      }


    private final int maxThreads = Math.max(AndroidUtil.isJellyBeanMR1OrLater ? Runtime.getRuntime().availableProcessors() : 2, 1);
    public static final ThreadFactory THREAD_FACTORY = new ThreadFactory() {
        @Override
        public Thread newThread(Runnable runnable) {
            final Thread thread = new Thread(runnable);
            thread.setPriority(Process.THREAD_PRIORITY_DEFAULT+Process.THREAD_PRIORITY_LESS_FAVORABLE);
            return thread;
        }
    };
    private final ThreadPoolExecutor mThreadPool = new ThreadPoolExecutor(Math.min(2, maxThreads), maxThreads, 30, TimeUnit.SECONDS,
            new LinkedBlockingQueue<Runnable>(), THREAD_FACTORY);
    private Handler mHandler = new Handler(Looper.getMainLooper());
    public  void runBackground(Runnable runnable) {
        if (Looper.myLooper() != Looper.getMainLooper())
            runnable.run();
        else
            mThreadPool.execute(runnable);
    }

    void runOnUIThread(Runnable runnable)

    {
        handler.post(runnable);
    }
	
	 public String converPidArrayToStr(byte b[]) {
        String str = "";
        if (b != null)
            for (int i = 0; i < b.length; i++)
                str = str + converByteToStr(b[i]);
        return str;
    }

    public String converByteToStr(byte b) {
        String str = Integer.toHexString(b);
        if (str.length() == 1)
            str = "0" + str;
        else if (str.length() > 2) {
            str = str.substring(str.length() - 2, str.length());
        }
        str = "0x" + str;
        return str;
    }

    JsonObject jsonObject = new JsonObject();
    JsonObject message = new JsonObject();
    JsonObject content = new JsonObject();

    private void sendMediaInfo(MediaItem mediaItem) {
        jsonObject.addProperty("type", "multimedia");
        jsonObject.addProperty("timestamp",System.currentTimeMillis());
        message.addProperty("type", "media_info");
        content.addProperty("source", 2);
        content.addProperty("author",mediaItem.getMediaItemArtist());
        content.addProperty("title",mediaItem.getMediaItemTitle());
        content.addProperty("length",mediaItem.getMediaItemDuration());
        message.add("content",content);
        jsonObject.add("message",message);
        Log.d("json", jsonObject.toString());
        Settings.System.putString(getContentResolver(), "multimedia", jsonObject.toString());
    }

    JsonObject statusObject = new JsonObject();
    JsonObject statusMessage = new JsonObject();
    JsonObject statusContent = new JsonObject();
    private void sendStatusInfo(MediaItem mediaItem) {
        if(mediaItem!=null)
        {
            statusObject.addProperty("type", "multimedia");
            statusObject.addProperty("timestamp",System.currentTimeMillis());
            statusMessage.addProperty("type", "media_status");
            statusContent.addProperty("play_status",status == 1 ? 1 : 0);
            float p = (float) position / this.currentPlayMedia.getMediaItemDuration();
            statusContent.addProperty("progress",p);
            statusMessage.add("content",statusContent);
            statusObject.add("message",statusMessage);
            Log.d("json", statusObject.toString());
            Settings.System.putString(getContentResolver(), "multimedia", statusObject.toString());
        }
    }
}
