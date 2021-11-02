package com.autoipod.utils;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.support.annotation.Nullable;
import android.util.Log;

import com.autoipod.data.MediaItem;

import java.util.ArrayList;
import java.util.List;

public class IPodDataBaseHelper extends SQLiteOpenHelper {
  public static final String MEDIALIBIRAYR_TABLE = "media_library";

    public static final String MEDIALIBIRAYR_TABLE_COLUME_UID = "uid";
    public static final String MEDIALIBIRAYR_TABLE_COLUME_REVISION = "revision";
    public static final String MEDIALIBIRAYR_TABLE_COLUME_MEDIAITEM_TALBE = "mediaitem_table";
    public static final String MEDIALIBIRAYR_TABLE_COLUME_MEDIAPLAYLIST_TABLE = "mediaplaylist_table";
    public static final String MEDIALIBIRAYR_TABLE_COLUME_UPDATEPROGRESS = "updateprogress";
    public static final String MEDIALIBIRAYR_TABLE_COLUME_IS_HIDDEN = "ishiddengremoteitem";
    public static final String MEDIALIBIRAYR_TABLE_COLUME_PLAY_ALL_SONGS = "playallsongscapable";
	public static final String MEDIALIBIRAYR_TABLE_COLUME_UID_BLOB = "uid_blob";


