package com.example.txl.tool.mediaprovider.Interactive;

import android.content.ContentProviderClient;
import android.database.Cursor;
import android.os.RemoteException;
import android.provider.BaseColumns;
import android.provider.MediaStore;

import com.example.txl.tool.mediaprovider.MediaInfo;
import com.example.txl.tool.mediaprovider.StorageVolumeStrategy;

import java.util.ArrayList;
import java.util.List;

/**
 * 查询所有包含视频的文件夹
 * 查询某个文件夹下的视频
 * 查询所有视频文件
 * */
public class DefaultVideoBrowser implements IQueryStrategy{

    private ContentProviderClient mMediaProvider;

    public DefaultVideoBrowser(ContentProviderClient mMediaProvider) {
        this.mMediaProvider = mMediaProvider;
    }

    @Override
    public List<MediaInfo> queryAllDir() {
        ArrayList<MediaInfo> mediaInfos = new ArrayList<>();
        try {
            Cursor cursor = mMediaProvider.query(MediaStore.Files.getContentUri(StorageVolumeStrategy.getVolume()),new String[]{BaseColumns._ID,MediaStore.MediaColumns.DISPLAY_NAME,MediaStore.MediaColumns.DATA},"children_type NOTNULL AND children_type & 0x02 != 0",new String[]{},"_display_name ASC");
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
            Cursor cursor = mMediaProvider.query(MediaStore.Video.Media.getContentUri(StorageVolumeStrategy.getVolume()),new String[]{BaseColumns._ID,MediaStore.MediaColumns.DISPLAY_NAME,MediaStore.MediaColumns.DATA},null,null,"title ASC");
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
            Cursor cursor = mMediaProvider.query(MediaStore.Video.Media.getContentUri(StorageVolumeStrategy.getVolume()),new String[]{BaseColumns._ID,MediaStore.MediaColumns.DISPLAY_NAME,MediaStore.MediaColumns.DATA},"parent = ?",new String[]{""+pid},"title ASC");
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
