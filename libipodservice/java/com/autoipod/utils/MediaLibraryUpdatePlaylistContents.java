package com.autoipod.utils;

import android.content.ContentValues;
import com.autoipod.jni.IPlaylistContents;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;

public class MediaLibraryUpdatePlaylistContents implements IPlaylistContents {
    public final  static String TAG = "MediaLibraryUpdatePlaylistContents";
    IPodDataBaseHelper mIPodDataBaseHelper;

    public MediaLibraryUpdatePlaylistContents(IPodDataBaseHelper helper)
    {
        mIPodDataBaseHelper = helper;
    }

    @Override
    public void onStart(String fileName, int pidLen, byte[] pid, int uidLen, String mediaLibraryUid) {

    }

    @Override
    public void onFinish(String fileName, int pidLen, byte[] pid, int uidLen,String mediaLibraryUid) {
        int indexOf = fileName.indexOf(".mup") ;
        String idStr = fileName.substring(0,indexOf);
        int id = Integer.parseInt(idStr);
        mIPodDataBaseHelper.createMediaPlaylistContentsTable(idStr);
        try {
            File file = new File("/storage/emulated/0/Download/ipod/"+fileName);
            FileInputStream in = new FileInputStream(file);
            DataInputStream data_in = new DataInputStream(in);
            while(true) {
                byte[] itemBuf = new byte[8];
                int readLen = data_in.read(itemBuf, 0, 8);
                if (readLen == 8) {
                    ContentValues values = new ContentValues();
                        values.put("media_item_pid",converPidArrayToStr(itemBuf));
                        values.put("file_id",id);
                        values.put("playlist_pid",converPidArrayToStr(pid));
                        values.put("library_uid",mediaLibraryUid);
                    long result = mIPodDataBaseHelper.getWritableDatabase().insert("playlist_contents_"+idStr, null, values);
                } else
                    break;
            }
            file.deleteOnExit();
        }catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    public String converPidArrayToStr(byte b[])
    {
        String str = "";
        for(int i=0;i<b.length;i++)
            str = str+converByteToStr(b[i]);
        return str;
    }

    public String converByteToStr(byte b)
    {
        String str = Integer.toHexString(b);
        if(str.length() == 1)
            str = "0"+str;
        else if(str.length() > 2)
        {
            str = str.substring(str.length()-2,str.length());
        }
        str = "0x"+str;
        return str;
    }
}
