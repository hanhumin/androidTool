package com.autoipod.utils;

import android.util.Log;

import com.autoipod.data.MediaLibraryInformation;
import com.autoipod.jni.IMediaLibraryInformation;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.List;

import android.os.Handler;

public class MediaLibraryInformationImp implements IMediaLibraryInformation{
    final  static String TAG = "MediaLibraryInformation";
    private Handler mHandler;
    private List<MediaLibraryInformation> mMediaLibraryInformationsList;
    public static int MediaLibray_Msg = 1;


    public MediaLibraryInformationImp(Handler handler)
    {
        mHandler = handler;
    }


    @Override
    public void OnMediaLibrary(ArrayList<MediaLibraryInformation> list) {

        mMediaLibraryInformationsList = list;
        if(list!=null && list.size() > 0) {
            Log.i(TAG, "MediaLibraryInformationImp OnMediaLibrary list.size=%d" + list.size());
            for (int i = 0; i < mMediaLibraryInformationsList.size();i++)
            {
                Log.i(TAG, " OnMediaLibrary list["+i+"].mediaLibraryName=" + list.get(i).mediaLibraryName);
                Log.i(TAG, " OnMediaLibrary list["+i+"].mediaLibraryUID=" + list.get(i).mediaLibraryUID);
                Log.i(TAG, " OnMediaLibrary list["+i+"].mediaLibraryType=" + list.get(i).mediaLibraryType);
            }
        }
        mHandler.sendEmptyMessage(MediaLibray_Msg);
    }

    public List<MediaLibraryInformation> getMediaLibraryList()
    {
        return  mMediaLibraryInformationsList;
    }

    /*
    @Override
    public void OnMediaLibraryName(int len, byte[] utf8Data) {
        try {
            String str = new String(utf8Data, "utf-8");
            Log.i(TAG, "OnMediaLibraryName :" + str);
        }catch (Exception e){
            printErrorInfoFromException("OnMediaLibraryName",e);
        }
    }

    @Override
    public void OnMeidaLibraryUID(int len, byte[] utf8Data) {
        try {
            String str = new String(utf8Data, "utf-8");
            Log.i(TAG, "OnMeidaLibraryUID :" + str);
        }catch (Exception e){
            printErrorInfoFromException("OnMeidaLibraryUID",e);
        }
    }

    @Override
    public void OnMediaLibraryType(int type) {
        Log.i(TAG, "OnMediaLibraryType :" + type);
    }
*/
    public void printErrorInfoFromException(String tag,Exception e) {
        try {
            StringWriter sw = new StringWriter();
            PrintWriter pw = new PrintWriter(sw);
            e.printStackTrace(pw);
            Log.i(TAG,tag+": "+sw.toString());
            //  return "\r\n" + sw.toString() + "\r\n";
            sw.close();
            pw.close();
        } catch (Exception e2) {
            e2.printStackTrace();
            //  return "ErrorInfoFromException";
        }
    }

}
