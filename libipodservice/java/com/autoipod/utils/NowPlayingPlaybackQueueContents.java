package com.autoipod.utils;

import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.autoipod.jni.INowPlayingPlaybackQueueContents;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.List;

public class NowPlayingPlaybackQueueContents implements INowPlayingPlaybackQueueContents {
    Handler mHandler;
    List<byte[]>  listItemPid = new ArrayList<>();
    @Override
    public void onStart(String fileName) {
        Log.i("NowPlayingPlaybackQueueContents","onStart filename = "+fileName);
    }

    @Override
    public void onFinish(String fileName) {
        listItemPid.clear();
        Log.i("NowPlayingPlaybackQueueContents","onFinish filename = "+fileName);
        try {
            File file = new File("/storage/emulated/0/Download/ipod/"+fileName);
            FileInputStream in = new FileInputStream(file);
            DataInputStream data_in = new DataInputStream(in);
            while(true) {
                byte[] itemBuf = new byte[8];
                int readLen = data_in.read(itemBuf, 0, 8);
                if (readLen == 8) {
                    listItemPid.add(itemBuf);
                } else
                    break;
            }
            file.deleteOnExit();
        }catch (Exception e)
        {
            e.printStackTrace();
        }
        Message msg = new Message();
        msg.what = 1;
        msg.obj = listItemPid;
        Log.i("NowPlayingPlaybackQueueContents","onFinish filename listItemPid.leng= "+listItemPid.size());
        mHandler.sendMessage(msg);

    }
}
