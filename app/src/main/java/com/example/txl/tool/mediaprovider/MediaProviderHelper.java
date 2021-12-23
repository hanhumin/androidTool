package com.example.txl.tool.mediaprovider;

import android.content.ContentProviderClient;
import android.content.Context;
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
}
