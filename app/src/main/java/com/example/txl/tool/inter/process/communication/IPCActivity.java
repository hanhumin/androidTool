package com.example.txl.tool.inter.process.communication;

import android.content.Context;
import android.content.Intent;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.example.txl.tool.R;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class IPCActivity extends AppCompatActivity {
    private static final String TAG = "IPCActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_ipc );
        initData();
    }

    private void initData(){
        Intent intent = getIntent();
        String msg = (String) intent.getCharSequenceExtra( "msg" );
        if(msg!= null && !"".equals( msg )){
            Toast.makeText( this,"我接收到了消息："+msg,Toast.LENGTH_SHORT ).show();
            return;
        }
        new Thread( new Runnable() {
            @Override
            public void run() {
                User user = null;
                File cacheFileDir = getDiskCacheDir(IPCActivity.this,"file_ipc");
                if(!cacheFileDir.exists()){
                    Log.d( TAG,"cacheFileDir 不存在" );
                    return;
                }
                File cacheFile = new File( cacheFileDir.getPath()+File.separator+"user1" );
                ObjectInputStream objectInputStream = null;
                try {
                    objectInputStream = new ObjectInputStream( new FileInputStream( cacheFile ) );
                    user = (User) objectInputStream.readObject();
                    Log.d( TAG,"recover user:" +user);
                    objectInputStream.close();
                    runOnUiThread( new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText( IPCActivity.this,"查找到文件共享",Toast.LENGTH_SHORT ).show();
                        }
                    } );
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (ClassNotFoundException e) {
                    e.printStackTrace();
                }
            }
        } ).start();
    }

    public File getDiskCacheDir(Context context, String uniqueName){
        boolean externalStorageAvailable = Environment.getExternalStorageState().equals( Environment.MEDIA_MOUNTED );
        final String cachePath;
        if(externalStorageAvailable){
            cachePath = context.getExternalCacheDir().getPath();
        }else {
            cachePath = context.getCacheDir().getPath();
        }
        return new File( cachePath+File.separator+uniqueName );
    }
}
