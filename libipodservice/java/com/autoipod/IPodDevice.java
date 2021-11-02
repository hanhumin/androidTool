package com.autoipod;

import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.support.v4.content.LocalBroadcastManager;
import android.text.TextUtils;
import android.util.Log;

import com.autoipod.data.MediaItem;
import com.autoipod.data.MediaLibraryInformation;
import com.autoipod.data.MediaTableItem;
import com.autoipod.data.MediaType;
import com.autoipod.jni.IIpodStart;
import com.autoipod.jni.IMediaLibraryInformation;
import com.autoipod.jni.Ipod;
import com.autoipod.utils.IPodDataBaseHelper;
import com.autoipod.utils.MediaLibraryUpdate;
import com.autoipod.utils.MediaLibraryUpdatePlaylistContents;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.List;

public class IPodDevice implements IIpodStart, IMediaLibraryInformation {

    private String TAG = "IPodDevice";
    public static final int IPOD_DEV_NOT_SYN = 3;
    public static final int IPOD_DEV_START_SYN = 1;
    public static final int IPOD_DEV_SYN_COMPLETE = 2;
    private IPodDataBaseHelper dbHelper;
    private int state = IPOD_DEV_NOT_SYN;
    Context mContext;

    public int getDeviceSynState() {
        return state;
    }

    public void setDeviceSynState(int state) {
        Log.d(TAG, "set ipo dev state " + state);
        makeIflyDic();
        this.state = state;
    }

    public IPodDevice(Context context, String uuid) {
       // Ipod.getInstance().startIpod();
	    mContext = context;
        dbHelper = new IPodDataBaseHelper(context, uuid, null, 1);
        Ipod.getInstance().registerIpodStartCallback(this);
		Ipod.getInstance().startIpod();
       // Ipod.getInstance().registerMediaLibraryInformationCallback(this);
       // Ipod.getInstance().registerMediaLibraryUpdateCallback(new MediaLibraryUpdate(dbHelper, this, context));
        //注册播放列表文件回调
       // Ipod.getInstance().registerMediaLibraryUpdatePlaylistContentsCallback(new MediaLibraryUpdatePlaylistContents(dbHelper));
        Log.d(TAG, "add IPO Device");
       
    }

    void startSyn() {

        Log.d(TAG, "startSyn: state " + state);
        if (state == IPOD_DEV_NOT_SYN)
            state = IPOD_DEV_START_SYN;
    }

    @Override
    public void OnIPodStartReady() {
        Log.d(TAG,"onIpodReady");
   Ipod.getInstance().registerMediaLibraryInformationCallback(this);
        Ipod.getInstance().registerMediaLibraryUpdateCallback(new MediaLibraryUpdate(dbHelper, this,mContext));
        //注册播放列表文件回调
        Ipod.getInstance().registerMediaLibraryUpdatePlaylistContentsCallback(new MediaLibraryUpdatePlaylistContents(dbHelper));
    
        Ipod.getInstance().iAP2StartMediaLibraryInformation();
        //更新当前播放信息
        Ipod.getInstance().iAP2StartNowPlayingUpdates();
    }

    @Override
    public void OnIPodStop() {
    }

