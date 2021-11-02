package com.autoipod;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.database.Cursor;
import android.net.Uri;

import java.util.List;

import static com.autoipod.IPodDevice.IPOD_DEV_NOT_SYN;

public class IFlyIpodContentProvider extends ContentProvider {

    public static final String AUTHORITY = "com.autoipod.IFlyIpodContentProvider";
    public static final String SINGERS_PATH = "singers";
    public static final String SONGS_PATH = "songs";
    public static final String ALBUMS_PATH = "albums";
    public static final String GENRES_PATH = "genres";
    public static final int SINGER_CODE = 1;//查询所有歌手
    public static final int SONGS_CODE = 2;//查询所有歌曲
    public static final int ALBUMS_CODE = 3;//查询所有专辑
    public static final int GENRE_CODE = 4;//查询所有风格
    private static final UriMatcher uriMatcher ;

    static {
        uriMatcher = new UriMatcher(UriMatcher.NO_MATCH);
        uriMatcher.addURI(AUTHORITY, SINGERS_PATH, SINGER_CODE);
        uriMatcher.addURI(AUTHORITY, SONGS_PATH, SONGS_CODE);
        uriMatcher.addURI(AUTHORITY, ALBUMS_PATH, ALBUMS_CODE);
        uriMatcher.addURI(AUTHORITY, GENRES_PATH, GENRE_CODE);
    }
    @Override
    public boolean onCreate() {
        return false;
    }

   public int getIpodSyncStatus()
   {
       IPodDevice device = IPodDeviceMannager.getActiveDevice();
       if(device!=null)
           device.getDeviceSynState();
       return IPOD_DEV_NOT_SYN;
   }

    @Override
    public Cursor query(Uri uri,  String[] projection,  String selection,  String[] selectionArgs,  String sortOrder) {
        List<String> list = null;
        IPodDevice device = IPodDeviceMannager.getActiveDevice();
        if(device == null)
            return null;
          switch (uriMatcher.match(uri)) {
              case SINGER_CODE:
                  list = device.getAllArtist();
                  break;
              case SONGS_CODE:
                  list = device.getALLSongsTitle();
                  break;
              case ALBUMS_CODE:
                  list = device.getAllAlumb();
                  break;
              case  GENRE_CODE:
                  list = device.getALLGennre();
                  break;
        }
        return null;
    }


    @Override
    public String getType(Uri uri) {
        return null;
    }


    @Override
    public Uri insert(Uri uri,  ContentValues values) {
        return null;
    }

    @Override
    public int delete( Uri uri,  String selection, String[] selectionArgs) {
        return 0;
    }

    @Override
    public int update( Uri uri,  ContentValues values,  String selection,  String[] selectionArgs) {
        return 0;
    }
}
