package com.example.txl.tool.mediaprovider;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.ContentProviderClient;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.RemoteException;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.bumptech.glide.Glide;
import com.bumptech.glide.load.DataSource;
import com.bumptech.glide.load.engine.GlideException;
import com.bumptech.glide.request.RequestListener;
import com.bumptech.glide.request.target.Target;
import com.example.txl.tool.R;
import com.example.txl.tool.glide.AudioAssetUriLoader;

import java.util.ArrayList;
import java.util.List;

public class MediaProviderActivity extends AppCompatActivity implements View.OnClickListener {
    private final String TAG = MediaProviderActivity.class.getSimpleName();

    private ContentProviderClient mMediaProvider;
    private TextView tvVideo, tvAudio, tvPic;
    private ImageView imageView;

    int temp =0 ;
    ArrayList<String> strings = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media_providerctivity2);
        mMediaProvider = getContentResolver()
                .acquireContentProviderClient(MediaStore.AUTHORITY);
//                .acquireContentProviderClient("hase_media");
        initView();
        AudioAssetUriLoader.init(this);
    }

    private void initView() {
        tvVideo = findViewById(R.id.tvVideo);
        tvAudio = findViewById(R.id.tvAudio);
        tvPic = findViewById(R.id.tvPic);
        imageView = findViewById(R.id.image);
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
        strings.clear();
    }

    @Override
    public void onClick(View v) {
        strings.clear();
        temp = 0;
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
            String name = cursor.getString(cursor.getColumnIndex(MediaStore.Video.Media.DISPLAY_NAME));
            builder.append("   name = ").append(name);
            String data = cursor.getString(cursor.getColumnIndex(MediaStore.Video.Media.DATA));
            builder.append("   data = ").append(data);
            Log.d(TAG,new String(builder));
            strings.add(data);
        }
        loadAudioImg();
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
            cursor = mMediaProvider.query(MediaStore.Audio.Media.INTERNAL_CONTENT_URI,null,null,null,"_data asc");//desc降序
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
            strings.add(data);
            Log.d(TAG,new String(builder));
        }
        loadAudioImg();
        if(cursor != null){
            cursor.close();
        }
        Log.d(TAG,"initAudio end");
    }

    Handler handler = new Handler(Looper.getMainLooper());

    private void loadAudioImg(){
        if(strings.isEmpty()){
            return;
        }
        if(temp >= strings.size()){
            temp = 0;
        }
        Glide.with(this).load(Uri.parse("file://"+strings.get(temp))).addListener(new RequestListener<Drawable>() {
            @Override
            public boolean onLoadFailed(@Nullable GlideException e, Object model, Target<Drawable> target, boolean isFirstResource) {
                temp++;
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        if(strings.isEmpty() || temp>=strings.size()){
                            return;
                        }
                        loadAudioImg();
                    }
                },20);
                return false;
            }

            @Override
            public boolean onResourceReady(Drawable resource, Object model, Target<Drawable> target, DataSource dataSource, boolean isFirstResource) {
                temp++;
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        if(strings.isEmpty() || temp>=strings.size()){
                            return;
                        }
                        loadAudioImg();
                    }
                },2*60*1000);
                return false;
            }
        }).into(imageView);
    }


    private void initVideo() {
        Cursor cursor = MediaStore.Video.query(getContentResolver(),MediaStore.Video.Media.INTERNAL_CONTENT_URI,null);
        Log.d(TAG,"initVideo start");
        int i =0 ;
        while (cursor != null && cursor.moveToNext()){
            StringBuilder builder = new StringBuilder();
            builder.append("initVideo  ============== ");
            long duration = cursor.getLong(cursor.getColumnIndex(MediaStore.Video.Media.DURATION));
            builder.append("duration = ").append(duration);
            String name = cursor.getString(cursor.getColumnIndex(MediaStore.Video.Media.DISPLAY_NAME));
            builder.append("   name = ").append(name);
            String data = cursor.getString(cursor.getColumnIndex(MediaStore.Video.Media.DATA));
            builder.append("   data = ").append(data);
            strings.add(data);
            Log.d(TAG,new String(builder));
        }
        loadAudioImg();
        if(cursor != null){
            cursor.close();
        }
        Log.d(TAG,"initVideo end");
    }
}