    @Override
    public void OnMediaLibrary(ArrayList<MediaLibraryInformation> list) {
        Log.i(TAG, "start MediaUpdate size " + list.size());
        if (list != null && list.size() > 0) {
            dbHelper.setMediaLibraryCount(list.size());
            for (int i = 0; i < list.size(); i++) {
                MediaLibraryInformation info = list.get(i);
                Log.i(TAG, "MediaLibrary size=" + list.size());
                try {
                    String revision = dbHelper.getMedialibirayRevision(info.mediaLibraryUID);
                    if (revision != null) //原来数据中有revision
                    {
                        Log.i(TAG, "has revision");
                        Ipod.getInstance().iAP2StartMediaLibraryUpdates(
                                info.getMediaLibraryUID().getBytes().length, info.mediaLibraryUID.getBytes(), revision.getBytes().length, revision.getBytes());//-1 means no revi
                    } else//数据库没有revision
                    {
                        Log.i(TAG, "no revision");
                        Ipod.getInstance().iAP2StartMediaLibraryUpdates(
                                info.getMediaLibraryUID().getBytes().length, info.mediaLibraryUID.getBytes(), -1, null);//-1 means no revision
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public List<MediaTableItem> getMediaTables() {
        Cursor cursor = dbHelper.getReadableDatabase().rawQuery("select * from media_library where mediaitem_table!=? ", new String[]{"null"});
        List<MediaTableItem> tableName = new ArrayList<>();
        while (cursor.moveToNext()) {
            MediaTableItem item = new MediaTableItem();
            int index = cursor.getColumnIndex("mediaitem_table");
            item.setTableName(cursor.getString(index));
            item.setUid(cursor.getBlob(cursor.getColumnIndex("uid_blob")));
            tableName.add(item);
        }
        cursor.close();
        return tableName;
    }

    public MediaItem getMediaItemByPid(byte[] pid) {
        List<MediaTableItem> tables = getMediaTables();
        for (MediaTableItem item : tables) {
            MediaItem mediaItem = dbHelper.getMediaItem(item.getTableName(), dbHelper.converPidArrayToStr(pid));
            if (mediaItem != null)
                return mediaItem;
        }
        return null;
    }

    public List<String> getALLSongsTitle() {
        List<MediaTableItem> tables = getMediaTables();
        List<MediaItem> items = new ArrayList<>();
        List<String> listStr = new ArrayList<>();
        for (int i = 0; i < tables.size(); i++) {
            List<MediaItem> list = dbHelper.getMediaItemByType(tables.get(i).getTableName(), 2);
            if (list != null && list.size() > 0) {
                for (MediaItem mi : list) {
                    mi.setUid(tables.get(i).getUid());
                }
                items.addAll(list);
            }
        }

        for (int i = 0; i < items.size(); i++) {
            String title = items.get(i).getMediaItemTitle();
            if (title != null && !title.equals("NULL") && !listStr.contains(title))
                listStr.add(title);
        }
        return listStr;
    }

    public List<MediaItem> searchSons(String songs,String album,String genre,String artist)
    {
        List<MediaItem> itemsSongs = new ArrayList<>();
        List<MediaItem> itemsAlbum = new ArrayList<>();
        List<MediaItem> itemsGenre = new ArrayList<>();
        List<MediaItem> iTemsArtist = new ArrayList<>();

            List<MediaItem> list = getALLSongs();
            if(songs!=null)
            {
            if (list != null && list.size() > 0) {
                for (MediaItem mi : list) {

                    if (songs != null && mi.getMediaItemTitle()!= null && mi.getMediaItemTitle().contains(songs)) {
                        itemsSongs.add(mi);
                    }
                }
              }
           }
            else
            	{
            	
                itemsSongs.addAll(list);
			
            	}
           // }

                if(album!=null)
                {
                    if(itemsSongs.size() > 0)
                    {
                        for (int i = 0; i < itemsSongs.size(); i++) {
                            MediaItem item = itemsSongs.get(i);
                            if (item.getMediaItemAlbumTitle()!=null && item.getMediaItemAlbumTitle().contains(album))
                                itemsAlbum.add(item);
                        }
                    }
                }
                else
                	{
                    itemsAlbum.addAll(itemsSongs);
                	}
                if(genre!=null)
                {
                    if(itemsAlbum.size() > 0)
                    {
                        for (int i = 0; i < itemsAlbum.size(); i++) {
                            MediaItem item = itemsAlbum.get(i);
                            if (item.getMediaItemGenre()!=null && item.getMediaItemGenre().contains(genre))
                                itemsGenre.add(item);
                        }
                    }
                }
                else
                	{
                    itemsGenre.addAll(itemsAlbum);
                	}

                if(artist!=null)
                {
                    if(itemsGenre.size() > 0)
                    {
                        for (int i = 0; i < itemsGenre.size(); i++) {
                            MediaItem item = itemsGenre.get(i);
							{
                            if (item.getMediaItemArtist()!=null && item.getMediaItemArtist().contains(artist))
                            	{
                                iTemsArtist.add(item);
                            	}
                            }
                        }
                    }
                }
                else
                	{
                    iTemsArtist.addAll(itemsGenre);

            }


        return iTemsArtist;
    }

    public List<MediaItem> getALLSongs() {
        List<MediaTableItem> tables = getMediaTables();
        List<MediaItem> items = new ArrayList<>();
        for (int i = 0; i < tables.size(); i++) {
            List<MediaItem> list = dbHelper.getMediaItemByType(tables.get(i).getTableName(), 2);
            if (list != null && list.size() > 0) {
                for (MediaItem mi : list) {
                    mi.setUid(tables.get(i).getUid());
                }
                items.addAll(list);
            }
        }
        return items;
    }

    public List<String> getAllAlumb() {
        List<MediaTableItem> tables = getMediaTables();
        List<String> albumTitleList = new ArrayList<>();
        for (int i = 0; i < tables.size(); i++) {
            String sql = String.format("select album_tile from %s group by album_tile", tables.get(i).getTableName());
            Log.d("IPO", sql);
            Cursor c = dbHelper.getReadableDatabase().rawQuery(sql, null);
            while (c.moveToNext()) {
                // if (TextUtils.isEmpty(c.getString(0))||)
                //   artistList.add("无歌手");
                //   else {
                String str = c.getString(0);
                if (!str.equals("NULL") && !TextUtils.isEmpty(str) && !albumTitleList.contains(str))
                    albumTitleList.add(str);
                //}
            }
            c.close();
        }
        Log.d("IPO", albumTitleList.toString());
        return albumTitleList;
    }

    public List<String> getAllArtist() {
        List<MediaTableItem> tables = getMediaTables();
        List<String> artistList = new ArrayList<>();
        for (int i = 0; i < tables.size(); i++) {
            String sql = String.format("select artist from %s group by artist", tables.get(i).getTableName());
            Log.d("IPO", sql);
            Cursor c = dbHelper.getReadableDatabase().rawQuery(sql, null);
            while (c.moveToNext()) {
                // if (TextUtils.isEmpty(c.getString(0))||)
                //   artistList.add("无歌手");
                //   else {
                String str = c.getString(0);
                if (!str.equals("NULL") && !TextUtils.isEmpty(str) && !artistList.contains(str))
                    artistList.add(str);
                //}
            }
            c.close();
        }
        Log.d("IPO", artistList.toString());
        return artistList;
    }

    public List<String> getALLGennre() {
        List<MediaTableItem> tables = getMediaTables();
        List<String> genreList = new ArrayList<>();
        for (int i = 0; i < tables.size(); i++) {
            String sql = String.format("select genre from %s group by genre", tables.get(i).getTableName());
            Log.d("IPO", sql);
            Cursor c = dbHelper.getReadableDatabase().rawQuery(sql, null);
            while (c.moveToNext()) {
                if (TextUtils.isEmpty(c.getString(0)))
                    genreList.add("无流派");
                else {
                    genreList.add(c.getString(0));
                }
            }
            c.close();
        }
        Log.d("IPO", genreList.toString());
        return genreList;
    }

    public List<String> getAristByGennre(String gennre) {
        List<String> aristList = new ArrayList<>();
        List<MediaTableItem> tables = getMediaTables();

        for (int i = 0; i < tables.size(); i++) {
            String sql = String.format("select artist from %s where (1=%d and genre is null) or (1=%d and genre ='%s') group by artist", tables.get(i).getTableName(), gennre.equals("无流派") ? 1 : 0, gennre.equals("无流派") ? 0 : 1, gennre);
            Log.d("IPO", sql);
            Cursor cursor = dbHelper.getReadableDatabase().rawQuery(sql, null);
            while (cursor.moveToNext()) {
                aristList.add(cursor.getString(0));
            }
            cursor.close();
        }
        return aristList;
    }

    public List<String> getAlbumByArist(String arist) {
        List<String> albumList = new ArrayList<>();
        List<MediaTableItem> tables = getMediaTables();
        for (int i = 0; i < tables.size(); i++) {
            String sql = String.format("select album_tile from %s where artist like '%%%s%%'", tables.get(i).getTableName(), arist);
            Log.d("IPO", sql);
            Cursor cursor = dbHelper.getReadableDatabase().rawQuery(sql, null);
            while (cursor.moveToNext()) {
                albumList.add(cursor.getString(0));
            }
        }
        return albumList;
    }

    public List<MediaItem> getSongByAlbum(String album) {
        List<MediaItem> mediaItems = new ArrayList<>();
        List<MediaTableItem> tables = getMediaTables();
        for (int i = 0; i < tables.size(); i++) {
            String sql = String.format("select pid from %s where album_tile = '%s' and mediatype = 2", tables.get(i).getTableName(), album);
            Log.d("IPO", sql);
            Cursor cursor = dbHelper.getReadableDatabase().rawQuery(sql, null);
            while (cursor.moveToNext()) {
                MediaItem item = dbHelper.getMediaItem(tables.get(i).getTableName(), cursor.getString(0));
                if (item != null) {
                    item.setUid(tables.get(i).getUid());
                    mediaItems.add(item);
                }
            }
        }
        return mediaItems;
    }

    public List<MediaItem> getPodCast() {
        List<MediaItem> mediaItems = new ArrayList<>();
        List<MediaTableItem> tables = getMediaTables();
        for (int i = 0; i < tables.size(); i++) {
            List<MediaItem> list = dbHelper.getMediaItemByType(tables.get(i).getTableName(), 4);
            if (list != null && list.size() > 0) {
                for (MediaItem mi : list) {
                    mi.setUid(tables.get(i).getUid());
                }
                mediaItems.addAll(list);
            }
        }
        return mediaItems;
    }


    public List<MediaItem> getAudioBook() {
        List<MediaItem> mediaItems = new ArrayList<>();
        List<MediaTableItem> tables = getMediaTables();
        for (int i = 0; i < tables.size(); i++) {
            List<MediaItem> list = dbHelper.getMediaItemByType(tables.get(i).getTableName(), 8);
            if (list != null && list.size() > 0) {
                for (MediaItem mi : list) {
                    mi.setUid(tables.get(i).getUid());
                }
                mediaItems.addAll(list);
            }
        }
        return mediaItems;
    }

    public List<MediaType> getMediaTypes() {
        List<MediaTableItem> tables = getMediaTables();
        List<MediaType> mediaTypesList = new ArrayList<>();
        for (int i = 0; i < tables.size(); i++) {
            String sql = String.format("select mediatype,CASE mediatype when 2 then '歌曲' when 4 then '播客' when 8 then '有声读物'end as mediatypename from %s GROUP BY mediatype ", tables.get(i).getTableName());
            Log.d("IPO", sql);
            Cursor cursor = dbHelper.getReadableDatabase().rawQuery(sql, null);
            while (cursor.moveToNext()) {
                MediaType mediaType = new MediaType();
                mediaType.setMediaType(cursor.getInt(0));
                mediaType.setMediaTitle(cursor.getString(1));
                mediaTypesList.add(mediaType);
            }
        }


        return mediaTypesList;
    }


    public JSONArray makeJasionArray(List<String> list)
    {
         if(list == null || list.size() == 0)
             return null;
        JSONArray jsonArray = new JSONArray();
        for(int i = 0;i < list.size();i++) {
            JSONObject jsonObject = new JSONObject();
            try
            {
                jsonObject.put("id", i);
                jsonObject.put("name", list.get(i));
            } catch (Exception e)

            {
                e.printStackTrace();
            }
            jsonArray.put(jsonObject);
        }
        return  jsonArray;
}

    public void makeIflyDic()
    {
        {
            File file = new File(mContext.getExternalFilesDir(null), "singers");
            try {
                // make json
                JSONArray singersValue = makeJasionArray(getAllArtist());
                JSONObject singers = new JSONObject();
                singers.put("dictname", "singers");
                singers.put("dictcontant", singersValue);

                JSONArray values = new JSONArray();
                values.put(singers);

                JSONObject grm = new JSONObject();
                grm.put("grm", values);

                // write file
                FileOutputStream fos = new FileOutputStream(file);
                fos.write(grm.toString().getBytes());
                fos.flush();
                fos.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        {
            File file = new File(mContext.getExternalFilesDir(null), "albums");
            try {
                // make json
                JSONArray albumsValue =  makeJasionArray(getAllAlumb());
                JSONObject albums = new JSONObject();
                albums.put("dictname", "albums");
                albums.put("dictcontant", albumsValue);

                JSONArray values = new JSONArray();
                values.put(albums);

                JSONObject grm = new JSONObject();
                grm.put("grm", values);

                // write file
                FileOutputStream fos = new FileOutputStream(file);
                fos.write(grm.toString().getBytes());
                fos.flush();
                fos.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        {
            File file = new File(mContext.getExternalFilesDir(null), "genres");
            try {
                // make json
                JSONArray genresValue = makeJasionArray(getALLGennre());
                JSONObject genres = new JSONObject();
                genres.put("dictname", "genres");
                genres.put("dictcontant", genresValue);

                JSONArray values = new JSONArray();
                values.put(genres);

                JSONObject grm = new JSONObject();
                grm.put("grm", values);

                // write file
                FileOutputStream fos = new FileOutputStream(file);
                fos.write(grm.toString().getBytes());
                fos.flush();
                fos.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        {
            File file = new File(mContext.getExternalFilesDir(null), "songs");
            try {
                // make json
                JSONArray songsValue = makeJasionArray(getALLSongsTitle());
                JSONObject songs = new JSONObject();
                songs.put("dictname", "songs");
                songs.put("dictcontant", songsValue);

                JSONArray values = new JSONArray();
                values.put(songs);

                JSONObject grm = new JSONObject();
                grm.put("grm", values);

                String test = grm.toString();

                // write file
                FileOutputStream fos = new FileOutputStream(file);
                fos.write(grm.toString().getBytes());
                fos.flush();
                fos.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        LocalBroadcastManager lbm = LocalBroadcastManager.getInstance(mContext);
        Intent intent = new Intent("com.hsae.ipod.ifly_dic_ready");
       // intent.putExtra("provider_url","content://com.hsae.libmediaplayer.device");
        lbm.sendBroadcast(intent);
    }
}
