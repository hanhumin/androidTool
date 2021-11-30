package com.example.txl.tool.mediaprovider;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ContentProviderClient;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.RemoteException;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.txl.tool.R;

public class MediaProviderActivity extends AppCompatActivity implements View.OnClickListener {
    private final String TAG = MediaProviderActivity.class.getSimpleName();

    private ContentProviderClient mMediaProvider;
    private TextView tvVideo, tvAudio, tvPic;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media_providerctivity2);
        mMediaProvider = getContentResolver()
                .acquireContentProviderClient(MediaStore.AUTHORITY);
//                .acquireContentProviderClient("hase_media");
        initView();
    }

    private void initView() {
        tvVideo = findViewById(R.id.tvVideo);
        tvAudio = findViewById(R.id.tvAudio);
        tvPic = findViewById(R.id.tvPic);
        tvVideo.setOnClickListener(this);
        tvAudio.setOnClickListener(this);
        tvPic.setOnClickListener(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(mMediaProvider != null){
            mMediaProvider.release();
        }
    }

    @Override
    public void onClick(View v) {
        if(v == tvVideo){
            initVideo();
        }else if(v == tvAudio){
            initAudio();
        }else if(v == tvPic){
            initPic();
        }
    }

    private void initPic() {
        Cursor cursor = MediaStore.Video.query(getContentResolver(),MediaStore.Images.Media.INTERNAL_CONTENT_URI,null);
        Log.d(TAG,"initPic start");
        while (cursor != null && cursor.moveToNext()){
            StringBuilder builder = new StringBuilder();
            builder.append("initPic  ============== ");
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
        Log.d(TAG,"initPic end");
    }

    private void initAudio() {
        Cursor cursor = null;
        try {
            if(mMediaProvider == null){
                mMediaProvider = getContentResolver()
                        .acquireContentProviderClient(MediaStore.AUTHORITY);
            }
            if(mMediaProvider == null){
                Intent intent = new Intent("com.hase.test.lunchself");
                sendBroadcast(intent);
                return;
            }
            cursor = mMediaProvider.query(MediaStore.Audio.Media.INTERNAL_CONTENT_URI,null,null,null,"title_key desc");
//            cursor = mMediaProvider.query(uri,null,"children_type = ? or children_type = ? or children_type = ?",new String[]{"1","3","7"},"title_key desc");
//            cursor = mMediaProvider.query(MediaStore.Audio.Media.INTERNAL_CONTENT_URI,null,null,null,"duration desc limit 10 offset 0 ");
//            cursor = mMediaProvider.query(MediaStore.Audio.Media.INTERNAL_CONTENT_URI,null,"duration > ?  or  duration < ?",new String[]{"20000","3000"},"duration desc  ");
        } catch (RemoteException e) {
            e.printStackTrace();
        }
//        Cursor cursor = MediaStore.Video.query(getContentResolver(),MediaStore.Audio.Media.INTERNAL_CONTENT_URI,null);
        Log.d(TAG,"initAudio start");
        while (cursor != null && cursor.moveToNext()){
            StringBuilder builder = new StringBuilder();
            builder.append("initAudio  ============== ");
            long duration = cursor.getLong(cursor.getColumnIndex(MediaStore.Audio.Media.DURATION));
            builder.append("duration = ").append(duration);
            String name = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.Media.DISPLAY_NAME));
            builder.append("   name = ").append(name);
            String data = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.Media.DATA));
            builder.append("   data = ").append(data);
            String mime_type = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.Media.MIME_TYPE));
            builder.append("   mime_type = ").append(mime_type);
            String title_key = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.Media.TITLE_KEY));
            builder.append("   title_key = ").append(title_key);
//            String parent = cursor.getString(cursor.getColumnIndex("parent"));
//            builder.append("   parent = ").append(parent);
            Log.d(TAG,new String(builder));
        }
        if(cursor != null){
            cursor.close();
        }
        Log.d(TAG,"initAudio end");
    }

    private void initVideo() {
        Cursor cursor = MediaStore.Video.query(getContentResolver(),MediaStore.Video.Media.INTERNAL_CONTENT_URI,null);
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