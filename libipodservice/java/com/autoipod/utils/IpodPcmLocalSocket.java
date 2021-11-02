package com.autoipod.utils;


import android.content.Context;
import android.media.AudioFormat;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.util.Log;

import com.autoipod.AudioPlayer;
import com.autoipod.jni.Ipod;

import java.io.IOException;
import java.io.InputStream;

public class IpodPcmLocalSocket {
    public final static String TAG = "IpodLocalSocketService";
    public final static String ADDRESS = "loacalsoketautoipod";
    public final int[] SAMPLE_TABLE = new int[]{
      8000,11025,12000,16000,22050,24000,32000,44100,48000
    };

    LocalSocket localSocket;
    private LocalServerSocket localServerSocket;
    private int pcmBufferSize = 2048;
    private AudioPlayer mAudioPlayer;
    private int mAutioSamepleRate = 44100;
    private Context context;
    public IpodPcmLocalSocket(Context context) {
        this.context = context;
    }

    public void setmAutioSamepleRate(int samepleRate) {
        Log.d(TAG,"apple sample "+SAMPLE_TABLE[samepleRate]);
        mAutioSamepleRate = SAMPLE_TABLE[samepleRate];
    }

    public void startPcmSocket() {

        new Thread() {
            @Override
            public void run() {
                try {
                    LocalServerSocket server = new LocalServerSocket(ADDRESS);
                    if (server != null)
                        Log.i(TAG, "server!=null");
                    else
                        Log.i(TAG, "server==null");
                    while (true) {
                        // 这个方法是阻塞的,有Client连接上来的时候返回.
                        Log.i(TAG, "before server.accept();");
                        final LocalSocket receiver = server.accept();
                        Log.i(TAG, "after server.accept();");
                        Log.i(TAG,"sample "+mAutioSamepleRate);
                        new Thread() {
                            @Override
                            public void run() {
                                try {
                                    Log.i(TAG, "IpodPcmLocalSocket");
                                    InputStream input = receiver.getInputStream();
                                    // 这里是读取的代码
                                    int readed = 0;
                                    byte[] bytes = new byte[pcmBufferSize];
                                    // reading
                                    while (readed != -1) {
                                        // read next byte
                                        readed = input.read(bytes);
                                        if (readed > 0) {
                                            if (mAudioPlayer == null) {
                                                mAudioPlayer = new AudioPlayer(context);
                                                mAudioPlayer.startPlayer(mAutioSamepleRate, AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT);
                                            }
                                            mAudioPlayer.play(bytes, 0, readed);
                                        }
                                        //  Log.i(TAG, "server receive = " +new String(bytes, 0, readed));
                                    }
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
                            }
                        }.start();
                    }
                } catch (Exception e) {
                    Log.e(getClass().getName(), e.getMessage());
                }
            }
        }.start();
    }

    public AudioPlayer getAudioPlayer() {
        return mAudioPlayer;
    }
}