    private Context mContext;
    public IPodDataBaseHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version) {
        super(context, name, factory, version);
        mContext = context;
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        if(!tabbleIsExist(db,MEDIALIBIRAYR_TABLE)) {
            String CREATE_TABLE = "create table "+MEDIALIBIRAYR_TABLE+
                    "(id integer primary key autoincrement,uid text,revision text,mediaitem_table text,mediaplaylist_table text,updateprogress integer," +
                    "ishiddengremoteitem boolean,playallsongscapable boolean,uid_blob blob);";
            db.execSQL(CREATE_TABLE);
        }
    }

    public String getMedialibirayRevision(String uid)
    {
        Cursor cursor = null;
        String revison = null;
        try {
       cursor = getWritableDatabase().query(MEDIALIBIRAYR_TABLE,null,"uid=?",new String []{uid},null,null,null);
       //cursor = getWritableDatabase().query(MEDIALIBIRAYR_TABLE,null,null,null,null,null,null);
        if(cursor !=null)
            Log.i("EEEEEEEEEEE","cursor.getCount()="+cursor.getCount());
        if(cursor !=null && cursor.getCount() > 0) {

            Log.i("EEEEEEEEEEE", "111");

            // if(cursor.getCount()>1)

            cursor.moveToFirst();

            int index = cursor.getColumnIndex("revision");
             revison = cursor.getString(index);
        }
        }catch (Exception e)
        {
            e.printStackTrace();
        }

        if(cursor!=null)
            cursor.close();
        return revison;
    }
    public String getMediaLibraryItemTableName(String uid)
    {
        String str = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(MEDIALIBIRAYR_TABLE,null,"uid=?",new String[]{uid},null,null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
           // if(cursor.getCount()>1)
            cursor.moveToFirst();
            int nameColumnIndex = cursor.getColumnIndex("mediaitem_table");
            str=cursor.getString(nameColumnIndex);
        }
        if(cursor!=null)
            cursor.close();
        return str;
    }
    public MediaItem  getMediaItemByComposer(String tablename,String ablums)
    {
        if(tablename == null && ablums == null)
            return null;
        // ContentValues values = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(tablename,null,"composer = ?", new String[]{ablums},
                null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
            // values = new ContentValues();
            //if(cursor.getCount()>1)
            cursor.moveToFirst();
            MediaItem item = new MediaItem();
            int index = -1;

            index = cursor.getColumnIndex("pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemPid(PidString.covertStrToByteArray(cursor.getString(index)));

            index = cursor.getColumnIndex("title");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemTitle(cursor.getString(index));
            index = cursor.getColumnIndex("mediatype");
            if(index!=-1)
                item.setMediaItemType(cursor.getInt(index));
            index = cursor.getColumnIndex("rating");
            if(index!=-1)
                item.setMediaItemRating(cursor.getInt(index));

            index = cursor.getColumnIndex("duration");
            if(index!=-1 )
                item.setMediaItemDuration(cursor.getInt(index));
            index = cursor.getColumnIndex("album_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemmAlbumPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("album_tile");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemAlbumTitle(cursor.getString(index));
            index = cursor.getColumnIndex("album_track_number");
            if(index!=-1)
                item.setMediaItemAlbumTrackNumber(cursor.getInt(index));
            index = cursor.getColumnIndex("album_track_count");
            if(index!=-1)
                item.setMediaItemAlbumTrackCount(cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_number");
            if(index!=-1 )
                item.setMediaItemAlbumDiscNumber(cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_count");
            if(index!=-1 )
                item.setMediaItemAlbumDiscCount(cursor.getInt(index));
            index = cursor.getColumnIndex("artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));

            index = cursor.getColumnIndex("artist");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtist(cursor.getString(index));
            index = cursor.getColumnIndex("album_artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("album_artist");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemAlbumArtist(cursor.getString(index));
            index = cursor.getColumnIndex("genre_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemGenrePid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("genre");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemGenre(cursor.getString(index));
            index = cursor.getColumnIndex("composer_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemComposerPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("composer");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemComposer(cursor.getString(index));

            index = cursor.getColumnIndex("is_part_compilation");
            if(index!=-1 )
                item.setIsPartOfCompilation((cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("is_like_support");
            if(index!=-1 )
                item.setLikeSupported((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("is_ban_supported");
            if(index!=-1 )
                item.setBanSupported((cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("islike");
            if(index!=-1 )
                item.setLiked((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("isbanned");
            if(index!=-1 )
                item.setBaned((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("isresident_on_device");
            if(index!=-1 )
                item.setResidentOnDevice((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("artwork_filetransfer_id");
            if(index!=-1 )
                item.setArtWorkFileTransferId(cursor.getInt(index));
            index = cursor.getColumnIndex("chapter_count");
            if(index!=-1 )
                item.setChapterCount(cursor.getInt(index));
            index = cursor.getColumnIndex("exist_attribute");
            if(index!=-1 )
                item.setExistAttribute(cursor.getInt(index));

            if(cursor!=null)
                cursor.close();
            return item;

        }
        if(cursor!=null)
            cursor.close();
        return null;
    }
    public MediaItem getMediaItemByGenre(String tablename,String ablums)
    {
        if(tablename == null && ablums == null)
            return null;
        // ContentValues values = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(tablename,null,"genre = ?", new String[]{ablums},
                null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
            // values = new ContentValues();
            //if(cursor.getCount()>1)
            cursor.moveToFirst();
            MediaItem item = new MediaItem();
            int index = -1;

            index = cursor.getColumnIndex("pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemPid(PidString.covertStrToByteArray(cursor.getString(index)));

            index = cursor.getColumnIndex("title");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemTitle(cursor.getString(index));
            index = cursor.getColumnIndex("mediatype");
            if(index!=-1)
                item.setMediaItemType(cursor.getInt(index));
            index = cursor.getColumnIndex("rating");
            if(index!=-1)
                item.setMediaItemRating(cursor.getInt(index));

            index = cursor.getColumnIndex("duration");
            if(index!=-1 )
                item.setMediaItemDuration(cursor.getInt(index));
            index = cursor.getColumnIndex("album_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemmAlbumPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("album_tile");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemAlbumTitle(cursor.getString(index));
            index = cursor.getColumnIndex("album_track_number");
            if(index!=-1)
                item.setMediaItemAlbumTrackNumber(cursor.getInt(index));
            index = cursor.getColumnIndex("album_track_count");
            if(index!=-1)
                item.setMediaItemAlbumTrackCount(cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_number");
            if(index!=-1 )
                item.setMediaItemAlbumDiscNumber(cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_count");
            if(index!=-1 )
                item.setMediaItemAlbumDiscCount(cursor.getInt(index));
            index = cursor.getColumnIndex("artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));

            index = cursor.getColumnIndex("artist");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtist(cursor.getString(index));
            index = cursor.getColumnIndex("album_artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("album_artist");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemAlbumArtist(cursor.getString(index));
            index = cursor.getColumnIndex("genre_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemGenrePid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("genre");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemGenre(cursor.getString(index));
            index = cursor.getColumnIndex("composer_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemComposerPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("composer");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemComposer(cursor.getString(index));

            index = cursor.getColumnIndex("is_part_compilation");
            if(index!=-1 )
                item.setIsPartOfCompilation((cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("is_like_support");
            if(index!=-1 )
                item.setLikeSupported((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("is_ban_supported");
            if(index!=-1 )
                item.setBanSupported((cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("islike");
            if(index!=-1 )
                item.setLiked((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("isbanned");
            if(index!=-1 )
                item.setBaned((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("isresident_on_device");
            if(index!=-1 )
                item.setResidentOnDevice((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("artwork_filetransfer_id");
            if(index!=-1 )
                item.setArtWorkFileTransferId(cursor.getInt(index));
            index = cursor.getColumnIndex("chapter_count");
            if(index!=-1 )
                item.setChapterCount(cursor.getInt(index));
            index = cursor.getColumnIndex("exist_attribute");
            if(index!=-1 )
                item.setExistAttribute(cursor.getInt(index));

            if(cursor!=null)
                cursor.close();
            return item;

        }
        if(cursor!=null)
            cursor.close();
        return null;
    }
    public MediaItem getMediaItemByAlbums(String tablename,String ablums)
    {
        if(tablename == null && ablums == null)
            return null;
        // ContentValues values = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(tablename,null,"album_tile = ?", new String[]{ablums},
                null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
            // values = new ContentValues();
            //if(cursor.getCount()>1)
            cursor.moveToFirst();
            MediaItem item = new MediaItem();
            int index = -1;

            index = cursor.getColumnIndex("pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemPid(PidString.covertStrToByteArray(cursor.getString(index)));

            index = cursor.getColumnIndex("title");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemTitle(cursor.getString(index));
            index = cursor.getColumnIndex("mediatype");
            if(index!=-1)
                item.setMediaItemType(cursor.getInt(index));
            index = cursor.getColumnIndex("rating");
            if(index!=-1)
                item.setMediaItemRating(cursor.getInt(index));

            index = cursor.getColumnIndex("duration");
            if(index!=-1 )
                item.setMediaItemDuration(cursor.getInt(index));
            index = cursor.getColumnIndex("album_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemmAlbumPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("album_tile");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemAlbumTitle(cursor.getString(index));
            index = cursor.getColumnIndex("album_track_number");
            if(index!=-1)
                item.setMediaItemAlbumTrackNumber(cursor.getInt(index));
            index = cursor.getColumnIndex("album_track_count");
            if(index!=-1)
                item.setMediaItemAlbumTrackCount(cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_number");
            if(index!=-1 )
                item.setMediaItemAlbumDiscNumber(cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_count");
            if(index!=-1 )
                item.setMediaItemAlbumDiscCount(cursor.getInt(index));
            index = cursor.getColumnIndex("artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));

            index = cursor.getColumnIndex("artist");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtist(cursor.getString(index));
            index = cursor.getColumnIndex("album_artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("album_artist");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemAlbumArtist(cursor.getString(index));
            index = cursor.getColumnIndex("genre_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemGenrePid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("genre");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemGenre(cursor.getString(index));
            index = cursor.getColumnIndex("composer_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemComposerPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("composer");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemComposer(cursor.getString(index));

            index = cursor.getColumnIndex("is_part_compilation");
            if(index!=-1 )
                item.setIsPartOfCompilation((cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("is_like_support");
            if(index!=-1 )
                item.setLikeSupported((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("is_ban_supported");
            if(index!=-1 )
                item.setBanSupported((cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("islike");
            if(index!=-1 )
                item.setLiked((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("isbanned");
            if(index!=-1 )
                item.setBaned((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("isresident_on_device");
            if(index!=-1 )
                item.setResidentOnDevice((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("artwork_filetransfer_id");
            if(index!=-1 )
                item.setArtWorkFileTransferId(cursor.getInt(index));
            index = cursor.getColumnIndex("chapter_count");
            if(index!=-1 )
                item.setChapterCount(cursor.getInt(index));
            index = cursor.getColumnIndex("exist_attribute");
            if(index!=-1 )
                item.setExistAttribute(cursor.getInt(index));

            if(cursor!=null)
                cursor.close();
            return item;

        }
        if(cursor!=null)
            cursor.close();
        return null;
    }
    public MediaItem getMediaItemByArtist(String tablename,String artist)
    {
        if(tablename == null && artist == null)
            return null;
        // ContentValues values = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(tablename,null,"artist = ?", new String[]{artist},
                null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
            // values = new ContentValues();
            //if(cursor.getCount()>1)
            cursor.moveToFirst();
            MediaItem item = new MediaItem();
            int index = -1;

            index = cursor.getColumnIndex("pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemPid(PidString.covertStrToByteArray(cursor.getString(index)));

            index = cursor.getColumnIndex("title");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemTitle(cursor.getString(index));
            index = cursor.getColumnIndex("mediatype");
            if(index!=-1)
                item.setMediaItemType(cursor.getInt(index));
            index = cursor.getColumnIndex("rating");
            if(index!=-1)
                item.setMediaItemRating(cursor.getInt(index));

            index = cursor.getColumnIndex("duration");
            if(index!=-1 )
                item.setMediaItemDuration(cursor.getInt(index));
            index = cursor.getColumnIndex("album_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemmAlbumPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("album_tile");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemAlbumTitle(cursor.getString(index));
            index = cursor.getColumnIndex("album_track_number");
            if(index!=-1)
                item.setMediaItemAlbumTrackNumber(cursor.getInt(index));
            index = cursor.getColumnIndex("album_track_count");
            if(index!=-1)
                item.setMediaItemAlbumTrackCount(cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_number");
            if(index!=-1 )
                item.setMediaItemAlbumDiscNumber(cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_count");
            if(index!=-1 )
                item.setMediaItemAlbumDiscCount(cursor.getInt(index));
            index = cursor.getColumnIndex("artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));

            index = cursor.getColumnIndex("artist");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtist(cursor.getString(index));
            index = cursor.getColumnIndex("album_artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("album_artist");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemAlbumArtist(cursor.getString(index));
            index = cursor.getColumnIndex("genre_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemGenrePid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("genre");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemGenre(cursor.getString(index));
            index = cursor.getColumnIndex("composer_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemComposerPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("composer");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemComposer(cursor.getString(index));

            index = cursor.getColumnIndex("is_part_compilation");
            if(index!=-1 )
                item.setIsPartOfCompilation((cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("is_like_support");
            if(index!=-1 )
                item.setLikeSupported((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("is_ban_supported");
            if(index!=-1 )
                item.setBanSupported((cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("islike");
            if(index!=-1 )
                item.setLiked((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("isbanned");
            if(index!=-1 )
                item.setBaned((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("isresident_on_device");
            if(index!=-1 )
                item.setResidentOnDevice((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("artwork_filetransfer_id");
            if(index!=-1 )
                item.setArtWorkFileTransferId(cursor.getInt(index));
            index = cursor.getColumnIndex("chapter_count");
            if(index!=-1 )
                item.setChapterCount(cursor.getInt(index));
            index = cursor.getColumnIndex("exist_attribute");
            if(index!=-1 )
                item.setExistAttribute(cursor.getInt(index));

            if(cursor!=null)
                cursor.close();
            return item;

        }
        if(cursor!=null)
            cursor.close();
        return null;
    }


    public List<MediaItem> getMediaItemByType(String tablename,int  mediaType)
    {
        if(tablename == null)
            return null;

        // ContentValues values = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(tablename,null,null, null,
                null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
            // values = new ContentValues();
            //if(cursor.getCount()>1)

            List<MediaItem> list = new ArrayList<>();
            cursor.moveToFirst();
            do {
                MediaItem item = new MediaItem();
                int index = -1;

                index = cursor.getColumnIndex("pid");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemPid(PidString.covertStrToByteArray(cursor.getString(index)));

                index = cursor.getColumnIndex("title");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemTitle(cursor.getString(index));
                index = cursor.getColumnIndex("mediatype");
                if (index != -1)
                    item.setMediaItemType(cursor.getInt(index));
                index = cursor.getColumnIndex("rating");
                if (index != -1)
                    item.setMediaItemRating(cursor.getInt(index));

                index = cursor.getColumnIndex("duration");
                if (index != -1)
                    item.setMediaItemDuration(cursor.getInt(index));
                index = cursor.getColumnIndex("album_pid");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemmAlbumPid(PidString.covertStrToByteArray(cursor.getString(index)));
                index = cursor.getColumnIndex("album_tile");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemAlbumTitle(cursor.getString(index));
                index = cursor.getColumnIndex("album_track_number");
                if (index != -1)
                    item.setMediaItemAlbumTrackNumber(cursor.getInt(index));
                index = cursor.getColumnIndex("album_track_count");
                if (index != -1)
                    item.setMediaItemAlbumTrackCount(cursor.getInt(index));
                index = cursor.getColumnIndex("album_disc_number");
                if (index != -1)
                    item.setMediaItemAlbumDiscNumber(cursor.getInt(index));
                index = cursor.getColumnIndex("album_disc_count");
                if (index != -1)
                    item.setMediaItemAlbumDiscCount(cursor.getInt(index));
                index = cursor.getColumnIndex("artist_pid");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));

                index = cursor.getColumnIndex("artist");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemArtist(cursor.getString(index));
                index = cursor.getColumnIndex("album_artist_pid");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));
                index = cursor.getColumnIndex("album_artist");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemAlbumArtist(cursor.getString(index));
                index = cursor.getColumnIndex("genre_pid");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemGenrePid(PidString.covertStrToByteArray(cursor.getString(index)));
                index = cursor.getColumnIndex("genre");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemGenre(cursor.getString(index));
                index = cursor.getColumnIndex("composer_pid");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemComposerPid(PidString.covertStrToByteArray(cursor.getString(index)));
                index = cursor.getColumnIndex("composer");
                if (index != -1 && cursor.getString(index) != null)
                    item.setMediaItemComposer(cursor.getString(index));

                index = cursor.getColumnIndex("is_part_compilation");
                if (index != -1)
                    item.setIsPartOfCompilation((cursor.getInt(index) == 1) ? true : false);
                index = cursor.getColumnIndex("is_like_support");
                if (index != -1)
                    item.setLikeSupported((cursor.getInt(index) == 1) ? true : false);

                index = cursor.getColumnIndex("is_ban_supported");
                if (index != -1)
                    item.setBanSupported((cursor.getInt(index) == 1) ? true : false);
                index = cursor.getColumnIndex("islike");
                if (index != -1)
                    item.setLiked((cursor.getInt(index) == 1) ? true : false);

                index = cursor.getColumnIndex("isbanned");
                if (index != -1)
                    item.setBaned((cursor.getInt(index) == 1) ? true : false);

                index = cursor.getColumnIndex("isresident_on_device");
                if (index != -1)
                    item.setResidentOnDevice((cursor.getInt(index) == 1) ? true : false);

                index = cursor.getColumnIndex("artwork_filetransfer_id");
                if (index != -1)
                    item.setArtWorkFileTransferId(cursor.getInt(index));
                index = cursor.getColumnIndex("chapter_count");
                if (index != -1)
                    item.setChapterCount(cursor.getInt(index));
                index = cursor.getColumnIndex("exist_attribute");
                if (index != -1)
                    item.setExistAttribute(cursor.getInt(index));


                int type = item.getMediaItemType();
                if((type & mediaType)!=0)
                    list.add(item);
            }while (cursor.moveToNext());

            return list;

        }


        if(cursor!=null)
            cursor.close();
        return null;
    }
    public MediaItem getMediaItem(String tablename,String pid)
    {
        if(tablename == null && pid == null)
            return null;
       // ContentValues values = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(tablename,null,"pid = ?", new String[]{pid},
                null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
           // values = new ContentValues();
            //if(cursor.getCount()>1)
            cursor.moveToFirst();
            MediaItem item = new MediaItem();
            int index = -1;

            index = cursor.getColumnIndex("pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemPid(PidString.covertStrToByteArray(cursor.getString(index)));

            index = cursor.getColumnIndex("title");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemTitle(cursor.getString(index));
            index = cursor.getColumnIndex("mediatype");
            if(index!=-1)
                item.setMediaItemType(cursor.getInt(index));
            index = cursor.getColumnIndex("rating");
            if(index!=-1)
                item.setMediaItemRating(cursor.getInt(index));

            index = cursor.getColumnIndex("duration");
            if(index!=-1 )
                item.setMediaItemDuration(cursor.getInt(index));
            index = cursor.getColumnIndex("album_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemmAlbumPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("album_tile");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemAlbumTitle(cursor.getString(index));
            index = cursor.getColumnIndex("album_track_number");
            if(index!=-1)
                item.setMediaItemAlbumTrackNumber(cursor.getInt(index));
            index = cursor.getColumnIndex("album_track_count");
            if(index!=-1)
                item.setMediaItemAlbumTrackCount(cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_number");
            if(index!=-1 )
                item.setMediaItemAlbumDiscNumber(cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_count");
            if(index!=-1 )
                item.setMediaItemAlbumDiscCount(cursor.getInt(index));
            index = cursor.getColumnIndex("artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));

            index = cursor.getColumnIndex("artist");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtist(cursor.getString(index));
            index = cursor.getColumnIndex("album_artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemArtistPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("album_artist");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemAlbumArtist(cursor.getString(index));
            index = cursor.getColumnIndex("genre_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemGenrePid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("genre");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemGenre(cursor.getString(index));
            index = cursor.getColumnIndex("composer_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemComposerPid(PidString.covertStrToByteArray(cursor.getString(index)));
            index = cursor.getColumnIndex("composer");
            if(index!=-1 && cursor.getString(index)!=null)
                item.setMediaItemComposer(cursor.getString(index));

            index = cursor.getColumnIndex("is_part_compilation");
            if(index!=-1 )
                item.setIsPartOfCompilation((cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("is_like_support");
            if(index!=-1 )
                item.setLikeSupported((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("is_ban_supported");
            if(index!=-1 )
                item.setBanSupported((cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("islike");
            if(index!=-1 )
                item.setLiked((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("isbanned");
            if(index!=-1 )
                item.setBaned((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("isresident_on_device");
            if(index!=-1 )
                item.setResidentOnDevice((cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("artwork_filetransfer_id");
            if(index!=-1 )
                item.setArtWorkFileTransferId(cursor.getInt(index));
            index = cursor.getColumnIndex("chapter_count");
            if(index!=-1 )
                item.setChapterCount(cursor.getInt(index));
            index = cursor.getColumnIndex("exist_attribute");
            if(index!=-1 )
                item.setExistAttribute(cursor.getInt(index));

            if(cursor!=null)
                cursor.close();
            return item;

        }
        if(cursor!=null)
            cursor.close();
        return null;
    }

    public ContentValues getMediaItemCursorBypid(String tablename,String pid)
    {
        if(tablename == null && pid == null)
            return null;
        ContentValues values = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(tablename,null,"pid = ?", new String[]{pid},
                null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
            values = new ContentValues();
            //if(cursor.getCount()>1)
              cursor.moveToFirst();
            int index = -1;

            index = cursor.getColumnIndex("pid");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("pid",cursor.getString(index));
            index = cursor.getColumnIndex("title");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("title",cursor.getString(index));
            index = cursor.getColumnIndex("mediatype");
            if(index!=-1)
                values.put("mediatype",cursor.getInt(index));
            index = cursor.getColumnIndex("rating");
            if(index!=-1 )
                values.put("rating",cursor.getInt(index));
            index = cursor.getColumnIndex("duration");
            if(index!=-1 )
                values.put("duration",cursor.getInt(index));
            index = cursor.getColumnIndex("album_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("album_pid",cursor.getString(index));
            index = cursor.getColumnIndex("album_tile");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("album_tile",cursor.getString(index));
            index = cursor.getColumnIndex("album_track_number");
            if(index!=-1)
                values.put("album_track_number",cursor.getInt(index));
            index = cursor.getColumnIndex("album_track_count");
            if(index!=-1)
                values.put("album_track_count",cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_number");
            if(index!=-1 )
                values.put("album_disc_number",cursor.getInt(index));
            index = cursor.getColumnIndex("album_disc_count");
            if(index!=-1 )
                values.put("album_disc_count",cursor.getInt(index));
            index = cursor.getColumnIndex("artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("artist_pid",cursor.getString(index));
            index = cursor.getColumnIndex("artist");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("artist",cursor.getString(index));
            index = cursor.getColumnIndex("album_artist_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("album_artist_pid",cursor.getString(index));
            index = cursor.getColumnIndex("album_artist");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("album_artist",cursor.getString(index));
            index = cursor.getColumnIndex("genre_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("genre_pid",cursor.getString(index));
            index = cursor.getColumnIndex("genre");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("genre",cursor.getString(index));
            index = cursor.getColumnIndex("composer_pid");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("composer_pid",cursor.getString(index));
            index = cursor.getColumnIndex("composer");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("composer",cursor.getString(index));

            index = cursor.getColumnIndex("is_part_compilation");
            if(index!=-1 )
                values.put("is_part_compilation",(cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("is_like_support");
            if(index!=-1 )
                values.put("is_like_support",(cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("is_ban_supported");
            if(index!=-1 )
                values.put("is_ban_supported",(cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("islike");
            if(index!=-1 )
                values.put("islike",(cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("isbanned");
            if(index!=-1 )
                values.put("isbanned",(cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("isresident_on_device");
            if(index!=-1 )
                values.put("isresident_on_device",(cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("artwork_filetransfer_id");
            if(index!=-1 )
                values.put("artwork_filetransfer_id",cursor.getInt(index));
            index = cursor.getColumnIndex("chapter_count");
            if(index!=-1 )
                values.put("chapter_count",cursor.getInt(index));
            index = cursor.getColumnIndex("exist_attribute");
            if(index!=-1 )
                values.put("exist_attribute",cursor.getInt(index));

        }
        if(cursor!=null)
            cursor.close();
        return  values;
    }

    public ContentValues getMediaLibraryCursorByUid(String uid)
    {
        if(uid == null )
            return null;
        ContentValues values = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(MEDIALIBIRAYR_TABLE,null,MEDIALIBIRAYR_TABLE_COLUME_UID + " = ?", new String[]{uid},
                null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
            values = new ContentValues();
           // if(cursor.getCount()>1)
            cursor.moveToFirst();
            int index = -1;
            index = cursor.getColumnIndex("id");
            if(index!=-1 ) {
                values.put("id", cursor.getInt(index));
                Log.i("getMediaLibraryCursorByUid","000   index="+index);
                Log.i("getMediaLibraryCursorByUid","000 id="+ cursor.getInt(index));

            }

            index = cursor.getColumnIndex("uid");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put(MEDIALIBIRAYR_TABLE_COLUME_UID,cursor.getString(index));

            index = cursor.getColumnIndex(MEDIALIBIRAYR_TABLE_COLUME_REVISION);
            if(index!=-1 && cursor.getString(index)!=null)
                values.put(MEDIALIBIRAYR_TABLE_COLUME_REVISION,cursor.getString(index));

            index = cursor.getColumnIndex(MEDIALIBIRAYR_TABLE_COLUME_MEDIAITEM_TALBE);
            if(index!=-1 && cursor.getString(index)!=null)
                values.put(MEDIALIBIRAYR_TABLE_COLUME_MEDIAITEM_TALBE,cursor.getString(index));

            index = cursor.getColumnIndex(MEDIALIBIRAYR_TABLE_COLUME_MEDIAPLAYLIST_TABLE);
            if(index!=-1 && cursor.getString(index)!=null)
                values.put(MEDIALIBIRAYR_TABLE_COLUME_MEDIAPLAYLIST_TABLE,cursor.getString(index));

            index = cursor.getColumnIndex(MEDIALIBIRAYR_TABLE_COLUME_UPDATEPROGRESS);
            if(index!=-1 )
                values.put(MEDIALIBIRAYR_TABLE_COLUME_UPDATEPROGRESS,cursor.getInt(index));

            index = cursor.getColumnIndex(MEDIALIBIRAYR_TABLE_COLUME_IS_HIDDEN);
            if(index!=-1 )
                values.put(MEDIALIBIRAYR_TABLE_COLUME_IS_HIDDEN,cursor.getInt(index) == 1?true:false);

            index = cursor.getColumnIndex(MEDIALIBIRAYR_TABLE_COLUME_PLAY_ALL_SONGS);
            if(index!=-1 )
                values.put(MEDIALIBIRAYR_TABLE_COLUME_PLAY_ALL_SONGS,cursor.getInt(index) == 1?true:false);
            index = cursor.getColumnIndex(MEDIALIBIRAYR_TABLE_COLUME_UID_BLOB);
			 if(index!=-1 )
                values.put(MEDIALIBIRAYR_TABLE_COLUME_UID_BLOB,cursor.getBlob(index));


        }
        if(cursor!=null)
            cursor.close();
        return  values;
    }
    public int getMediaLibraryIdByUid(String uid)
    {
        int id = -1;
        SQLiteDatabase db = getWritableDatabase();
         Cursor cursor = db.query(MEDIALIBIRAYR_TABLE,null,"uid=?",new String[]{uid},null,null,null,null);
         if(cursor!=null && cursor.getCount() > 0)
         {
            // if(cursor.getCount()>1) {
                 cursor.moveToFirst();

            // }
             int nameColumnIndex = cursor.getColumnIndex("id");
             id=cursor.getInt(nameColumnIndex);
             Log.i("listview", "getMediaLibraryIdByUid  cursor.getcount="+cursor.getCount() );

             Log.i("listview", "getMediaLibraryIdByUid  id="+id );
         }
         if(cursor!=null)
             cursor.close();
        return id;
    }
    public String getMediaLibrarymediaplaylistTableName(String uid)
    {
        String str = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(MEDIALIBIRAYR_TABLE,null,"uid=?",new String[]{uid},null,null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
           // if(cursor.getCount()>1)
            cursor.moveToFirst();
            int nameColumnIndex = cursor.getColumnIndex("mediaplaylist_table");
            str=cursor.getString(nameColumnIndex);
        }
        if(cursor!=null)
            cursor.close();
        return str;
    }
    public void createMediaPlaylistContentsTable(String fileTransferId)
    {
        SQLiteDatabase db = getWritableDatabase();
        String tableName = "playlist_contents_"+fileTransferId;
        if(!tabbleIsExist(db,tableName)) {
           // media_item_pid blob primary key, file_id int, playlist_pid blob ,library_uid text
            String CREATE_TABLE = "create table "+tableName +
                    "(id integer primary key autoincrement,media_item_pid text,file_id int,playlist_pid text,library_uid text);";
            //   String CREATE_TABLE = "create table "+tableName +
            //        "(id integer primary key autoincrement,media_item_pid text,file_id int,playlist_pid text,library_uid text,media_item_pid text);";

            db.execSQL(CREATE_TABLE);
        }
    }
    public boolean isPlaylistExist(byte[] pid,String tableName)
    {
        boolean isExist = false;
        String str = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(tableName,null,"pid=?",new String[]{converPidArrayToStr(pid)},null,null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
            isExist = true;
        }
        if(cursor!=null)
            cursor.close();
        return isExist;
    }
    public ContentValues getMediaPlaylistCursorBypid(String tablename,String pid)
    {
        if(tablename == null || pid== null)
            return null;
        ContentValues values = null;
        SQLiteDatabase db = getWritableDatabase();
        Cursor cursor = db.query(tablename,null,"pid = ?", new String[]{pid},
                null,null,null);
        if(cursor!=null && cursor.getCount() > 0)
        {
            values = new ContentValues();
           // if(cursor.getCount()>1)
            cursor.moveToFirst();
            int index = -1;

            index = cursor.getColumnIndex("pid");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("pid",cursor.getString(index));
            index = cursor.getColumnIndex("name");
            if(index!=-1 && cursor.getString(index)!=null)
                values.put("name",cursor.getString(index));
            index = cursor.getColumnIndex("parent_pid");
            if(index!=-1 )
                values.put("parent_pid",(cursor.getInt(index)==1)?true:false);
            index = cursor.getColumnIndex("isfolder");
            if(index!=-1 )
                values.put("isfolder",(cursor.getInt(index)==1)?true:false);

            index = cursor.getColumnIndex("file_transfer_id");
            if(index!=-1 )
                values.put("file_transfer_id",cursor.getInt(index));

            index = cursor.getColumnIndex("is_radio_station");
            if(index!=-1 )
                values.put("is_radio_station",cursor.getInt(index));
            index = cursor.getColumnIndex("exist_attribute");
            if(index!=-1 )
                values.put("exist_attribute",cursor.getInt(index));

        }
        if(cursor!=null)
            cursor.close();
        return  values;
    }


    public void createMediaPlaylistTable(String MediaLibraryUID)
    {
        SQLiteDatabase db = getWritableDatabase();
        String tableName = "playlist_"+getMediaLibraryIdByUid(MediaLibraryUID);
        Log.i("listview", "createMediaPlaylistTable playlist tableName="+tableName );
        if(!tabbleIsExist(db,tableName)) {
            String CREATE_TABLE = "create table "+tableName +
                    "(pid text primary key,name text,parent_pid text,genius_mix boolean,isfolder boolean,file_transfer_id int,is_radio_station boolean,"+
                    "exist_attribute integer);";
            db.execSQL(CREATE_TABLE);

        }
        ContentValues values = getMediaLibraryCursorByUid(MediaLibraryUID);
        // ContentValues values = new ContentValues();
        Log.i("listview", "createMediaPlaylistTable MediaLibraryUID=" +MediaLibraryUID);
        values.put("mediaplaylist_table", tableName);
        db.update(MEDIALIBIRAYR_TABLE, values, "uid = ?", new String[] { MediaLibraryUID });

    }

    public void  deletePlaylistTable(String tableName)
    {

        if(!tabbleIsExist(getWritableDatabase(),tableName))
            return;
        Cursor cursor = null;
        SQLiteDatabase db = getWritableDatabase();
        try{
            cursor = db.query(tableName,new String[]{"file_transfer_id"},null,null,null,null,null);
            if(cursor!=null && cursor.getCount() > 0)
            {
               // if(cursor.getCount()>1)
                    cursor.moveToFirst();
                do{
                    int index = cursor.getColumnIndex("file_transfer_id");
                    if(index != -1 )
                    {
                        int id = cursor.getInt(index);
                        String playlist_contents_tableName = "playlist_contents_"+id;
                        if(tabbleIsExist(db,playlist_contents_tableName))
                            db.execSQL("drop table "+playlist_contents_tableName);
                    }

                }while (cursor.moveToNext());
            }
        }catch (Exception e)
        {
            e.printStackTrace();
        }
        if(cursor!=null)
         cursor.close();
        db.execSQL("drop table "+tableName);
    }

   public boolean isMediaItemExist(byte[] pid,String tableName)
   {
       boolean isExist = false;
       String str = null;
       SQLiteDatabase db = getWritableDatabase();
       Cursor cursor = db.query(tableName,null,"pid=?",new String[]{converPidArrayToStr(pid)},null,null,null,null);
       if(cursor!=null && cursor.getCount() > 0)
       {
           isExist = true;
       }
       if(cursor!=null)
           cursor.close();
       return isExist;
   }

   public boolean isMediaItemExist(String tableName ,String pid)
   {

    //   Log.i("XXX","isMediaItemExist pid= "+pid);
       boolean isExist = false;
       String str = null;
       SQLiteDatabase db = getWritableDatabase();
       Cursor cursor = db.query(tableName,null,"pid=?",new String[]{pid},null,null,null,null);
       if(cursor!=null && cursor.getCount() > 0)
       {
           isExist = true;
       }
       if(cursor!=null)
           cursor.close();
       return isExist;
   }
    public void createMediaItemTable(String MediaLibraryUID)
    {
        SQLiteDatabase db = getWritableDatabase();
         String tableName = "media_item_"+getMediaLibraryIdByUid(MediaLibraryUID);//MediaLibraryUID;
        if(!tabbleIsExist(db,tableName)) {
            String CREATE_TABLE = "create table "+tableName +
                    "(pid text primary key,title text,mediatype integer,rating integer,duration integer,album_pid text,album_tile text," +
                    "album_track_number integer,album_track_count integer,album_disc_number integer,album_disc_count integer,artist_pid text,artist text," +
                    "album_artist_pid text,album_artist text,genre_pid text,genre text,composer_pid text,composer text,is_part_compilation boolean,is_like_support boolean," +
                    "is_ban_supported boolean,islike boolean,isbanned boolean,isresident_on_device boolean,artwork_filetransfer_id integer,chapter_count integer,exist_attribute integer);";
          //  Log.i("#############################","##################################");
        //    Log.i("XXxxx1",MediaLibraryUID);
        //    Log.i("XXxxx2",tableName);
        //    Log.i("XX",CREATE_TABLE);
        //    Log.i("#############################","##################################");
            db.execSQL(CREATE_TABLE);
        }
        ContentValues values = getMediaLibraryCursorByUid(MediaLibraryUID);
        //  ContentValues values = new ContentValues();
        values.put("mediaitem_table", tableName);
        db.update(MEDIALIBIRAYR_TABLE, values, "uid = ?", new String[] { MediaLibraryUID });
    }


    public boolean tabbleIsExist(SQLiteDatabase db, String tableName){
        boolean result = false;
        if(tableName == null){
            return false;
        }
        Cursor cursor = null;
        try {
            String sql = "select count(*) as c from Sqlite_master  where type ='table' and name ='"+tableName.trim()+"' ";
            cursor = db.rawQuery(sql, null);
            if(cursor.moveToNext()){
                int count = cursor.getInt(0);
                if(count>0){
                    result = true;
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
            // TODO: handle exception
        }
        if(cursor!=null)
            cursor.close();
        return result;
    }
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

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

    public byte[] covertStrToByteArray(String str)
    {
        if(!str.startsWith("0x"))
            return null;
        String[] byteStrArray = str.split("0x");
        if(byteStrArray==null || byteStrArray.length == 0)
            return null;
        byte[] byteArray = new byte[byteStrArray.length-1];
        for(int j=1;j<byteStrArray.length;j++) {
            byteArray[j-1] = Integer.valueOf(byteStrArray[j], 16).byteValue();
            // Log.i("ppp","converByteToStr[j]="+converByteToStr(byteArray[j-1] ));

        }
        return byteArray;
    }

    int count;

    public int getMediaLibraryCount() {
        return count;
    }

    public void setMediaLibraryCount(int count)
    {
        this.count = count;
    }
}
