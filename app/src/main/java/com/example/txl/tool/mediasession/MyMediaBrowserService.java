package com.example.txl.tool.mediasession;

import android.app.Service;
import android.content.Intent;
import android.media.browse.MediaBrowser;
import android.os.Bundle;
import android.os.IBinder;
import android.service.media.MediaBrowserService;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.List;

public class MyMediaBrowserService extends MediaBrowserService {
    public MyMediaBrowserService() {
    }

    @Nullable
    @Override
    public BrowserRoot onGetRoot(@NonNull String clientPackageName, int clientUid, @Nullable Bundle rootHints) {
        return new BrowserRoot("root",new Bundle());
    }

    @Override
    public void onLoadChildren(@NonNull String parentId, @NonNull Result<List<MediaBrowser.MediaItem>> result) {

    }

}