package com.example.txl.tool.mediaprovider;

import android.content.ContentProviderClient;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.RemoteException;
import android.provider.MediaStore;
import android.util.Log;

public class MediaProviderUtils {
    private final String TAG = getClass().getSimpleName();
    ContentProviderClient mContentProviderClient;
    public void init(Context context){
        mContentProviderClient = context.getContentResolver().acquireContentProviderClient(MediaStore.AUTHORITY);
    }


    public void release(){
        if(mContentProviderClient != null){
            ContentProviderClient temp = mContentProviderClient;
            mContentProviderClient = null;
            temp.release();
        }
    }

    /**
     * 查询所有存储卷
     * */
    public void queryAllVolume(){
        Uri uri = Uri.parse("content://media/");
        Cursor cursor = null;
        try {
            cursor = mContentProviderClient.query(uri,new String[]{"display_name","uuid"},null,null,null);
            while (cursor!= null && cursor.moveToNext()){
                String display_name = cursor.getString(cursor.getColumnIndex("display_name"));
                int  uuid = cursor.getInt(cursor.getColumnIndex("uuid"));
                Log.d(TAG,"display_name: "+display_name+"  uuid : "+uuid);
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }finally {
            if(cursor != null){
                cursor.close();
            }
        }
    }

    /**
     * 查询所有存储卷
     * */
    public void queryVolume(long queryUuid){
        Uri uri = Uri.parse("content://media/");
        Cursor cursor = null;
        try {
            cursor = mContentProviderClient.query(uri,null,"uuid = ?",new String[]{"queryUuid"},null);
            while (cursor!= null && cursor.moveToNext()){//可以按照表结构获取数据
                String display_name = cursor.getString(cursor.getColumnIndex("display_name"));
                int  uuid = cursor.getInt(cursor.getColumnIndex("uuid"));
                Log.d(TAG,"display_name: "+display_name+"  uuid : "+uuid);
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }finally {
            if(cursor != null){
                cursor.close();
            }
        }
    }


    public void queryAllAudio(){
        Cursor cursor = null;
        try {
            //获取标题 时长 专辑  艺术家 信息
            String[] projection = new String[]{
                    MediaStore.Audio.AudioColumns._ID,
                    MediaStore.Audio.AudioColumns.TITLE,
                    MediaStore.Audio.AudioColumns.DURATION,
                    MediaStore.Audio.AudioColumns.ALBUM,
                    MediaStore.Audio.AudioColumns.ARTIST,
            };
            cursor = mContentProviderClient.query(MediaStore.Audio.Media.INTERNAL_CONTENT_URI,projection,null,null,null);
            while (cursor != null && cursor.moveToNext()){
                StringBuilder builder = new StringBuilder();
                builder.append("initAudio  ============== ");
                long duration = cursor.getLong(cursor.getColumnIndex(MediaStore.Audio.AudioColumns.DURATION));
                builder.append("duration = ").append(duration);
                String title = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.AudioColumns.TITLE));
                builder.append("   title = ").append(title);
                String album = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.AudioColumns.ALBUM));
                builder.append("   album = ").append(album);
                String artist = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.AudioColumns.ALBUM));
                builder.append("   artist = ").append(artist);
                String id = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.AudioColumns._ID));
                builder.append("   id = ").append(id);
                Log.d(TAG,new String(builder));
            }
            if(cursor != null){
                cursor.close();
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }finally {
            if(cursor != null){
                cursor.close();
            }
        }
    }

    public void queryAudio(String audioId){
        Cursor cursor = null;
        try {
            //获取标题 时长 专辑  艺术家 信息
            String[] projection = new String[]{
                    MediaStore.Audio.AudioColumns._ID,
                    MediaStore.Audio.AudioColumns.TITLE,
                    MediaStore.Audio.AudioColumns.DURATION,
                    MediaStore.Audio.AudioColumns.ALBUM,
                    MediaStore.Audio.AudioColumns.ARTIST,
            };
            Uri uri = Uri.parse("content://media/internal/audio/media/"+audioId);
            cursor = mContentProviderClient.query(uri,projection,null,null,null);
            while (cursor != null && cursor.moveToNext()){
                StringBuilder builder = new StringBuilder();
                builder.append("initAudio  ============== ");
                long duration = cursor.getLong(cursor.getColumnIndex(MediaStore.Audio.AudioColumns.DURATION));
                builder.append("duration = ").append(duration);
                String title = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.AudioColumns.TITLE));
                builder.append("   title = ").append(title);
                String album = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.AudioColumns.ALBUM));
                builder.append("   album = ").append(album);
                String artist = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.AudioColumns.ALBUM));
                builder.append("   artist = ").append(artist);
                String id = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.AudioColumns._ID));
                builder.append("   id = ").append(id);
                Log.d(TAG,new String(builder));
            }
            if(cursor != null){
                cursor.close();
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }finally {
            if(cursor != null){
                cursor.close();
            }
        }
    }

    private void queryAllVideo() {
//        Cursor cursor = MediaStore.Video.query(getContentResolver(),MediaStore.Video.Media.INTERNAL_CONTENT_URI,null);
        Cursor cursor = null;
        try {
            cursor = mContentProviderClient.query(MediaStore.Video.Media.INTERNAL_CONTENT_URI,null,null,null,null);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        Log.d(TAG,"initVideo start");
        while (cursor != null && cursor.moveToNext()){
            StringBuilder builder = new StringBuilder();
            builder.append("initVideo  ============== ");
            long duration = cursor.getLong(cursor.getColumnIndex(MediaStore.Video.Media.DURATION));
            builder.append("duration = ").append(duration);
            String name = cursor.getString(cursor.getColumnIndex(MediaStore.Video.Media.DISPLAY_NAME));
            builder.append("   name = ").append(name);
            String data = cursor.getString(cursor.getColumnIndex(MediaStore.Video.Media.DATA));
            builder.append("   data = ").append(data);
            Log.d(TAG,new String(builder));
        }
        if(cursor != null){
            cursor.close();
        }
        Log.d(TAG,"initVideo end");
    }

    private void queryVideo(String videoId) {
        Uri uri = Uri.parse("content://media/internal/video/media/"+videoId);
        Cursor cursor = null;
        try {
            cursor = mContentProviderClient.query(uri,null,null,null,null);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        Log.d(TAG,"initVideo start");
        while (cursor != null && cursor.moveToNext()){
            StringBuilder builder = new StringBuilder();
            builder.append("initVideo  ============== ");
            long duration = cursor.getLong(cursor.getColumnIndex(MediaStore.Video.Media.DURATION));
            builder.append("duration = ").append(duration);
            String name = cursor.getString(cursor.getColumnIndex(MediaStore.Video.Media.DISPLAY_NAME));
            builder.append("   name = ").append(name);
            String data = cursor.getString(cursor.getColumnIndex(MediaStore.Video.Media.DATA));
            builder.append("   data = ").append(data);
            Log.d(TAG,new String(builder));
        }
        if(cursor != null){
            cursor.close();
        }
        Log.d(TAG,"initVideo end");
    }

}
