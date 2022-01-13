package com.example.txl.tool.mediaprovider;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.RecyclerView;

import android.content.ContentProviderClient;
import android.content.IntentFilter;
import android.database.Cursor;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.MediaStore;
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
import java.util.Stack;
import android.content.Intent;

public class MediaProviderActivity extends AppCompatActivity implements View.OnClickListener {
    private final String TAG = MediaProviderActivity.class.getSimpleName();

    private ContentProviderClient mMediaProvider;
    private TextView tvVideo, tvAudio, tvPic,tvBack;
    private ImageView imageView;

    int temp =0 ;
    ArrayList<String> strings = new ArrayList<>();
    private MediaProviderItemAdapter mediaProviderItemAdapter;
    private MediaProviderHelper mediaProviderHelper;
    private Stack<BackInfo> visitStack = new Stack<>();
    private String currentVisitName;
    private MediaProviderReceiver2 mediaProviderReceiver2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media_providerctivity2);
        mMediaProvider = getContentResolver()
                .acquireContentProviderClient(MediaStore.AUTHORITY);
//                .acquireContentProviderClient("hase_media");
        mediaProviderHelper = new MediaProviderHelper();
        mediaProviderHelper.init(this);
        initView();
        AudioAssetUriLoader.init(this);
        mediaProviderReceiver2 = new MediaProviderReceiver2();
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction("android.intent.action.MEDIA_MOUNTED");
        intentFilter.addDataScheme("file");
        intentFilter.addAction("android.intent.action.MEDIA_EJECT");
        intentFilter.addDataScheme("file");
        intentFilter.addAction("txl.tool.test.mediaprovider");

        /*
        *  <intent-filter android:priority="1000">
                <action android:name="android.intent.action.MEDIA_MOUNTED" />
                <data android:scheme="file" />
            </intent-filter>
            <intent-filter android:priority="1000">
                <action android:name="android.intent.action.MEDIA_EJECT" />

                <data android:scheme="file" />
            </intent-filter>
            <intent-filter>
                <action android:name="txl.tool.test.mediaprovider"/>
            </intent-filter>
        * */
        registerReceiver(mediaProviderReceiver2,intentFilter);
    }

    private void initView() {
        tvVideo = findViewById(R.id.tvVideo);
        tvAudio = findViewById(R.id.tvAudio);
        tvPic = findViewById(R.id.tvPic);
        imageView = findViewById(R.id.image);
        findViewById(R.id.tvFileBrowse).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendBroadcast(new Intent("com.hase.test.lunchself"));
                Object o  = v.getTag();
                boolean internal = false;
                if(o == null){
                    internal = true;
                }else {
                    internal = !(Boolean)o;
                }
                v.setTag(internal);
                StorageVolumeStrategy.updateInternal(internal);
                visitStack.clear();
                mediaProviderItemAdapter.setMediaInfos(mediaProviderHelper.queryAllDir());
            }
        });
        tvVideo.setOnClickListener(this);
        tvAudio.setOnClickListener(this);
        tvPic.setOnClickListener(this);
        tvBack = findViewById(R.id.tv_back);
        tvBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(visitStack.isEmpty()){
                    return;
                }
                BackInfo backInfo = visitStack.pop();
                if(backInfo != null){
                    mediaProviderItemAdapter.setMediaInfos(backInfo.mediaInfos);
                    tvBack.setText(backInfo.name);
                    currentVisitName = backInfo.name;
                }
            }
        });
        mediaProviderItemAdapter = new MediaProviderItemAdapter();
        mediaProviderItemAdapter.setOnItemClick(new MediaProviderItemViewHolder.OnItemClick() {
            @Override
            public void onItemClick(MediaInfo mediaInfo) {
                if(mediaInfo != null && mediaInfo.isDir()){
                    //通过栈来保存当前数据，在进入下一级
                    BackInfo last;
                    String name = currentVisitName+"/"+mediaInfo.getDisplayName();
                    last = new BackInfo(currentVisitName,mediaProviderItemAdapter.getMediaInfos());
                    tvBack.setText(name);
                    currentVisitName = name;
                    visitStack.push(last);
                    mediaProviderItemAdapter.setMediaInfos(mediaProviderHelper.queryAllFileByPid(mediaInfo.getId()));
                }else {
                    Cursor cursor = mediaProviderHelper.queryFileInfo((int) mediaInfo.getId());
                    if(cursor != null && cursor.moveToNext()){
                        StringBuilder builder = new StringBuilder();
                        String mediaType = cursor.getString(0);
                        builder.append("mediaType").append(mediaType).append("/n");
                        String mineType = cursor.getString(1);
                        builder.append("mineType").append(mineType).append("/n");
                        String title = cursor.getString(2);
                        builder.append("title").append(title).append("/n");
                        long duration = cursor.getLong(3);
                        builder.append("duration").append(duration).append("/n");

                        cursor.close();
                        AlertDialog dialog = new AlertDialog.Builder(MediaProviderActivity.this)
                                .setTitle("id3 信息")
                                .setMessage(new String(builder))
                                .create();
                        dialog.show();

                    }
                }
            }
        });
        RecyclerView recyclerView = findViewById(R.id.recycler_view);
        recyclerView.setAdapter(mediaProviderItemAdapter);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(mMediaProvider != null){
            mMediaProvider.release();
        }
        if(mediaProviderHelper != null){
            mediaProviderHelper.release();
        }
        if(mediaProviderReceiver2!=null){
            unregisterReceiver(mediaProviderReceiver2);
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
        currentVisitName = "图片";
        tvBack.setText(currentVisitName);
        visitStack.clear();
        mediaProviderHelper.updateQueryType(MediaStore.Files.FileColumns.MEDIA_TYPE_IMAGE);
        mediaProviderItemAdapter.setMediaInfos(mediaProviderHelper.queryAllDir());
    }

    private void initAudio() {
        currentVisitName = "音频";
        tvBack.setText(currentVisitName);
        visitStack.clear();
        mediaProviderHelper.updateQueryType(MediaStore.Files.FileColumns.MEDIA_TYPE_AUDIO);
        mediaProviderItemAdapter.setMediaInfos(mediaProviderHelper.queryAllDir());
    }

    private void initVideo() {
        currentVisitName = "视频";
        tvBack.setText(currentVisitName);
        visitStack.clear();
        mediaProviderHelper.updateQueryType(MediaStore.Files.FileColumns.MEDIA_TYPE_VIDEO);
        mediaProviderItemAdapter.setMediaInfos(mediaProviderHelper.queryAllDir());
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


   private static class BackInfo{
        String name;
        List<MediaInfo> mediaInfos;

       public BackInfo(String name, List<MediaInfo> mediaInfos) {
           this.name = name;
           this.mediaInfos = mediaInfos;
       }
   }
}