package com.example.txl.tool.mediaprovider;

import android.content.ContentProviderClient;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.RemoteException;
import android.provider.MediaStore;

import com.example.txl.tool.mediaprovider.Interactive.DefaultAudioBrowser;
import com.example.txl.tool.mediaprovider.Interactive.DefaultImageBrowser;
import com.example.txl.tool.mediaprovider.Interactive.DefaultVideoBrowser;
import com.example.txl.tool.mediaprovider.Interactive.IQueryStrategy;

import java.util.List;


public class MediaProviderHelper implements IQueryStrategy{
    /**
     * 查询类型
     * */
    private int queryType = MediaStore.Files.FileColumns.MEDIA_TYPE_AUDIO;
    private IQueryStrategy queryStrategy;


    public void updateQueryType(int queryType){
        this.queryType = queryType;
        queryStrategy = getQueryStrategy(queryType);
    }


    private IQueryStrategy getQueryStrategy(int queryType){
        switch (queryType){
            case MediaStore.Files.FileColumns.MEDIA_TYPE_AUDIO:
                return new DefaultAudioBrowser(mContentProviderClient);
            case MediaStore.Files.FileColumns.MEDIA_TYPE_VIDEO:
                return new DefaultVideoBrowser(mContentProviderClient);
            case MediaStore.Files.FileColumns.MEDIA_TYPE_IMAGE:
                return new DefaultImageBrowser(mContentProviderClient);
            default:
                throw new IllegalArgumentException("not support queryType "+queryType);
        }
    }

    private final String TAG = getClass().getSimpleName();
    ContentProviderClient mContentProviderClient;
    public void init(Context context){
        mContentProviderClient = context.getContentResolver().acquireContentProviderClient(MediaStore.AUTHORITY);
        queryStrategy = getQueryStrategy(queryType);
    }


    public void release(){
        if(mContentProviderClient != null){
            ContentProviderClient temp = mContentProviderClient;
            mContentProviderClient = null;
            temp.release();
        }
    }

    @Override
    public List<MediaInfo> queryAllDir() {
        return queryStrategy.queryAllDir();
    }

    @Override
    public List<MediaInfo> queryAllFile() {
        return queryStrategy.queryAllFile();
    }

    @Override
    public List<MediaInfo> queryAllFileByPid(long pid) {
        return queryStrategy.queryAllFileByPid(pid);
    }

    public Cursor queryFileInfo(int fileId){
        return this.queryFileInfo(mContentProviderClient,fileId);
    }

    private void test() {
        //插入文件的播放状态
//        Uri uri = android.net.Uri.parse("content://hs_media/external/file");
//        ContentValues values = new ContentValues();
//        values.put("play_error_code",1);
//        try {
//            //_id 为文件的id
//            mContentProviderClient.update(uri,values,"where _id = ?",new String[]{"_id"});
//        } catch (RemoteException e) {
//            e.printStackTrace();
//        }

//        //插入视频缩略图
//        Uri uri = Uri.parse("content://hs_media/external/video/thumbnails");
//        ContentValues values = new ContentValues();
//        //path 图片的路径
//        values.put("_data","path");
//        //原来的文件id
//        values.put("video_id","_id");
//        //图片的宽高
//        values.put("width",50);
//        values.put("height",50);
//        try {
//            mContentProviderClient.insert(uri,values);
//        } catch (RemoteException e) {
//            e.printStackTrace();
//        }

        //播放模式  如果还有其它的字段可以协商增加
//        Uri uri = android.net.Uri.parse("content://hs_media/external/player");
//        ContentValues values = new ContentValues();
//        //播放器类型  音频、视频  需要调用端自己对应音频、视频播放器的类型  比如0代表视频，1代表音频
//        values.put("player_type",0);
//        //播放模式 单曲循环  文件夹循环 列表循环等
//        values.put("play_mode",0);
//        //播放速度
//        values.put("play_speed",1.5);
//        try {
//            mContentProviderClient.insert(uri,values);
//        } catch (RemoteException e) {
//            e.printStackTrace();
//        }

        //查询播放模式
        Uri uri = android.net.Uri.parse("content://hs_media/external/player");
        String select = "where player_type = ? ";
        String[] selectArgs = new String[]{"0"};
        try {
            Cursor cursor = mContentProviderClient.query(uri,null,select,selectArgs,null);
            cursor.getColumnCount();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }
}
