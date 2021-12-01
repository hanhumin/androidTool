package com.example.txl.tool.glide;

import android.content.ContentResolver;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.media.MediaMetadataRetriever;
import android.net.Uri;
import android.os.ParcelFileDescriptor;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.bumptech.glide.Glide;
import com.bumptech.glide.Priority;
import com.bumptech.glide.load.DataSource;
import com.bumptech.glide.load.Options;
import com.bumptech.glide.load.data.DataFetcher;
import com.bumptech.glide.load.model.ModelLoader;
import com.bumptech.glide.load.model.ModelLoaderFactory;
import com.bumptech.glide.load.model.MultiModelLoaderFactory;
import com.bumptech.glide.signature.ObjectKey;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * 扩展ModelLoader  利用现在已有的解码变换功能
 * */

public class VideoAssetUriLoader implements ModelLoader<Uri, AssetFileDescriptor> {
    private static final String TAG = "VideoAssetUriLoader";
    private static final String ASSET_PATH_SEGMENT = "android_asset";
    private static final String ASSET_PREFIX =
            ContentResolver.SCHEME_FILE + ":///" + ASSET_PATH_SEGMENT + "/";
    private static final int ASSET_PREFIX_LENGTH = ASSET_PREFIX.length();

    private final AssetManager assetManager;

    //注意这个在Glide加载asset目录下的数据之前调用
    public static void init(Context context) {
        Glide.get(context).getRegistry().prepend(Uri.class, AssetFileDescriptor.class, new VideoModelLoaderFactory(context.getAssets()));
    }

    public VideoAssetUriLoader(AssetManager assetManager) {
        this.assetManager = assetManager;
    }

    @Nullable
    @Override
    public LoadData<AssetFileDescriptor> buildLoadData(@NonNull Uri model, int width, int height, @NonNull Options options) {
        String assetPath = model.toString().substring(ASSET_PREFIX_LENGTH);
        Log.d(TAG,"buildLoadData");
        return new LoadData<AssetFileDescriptor>(new ObjectKey(model), new AudioFetcher(assetManager,assetPath));
    }

    @Override
    public boolean handles(@NonNull Uri model) {
        boolean result = ContentResolver.SCHEME_FILE.equals(model.getScheme()) && !model.getPathSegments()
                .isEmpty() && ASSET_PATH_SEGMENT.equals(model.getPathSegments().get(0)) && model.getPathSegments().get(1).endsWith(".mp4");
        Log.d(TAG,"handles  result  "+result + "  model "+model.toString());
        return result;
    }


    private static class VideoModelLoaderFactory implements ModelLoaderFactory<Uri,AssetFileDescriptor>{
        private final AssetManager assetManager;

        public VideoModelLoaderFactory(AssetManager assetManager) {
            this.assetManager = assetManager;
        }

        @NonNull
        @Override
        public ModelLoader<Uri, AssetFileDescriptor> build(@NonNull MultiModelLoaderFactory multiFactory) {
            return new VideoAssetUriLoader(assetManager);
        }

        @Override
        public void teardown() {

        }
    }

    private static class AudioFetcher implements DataFetcher<AssetFileDescriptor>{
        private final AssetManager assetManager;
        private final String assetPath;

        private AudioFetcher(AssetManager assetManager, String assetPath) {
            this.assetManager = assetManager;
            this.assetPath = assetPath;
        }


        @Override
        public void loadData(@NonNull Priority priority, @NonNull DataCallback<? super AssetFileDescriptor> callback) {
            try {
                Log.d(TAG,"loadData assetPath "+assetPath);
                AssetFileDescriptor fileDescriptor = assetManager.openFd(assetPath);
                callback.onDataReady(fileDescriptor);
            } catch (IOException e) {
                e.printStackTrace();
                callback.onLoadFailed(e);
            }
        }

        @Override
        public void cleanup() {

        }

        @Override
        public void cancel() {

        }

        @NonNull
        @Override
        public Class<AssetFileDescriptor> getDataClass() {
            return AssetFileDescriptor.class;
        }

        @NonNull
        @Override
        public DataSource getDataSource() {
            return DataSource.LOCAL;
        }
    }



}
