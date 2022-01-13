package com.example.txl.tool.mediaprovider.Interactive;

import android.content.ContentProviderClient;
import android.database.Cursor;
import android.net.Uri;
import android.os.RemoteException;
import android.provider.BaseColumns;
import android.provider.MediaStore;

import com.example.txl.tool.mediaprovider.Interactive.IQueryStrategy;
import com.example.txl.tool.mediaprovider.MediaInfo;
import com.example.txl.tool.mediaprovider.StorageVolumeStrategy;

import java.util.ArrayList;
import java.util.List;

/**
 * 查询所有包含视频的文件夹
 * 查询某个文件夹下的视频
 * 查询所有视频文件
 * */
public class DefaultAudioBrowser implements IQueryStrategy {
    public static final String AUTHORITY = "media";

    private static final String CONTENT_AUTHORITY_SLASH = "content://" + AUTHORITY + "/";

    public static Uri getContentUri(String volumeName) {
        return Uri.parse(CONTENT_AUTHORITY_SLASH + volumeName +
                "/audio/media");
    }

    private ContentProviderClient mMediaProvider;

    public DefaultAudioBrowser(ContentProviderClient mMediaProvider) {
        this.mMediaProvider = mMediaProvider;
    }

    /**
     * 查询包含所有视频的文件夹
     * */
    private List<MediaInfo> queryAllImageFolders(){
        ArrayList<MediaInfo> mediaInfos = new ArrayList<>();
        try {
            Cursor cursor = mMediaProvider.query(MediaStore.Files.getContentUri(StorageVolumeStrategy.getVolume()),new String[]{BaseColumns._ID,MediaStore.MediaColumns.DISPLAY_NAME,MediaStore.MediaColumns.DATA},"children_type NOTNULL AND children_type & 0x04 != 0",new String[]{},"_display_name ASC");
            while (cursor != null && cursor.moveToNext()){
                long id = cursor.getLong(0);
                String displayName = cursor.getString(1);
                String path = cursor.getString(2);
                mediaInfos.add(new MediaInfo(id,displayName,path,true));
            }
            if(cursor != null)
            cursor.close();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        return mediaInfos;
    }

    /**
     * 查询某个目录下的视频文件
     * */
    private List<MediaInfo> queryImagesByPId(long pid){
        ArrayList<MediaInfo> mediaInfos = new ArrayList<>();
        try {
            Cursor cursor = mMediaProvider.query(getContentUri(StorageVolumeStrategy.getVolume()),new String[]{BaseColumns._ID,MediaStore.MediaColumns.DISPLAY_NAME,MediaStore.MediaColumns.DATA},"parent = ?",new String[]{""+pid},"title ASC");
            while (cursor != null && cursor.moveToNext()){
                long id = cursor.getLong(0);
                String displayName = cursor.getString(1);
                String path = cursor.getString(2);
                mediaInfos.add(new MediaInfo(id,displayName,path,false));
            }
            if(cursor != null)
                cursor.close();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        return mediaInfos;
    }

    /**
     * 查询所有视频文件
     * */
    private List<MediaInfo> queryAllAudio(){
        ArrayList<MediaInfo> mediaInfos = new ArrayList<>();
        try {
            Uri uri = getContentUri(StorageVolumeStrategy.getVolume());
            Cursor cursor = mMediaProvider.query(uri,new String[]{BaseColumns._ID,MediaStore.MediaColumns.DISPLAY_NAME,MediaStore.MediaColumns.DATA},null,null,"title ASC");
            while (cursor != null && cursor.moveToNext()){
                long id = cursor.getLong(0);
                String displayName = cursor.getString(1);
                String path = cursor.getString(2);
                mediaInfos.add(new MediaInfo(id,displayName,path,false));
            }
            if(cursor != null)
                cursor.close();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        return mediaInfos;
    }

    @Override
    public List<MediaInfo> queryAllDir() {
        ArrayList<MediaInfo> mediaInfos = new ArrayList<>();
        try {
            Cursor cursor = mMediaProvider.query(MediaStore.Files.getContentUri(StorageVolumeStrategy.getVolume()),new String[]{BaseColumns._ID,MediaStore.MediaColumns.DISPLAY_NAME,MediaStore.MediaColumns.DATA},"children_type NOTNULL AND children_type & 0x01 != 0",new String[]{},"_display_name ASC");

//            Uri uri = android.net.Uri.parse("content://hs_media/external/file");
//            String selectArgs = "children_type NOTNULL AND children_type & 0x01 != 0";
//            Cursor cursor = mMediaProvider.query(uri,null,selectArgs,null,null);
            while (cursor != null && cursor.moveToNext()){
                long id = cursor.getLong(0);
                String displayName = cursor.getString(1);
                String path = cursor.getString(2);
                mediaInfos.add(new MediaInfo(id,displayName,path,true));
            }
            if(cursor != null)
                cursor.close();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        return mediaInfos;
    }

    @Override
    public List<MediaInfo> queryAllFile() {
        ArrayList<MediaInfo> mediaInfos = new ArrayList<>();
        try {
            Uri uri = getContentUri(StorageVolumeStrategy.getVolume());
            Cursor cursor = mMediaProvider.query(uri,new String[]{BaseColumns._ID,MediaStore.MediaColumns.DISPLAY_NAME,MediaStore.MediaColumns.DATA},null,null,"title ASC");
            while (cursor != null && cursor.moveToNext()){
                long id = cursor.getLong(0);
                String displayName = cursor.getString(1);
                String path = cursor.getString(2);
                mediaInfos.add(new MediaInfo(id,displayName,path,false));
            }
            if(cursor != null)
                cursor.close();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        return mediaInfos;
    }

    @Override
    public List<MediaInfo> queryAllFileByPid(long pid) {
        ArrayList<MediaInfo> mediaInfos = new ArrayList<>();
        try {
            Cursor cursor = mMediaProvider.query(getContentUri(StorageVolumeStrategy.getVolume()),new String[]{BaseColumns._ID,MediaStore.MediaColumns.DISPLAY_NAME,MediaStore.MediaColumns.DATA},"parent = ?",new String[]{""+pid},"title ASC");
            while (cursor != null && cursor.moveToNext()){
                long id = cursor.getLong(0);
                String displayName = cursor.getString(1);
                String path = cursor.getString(2);
                mediaInfos.add(new MediaInfo(id,displayName,path,false));
            }
            if(cursor != null)
                cursor.close();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        return mediaInfos;
    }
}
