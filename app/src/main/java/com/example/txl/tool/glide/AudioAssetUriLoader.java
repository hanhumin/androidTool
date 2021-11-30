package com.example.txl.tool.glide;

import android.content.ContentResolver;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.MediaMetadataRetriever;
import android.net.Uri;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.bumptech.glide.Glide;
import com.bumptech.glide.Priority;
import com.bumptech.glide.load.DataSource;
import com.bumptech.glide.load.Options;
import com.bumptech.glide.load.data.DataFetcher;
import com.bumptech.glide.load.model.AssetUriLoader;
import com.bumptech.glide.load.model.ModelLoader;
import com.bumptech.glide.load.model.ModelLoaderFactory;
import com.bumptech.glide.load.model.MultiModelLoaderFactory;
import com.bumptech.glide.signature.ObjectKey;
import com.example.txl.tool.App;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * 扩展ModelLoader  利用现在已有的解码变换功能
 * */

public class AudioAssetUriLoader implements ModelLoader<Uri,ByteBuffer> {
    private static final String TAG = "AudioAssetUriLoader";
    private static final String ASSET_PATH_SEGMENT = "android_asset";
    private static final String ASSET_PREFIX =
            ContentResolver.SCHEME_FILE + ":///" + ASSET_PATH_SEGMENT + "/";
    private static final int ASSET_PREFIX_LENGTH = ASSET_PREFIX.length();

    private final AssetManager assetManager;

    public static void init(Context context) {
        Glide.get(context).getRegistry().prepend(Uri.class, ByteBuffer.class, new AudioModelLoaderFactory(context.getAssets()));
    }

    public AudioAssetUriLoader(AssetManager assetManager) {
        this.assetManager = assetManager;
    }

    @Nullable
    @Override
    public LoadData<ByteBuffer> buildLoadData(@NonNull Uri model, int width, int height, @NonNull Options options) {
        String assetPath = model.toString().substring(ASSET_PREFIX_LENGTH);
        Log.d(TAG,"buildLoadData");
        return new LoadData<ByteBuffer>(new ObjectKey(model), new AudioFetcher(assetManager,assetPath));
    }

    @Override
    public boolean handles(@NonNull Uri model) {
        boolean result = ContentResolver.SCHEME_FILE.equals(model.getScheme()) && !model.getPathSegments()
                .isEmpty() && ASSET_PATH_SEGMENT.equals(model.getPathSegments().get(0)) && model.getPathSegments().get(1).endsWith(".mp3");
        Log.d(TAG,"handles  result  "+result + "  model "+model.toString());
        return result;
    }


    private static class AudioModelLoaderFactory implements ModelLoaderFactory<Uri,ByteBuffer>{
        private final AssetManager assetManager;

        public AudioModelLoaderFactory(AssetManager assetManager) {
            this.assetManager = assetManager;
        }

        @NonNull
        @Override
        public ModelLoader<Uri, ByteBuffer> build(@NonNull MultiModelLoaderFactory multiFactory) {
            return new AudioAssetUriLoader(assetManager);
        }

        @Override
        public void teardown() {

        }
    }

    private static class AudioFetcher implements DataFetcher<ByteBuffer>{
        private final AssetManager assetManager;
        private final String assetPath;

        MediaMetadataRetriever mediaMetadataRetriever = new MediaMetadataRetriever();
        private AudioFetcher(AssetManager assetManager, String assetPath) {
            this.assetManager = assetManager;
            this.assetPath = assetPath;
        }


        @Override
        public void loadData(@NonNull Priority priority, @NonNull DataCallback<? super ByteBuffer> callback) {
            try {
                Log.d(TAG,"loadData assetPath "+assetPath);
                AssetFileDescriptor fileDescriptor = assetManager.openFd(assetPath);
                mediaMetadataRetriever.setDataSource(fileDescriptor.getFileDescriptor(),fileDescriptor.getStartOffset(),fileDescriptor.getDeclaredLength());
                byte[] bytes = mediaMetadataRetriever.getEmbeddedPicture();
                ByteBuffer buf = ByteBuffer.wrap(bytes);
                callback.onDataReady(buf);
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
        public Class<ByteBuffer> getDataClass() {
            return ByteBuffer.class;
        }

        @NonNull
        @Override
        public DataSource getDataSource() {
            return DataSource.LOCAL;
        }
    }



}
