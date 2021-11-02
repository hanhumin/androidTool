package com.autoipod.utils;

import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.util.Log;
import android.support.v4.content.LocalBroadcastManager;

import com.autoipod.Constants;
import com.autoipod.IPodDevice;
import com.autoipod.data.MediaItem;
import com.autoipod.data.MediaPlaylist;
import com.autoipod.jni.IMediaLibraryUpdate;
import com.autoipod.jni.Ipod;

import java.io.PrintWriter;
import java.io.StringWriter;

import static com.autoipod.IPodDevice.IPOD_DEV_SYN_COMPLETE;
import static com.autoipod.utils.IPodDataBaseHelper.MEDIALIBIRAYR_TABLE;
import static com.autoipod.utils.IPodDataBaseHelper.MEDIALIBIRAYR_TABLE_COLUME_IS_HIDDEN;
import static com.autoipod.utils.IPodDataBaseHelper.MEDIALIBIRAYR_TABLE_COLUME_MEDIAITEM_TALBE;
import static com.autoipod.utils.IPodDataBaseHelper.MEDIALIBIRAYR_TABLE_COLUME_MEDIAPLAYLIST_TABLE;
import static com.autoipod.utils.IPodDataBaseHelper.MEDIALIBIRAYR_TABLE_COLUME_PLAY_ALL_SONGS;
import static com.autoipod.utils.IPodDataBaseHelper.MEDIALIBIRAYR_TABLE_COLUME_REVISION;
import static com.autoipod.utils.IPodDataBaseHelper.MEDIALIBIRAYR_TABLE_COLUME_UID;
import static com.autoipod.utils.IPodDataBaseHelper.MEDIALIBIRAYR_TABLE_COLUME_UID_BLOB;
import static com.autoipod.utils.IPodDataBaseHelper.MEDIALIBIRAYR_TABLE_COLUME_UPDATEPROGRESS;

public class MediaLibraryUpdate implements IMediaLibraryUpdate {
    final static String TAG = "MLU";
    private IPodDataBaseHelper mIPodDataBaseHelper;
    private IPodDevice iPodDevice;
    private int hasUpdateCount;
    private Context mContext;

    public MediaLibraryUpdate(IPodDataBaseHelper helper, IPodDevice iPodDevice,Context context) {
        mIPodDataBaseHelper = helper;
        this.iPodDevice = iPodDevice;
        mContext = context;

    }

    private boolean isExistUID(String uid) {
        Cursor cursor = null;
        try {
            cursor = mIPodDataBaseHelper.getWritableDatabase().query(MEDIALIBIRAYR_TABLE,
                    new String[]{MEDIALIBIRAYR_TABLE_COLUME_UID},
                    MEDIALIBIRAYR_TABLE_COLUME_UID + "=?",
                    new String[]{uid},
                    null,
                    null,
                    null);// 注意空格！

            if (cursor != null && cursor.getCount() > 0) {
                cursor.close();
                return true;
            }
            if (cursor != null)
                cursor.close();
            return false;
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (cursor != null)
            cursor.close();

        return false;
    }

    private void addNewMediaLibrartIndexInDb(String uid, byte[] byteArrayUid) {
        Cursor cursor = null;
        try {
            cursor = mIPodDataBaseHelper.getWritableDatabase().query(MEDIALIBIRAYR_TABLE, null, MEDIALIBIRAYR_TABLE_COLUME_UID + "=?", new String[]{uid},
                    null, null, null);
            if (cursor != null && cursor.getCount() > 0) {

                cursor.moveToFirst();
                int index = cursor.getColumnIndex("id");
                int id = cursor.getInt(index);
                Log.i(TAG, "00 id=" + id + " uid:" + uid);
            } else {
                ContentValues values = new ContentValues();
                values.put(MEDIALIBIRAYR_TABLE_COLUME_UID, uid);
                values.put(MEDIALIBIRAYR_TABLE_COLUME_REVISION, "null");
                values.put(MEDIALIBIRAYR_TABLE_COLUME_MEDIAITEM_TALBE, "null");
                values.put(MEDIALIBIRAYR_TABLE_COLUME_MEDIAPLAYLIST_TABLE, "null");
                values.put(MEDIALIBIRAYR_TABLE_COLUME_UPDATEPROGRESS, 0);
                values.put(MEDIALIBIRAYR_TABLE_COLUME_IS_HIDDEN, false);
                values.put(MEDIALIBIRAYR_TABLE_COLUME_PLAY_ALL_SONGS, false);
                values.put(MEDIALIBIRAYR_TABLE_COLUME_UID_BLOB, byteArrayUid);


                long result = mIPodDataBaseHelper.getWritableDatabase().insert(MEDIALIBIRAYR_TABLE, null, values);

                //for test
                if (cursor != null)
                    cursor.close();
                cursor = mIPodDataBaseHelper.getWritableDatabase().query(MEDIALIBIRAYR_TABLE, null, MEDIALIBIRAYR_TABLE_COLUME_UID + "=?", new String[]{uid},
                        null, null, null);
                cursor.moveToFirst();
                int index = cursor.getColumnIndex("id");
                int id = cursor.getInt(index);
                Log.i(TAG, " id=" + id + " uid:" + uid);
            }
            if (cursor != null)
                cursor.close();
            cursor = null;
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (cursor != null)
            cursor.close();
    }

    @Override
    public void OnMediaLibraryUpdateUID(int len, String uidStr, byte[] byteArryUid) {
        try {
            Log.i(TAG, "OnMediaLibraryUpdateUID  :" + uidStr);
            if (!isExistUID(uidStr)) {
                Log.i(TAG, "OnMediaLibraryUpdateUID !isExistUID  :" + uidStr);
                addNewMediaLibrartIndexInDb(uidStr, byteArryUid);
            } else
                Log.i(TAG, "OnMediaLibraryUpdateUID isExistUID  :" + uidStr);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    @Override
    public void OnMediaLibraryUpdateRevision(int len, String revision, int uidLen, String mediaLibraryUid, byte[] byteArryUid) {
        try {
            Log.d(TAG, "OnMediaLibraryUpdateRevision " + revision);
            if (!isExistUID(mediaLibraryUid)) {
                addNewMediaLibrartIndexInDb(mediaLibraryUid, byteArryUid);
            }
            ContentValues values = mIPodDataBaseHelper.getMediaLibraryCursorByUid(mediaLibraryUid);
            if (values == null)
                values = new ContentValues();
            values.put(MEDIALIBIRAYR_TABLE_COLUME_REVISION, revision);
            mIPodDataBaseHelper.getWritableDatabase()
                    .update(MEDIALIBIRAYR_TABLE, values, MEDIALIBIRAYR_TABLE_COLUME_UID + " = ?", new String[]{mediaLibraryUid});
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    @Override
    public void OnMediaItemDeleteUpdatePID(int datalen, byte[] dataItemPId, int uidLen, String libraryUidData) {
        Cursor cursor = null;
        try {
            cursor = mIPodDataBaseHelper.getWritableDatabase().query(MEDIALIBIRAYR_TABLE, new String[]{"mediaitem_table"}, "uid=?", new String[]{libraryUidData}, null, null, null);
            if (cursor != null && cursor.getCount() > 0) {
                cursor.moveToFirst();
                do {
                    int index = cursor.getColumnIndex("mediaitem_table");
                    if (index != -1) {
                        String mediaitem_table = cursor.getString(index);
                        if (mediaitem_table != null) {
                            if (mIPodDataBaseHelper.isMediaItemExist(dataItemPId, mediaitem_table))
                                mIPodDataBaseHelper.getWritableDatabase().delete(mediaitem_table, "pid=?", new String[]{PidString.converPidArrayToStr(dataItemPId)});
                        }
                    }
                } while (cursor.moveToNext());

            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (cursor != null)
            cursor.close();

    }


    @Override
    public void OnMediaPlayListDeleteUpdatePID(int datalen, byte[] dataPid, int uidLen, String libraryUidData) {
        Cursor cursor = null;
        try {
            cursor = mIPodDataBaseHelper.getWritableDatabase().query(MEDIALIBIRAYR_TABLE, new String[]{"mediaplaylist_table"}, "uid=?", new String[]{libraryUidData}, null, null, null);
            if (cursor != null && cursor.getCount() > 0) {
                cursor.moveToFirst();
                do {
                    int index = cursor.getColumnIndex("mediaplaylist_table");
                    if (index != -1) {
                        String playlitst_table = cursor.getString(index);
                        if (playlitst_table != null) {
                            mIPodDataBaseHelper.deletePlaylistTable(playlitst_table);
                        }
                    }
                } while (cursor.moveToNext());

            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (cursor != null)
            cursor.close();
    }

    @Override
    public void OnPlayAllSongsCapable(boolean capable, int datalen, String libraryUid, byte[] byteArryUid) {

        try {
            if (!isExistUID(libraryUid)) {
                addNewMediaLibrartIndexInDb(libraryUid, byteArryUid);
            }
            ContentValues values = mIPodDataBaseHelper.getMediaLibraryCursorByUid(libraryUid);
            if (values == null)
                values = new ContentValues();
            values.put(MEDIALIBIRAYR_TABLE_COLUME_PLAY_ALL_SONGS, capable);
            mIPodDataBaseHelper.getWritableDatabase()
                    .update(MEDIALIBIRAYR_TABLE, values, MEDIALIBIRAYR_TABLE_COLUME_UID + " = ?", new String[]{libraryUid});
        } catch (Exception e) {
            printErrorInfoFromException("OnMediaLibraryUpdateRevision", e);
        }
    }

    @Override
    public void OnIsHidingRemoteItems(boolean isHiding, int datalen, String libraryUid, byte[] byteArryUid) {
        try {
            if (!isExistUID(libraryUid)) {
                addNewMediaLibrartIndexInDb(libraryUid, byteArryUid);
            }
            ContentValues values = mIPodDataBaseHelper.getMediaLibraryCursorByUid(libraryUid);
            if (values == null)
                values = new ContentValues();
            values.put(MEDIALIBIRAYR_TABLE_COLUME_IS_HIDDEN, isHiding);
            mIPodDataBaseHelper.getWritableDatabase()
                    .update(MEDIALIBIRAYR_TABLE, values, MEDIALIBIRAYR_TABLE_COLUME_UID + " = ?", new String[]{libraryUid});
        } catch (Exception e) {
            printErrorInfoFromException("OnMediaLibraryUpdateRevision", e);
        }
    }



    //媒体库Progress,回调
    @Override
    public void OnMediaLibraryUpdateProgress(int progress, int datalen, String libraryUid, byte[] byteArryUid) {
        Log.i(TAG, "OnMediaLibraryUpdateProgress  progress:" + progress);
        try {
            if (!isExistUID(libraryUid)) {
                addNewMediaLibrartIndexInDb(libraryUid, byteArryUid);
            }
            ContentValues values = mIPodDataBaseHelper.getMediaLibraryCursorByUid(libraryUid);
            if (values == null)
                values = new ContentValues();
            values.put(MEDIALIBIRAYR_TABLE_COLUME_UPDATEPROGRESS, progress);
            mIPodDataBaseHelper.getWritableDatabase()
                    .update(MEDIALIBIRAYR_TABLE, values, MEDIALIBIRAYR_TABLE_COLUME_UID + " = ?", new String[]{libraryUid});
            if(progress == 100)
            {
                Ipod.getInstance().iAP2StopMediaLibraryUpdates(byteArryUid.length,byteArryUid);
                hasUpdateCount++;
                Log.d(TAG,"has update count "+hasUpdateCount);
                if(hasUpdateCount == mIPodDataBaseHelper.getMediaLibraryCount())
                {
                    Log.d(TAG,"MLU send complete");
					LocalBroadcastManager lbm = LocalBroadcastManager.getInstance(mContext);

                   lbm.sendBroadcast(new Intent(Constants.ACTION_SYNC_STATE_IPOD_COMPLETED));
                    iPodDevice.setDeviceSynState(IPOD_DEV_SYN_COMPLETE); //同步结束，

                }
            }
        } catch (Exception e) {
            printErrorInfoFromException("OnMediaLibraryUpdateRevision", e);
        }
    }

    @Override
    public void OnMediaLibraryReset(int datalen, String libraryUid) {
        Log.i(TAG, "OnMediaLibraryReset :");
    }

    //媒体库媒体信息回调
    @Override
    public void onMediaItem(MediaItem item, int uidLen, String libraryUid, byte[] byteArryUid) {

        try {
            if (!isExistUID(libraryUid)) {
                addNewMediaLibrartIndexInDb(libraryUid, byteArryUid);
            }
            if (mIPodDataBaseHelper != null) {
                mIPodDataBaseHelper.createMediaItemTable(libraryUid);
            }
            String tableName = mIPodDataBaseHelper.getMediaLibraryItemTableName(libraryUid);
            int existAttribute = item.getExistAttributeByte();
            ContentValues values = mIPodDataBaseHelper.getMediaItemCursorBypid(tableName, PidString.converPidArrayToStr(item.getMediaItemPid()));
            if (values == null)
                values = new ContentValues();
            if ((existAttribute & 0x00000001) != 0)//0
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_PID, PidString.converPidArrayToStr(item.getMediaItemPid()));
            if ((existAttribute & 0x00000002) != 0)//1
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_TITLE, item.getMediaItemTitle());
            if ((existAttribute & 0x00000004) != 0)//2
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_MEDIATYPE, item.getMediaItemType());
            if ((existAttribute & 0x00000008) != 0)//3
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_RATING, item.getMediaItemRating());
            if ((existAttribute & 0x00000010) != 0)//4
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_DURATION, item.getMediaItemDuration());
            if ((existAttribute & 0x00000020) != 0)//5
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ALBUM_PID, PidString.converPidArrayToStr(item.getMediaItemAlbumPid()));
            if ((existAttribute & 0x00000040) != 0)//6
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ALBUM_TITLE, item.getMediaItemAlbumTitle());
            if ((existAttribute & 0x00000080) != 0)//7
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ALBUM_TRACK_NUMBER, item.getMediaItemAlbumTrackNumber());
            if ((existAttribute & 0x00000100) != 0)//8
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ALBUM_TRACK_COUNT, item.getMediaItemAlbumTrackCount());
            if ((existAttribute & 0x00000200) != 0)//9
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ALBUM_DISC_NUMBER, item.getMediaItemAlbumDiscNumber());
            if ((existAttribute & 0x00000400) != 0)//10
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ALBUM_DISC_COUNT, item.getMediaItemAlbumDiscCount());
            if ((existAttribute & 0x00000800) != 0)//11
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ARTIST_PID, PidString.converPidArrayToStr(item.getMediaItemArtistPid()));
            if ((existAttribute & 0x00001000) != 0)//12
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ARTIST, item.getMediaItemArtist());
            if ((existAttribute & 0x00002000) != 0)//13
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ALBUM_ARTIST_PID, PidString.converPidArrayToStr(item.getMediaItemAlbumArtistPid()));
            if ((existAttribute & 0x00004000) != 0)//14
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ALBUM_ARTIST, item.getMediaItemAlbumArtist());
            if ((existAttribute & 0x00008000) != 0)//15
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_GENRE_PID, PidString.converPidArrayToStr(item.getMediaItemGenrePid()));
            if ((existAttribute & 0x00010000) != 0)//16
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_GENRE, item.getMediaItemGenre());
            if ((existAttribute & 0x00020000) != 0)//17
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_COMPOSER_PID, PidString.converPidArrayToStr(item.getMediaItemComposerPid()));
            if ((existAttribute & 0x00040000) != 0)//18
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_COMPOSER, item.getMediaItemComposer());
            if ((existAttribute & 0x00080000) != 0)//19
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_IS_PART_COMPILATION, item.getIsPartOfCompilation());
            if ((existAttribute & 0x00200000) != 0)//21
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_IS_LIKE_SUPPORT, item.getLikeSupported());
            if ((existAttribute & 0x00400000) != 0)//22
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_IS_BAN_SUPPORT, item.getBanSupported());
            if ((existAttribute & 0x00800000) != 0)//23
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ISLIKE, item.getLiked());
            if ((existAttribute & 0x01000000) != 0)//24
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ISBANED, item.getBaned());
            if ((existAttribute & 0x02000000) != 0)//25
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_IS_RESIDENT_ON_DEVICE, item.getResidentOnDevice());
            if ((existAttribute & 0x04000000) != 0)//26
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_ARTWORK_FS_ID, item.getArtWorkFileTransferId());
            if ((existAttribute & 0x08000000) != 0)//27
                values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_CHAPTER_COUNT, item.getChapterCount());
            //if ((existAttribute & 0x10000000) != 0)//28
            values.put(MediaItem.MEDIA_ITEM_TABLE_COLUME_EXIST_ATTRIBUTE, item.getExistAttributeByte());
            if (tableName != null) {
                if (mIPodDataBaseHelper.isMediaItemExist(tableName, PidString.converPidArrayToStr(item.getMediaItemPid()))) {
                    mIPodDataBaseHelper.getWritableDatabase().update(tableName, values, "pid=?", new String[]{PidString.converPidArrayToStr(item.getMediaItemPid())});
                } else {
                    long result = mIPodDataBaseHelper.getWritableDatabase().insert(tableName, null, values);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //播放列表回调
    @Override
    public void onMediaPlaylist(MediaPlaylist playlist, int uidLen, String libraryUid, byte[] byteArryUid) {

        Log.i(TAG, "onMediaPlaylist playlist Name= " + playlist.getName());
        Log.i(TAG, "onMediaPlaylist playlist getMediaLibraryUid= " + playlist.getMediaLibraryUid());
        try {
            // String strUid = new String(uidData, "utf-8");
            if (!isExistUID(libraryUid)) {
                Log.i(TAG, "not exist");
                addNewMediaLibrartIndexInDb(libraryUid, byteArryUid);
            }
            if (mIPodDataBaseHelper != null) {
                Log.i(TAG, "onMediaPlaylist playlist 111");
                mIPodDataBaseHelper.createMediaPlaylistTable(libraryUid);
            }
            int existAttribute = playlist.getExistAttributeByte();
            String tableName = mIPodDataBaseHelper.getMediaLibrarymediaplaylistTableName(libraryUid);
            Log.i(TAG, "onMediaPlaylist playlist tableName=" + tableName);
            ContentValues values = mIPodDataBaseHelper.getMediaPlaylistCursorBypid(tableName, PidString.converPidArrayToStr(playlist.getPid()));
            if (values == null)
                values = new ContentValues();
            if ((existAttribute & 0x00000001) != 0)//0
                values.put(MediaPlaylist.MEDIA_PLAYLIST_TABLE_COLUME_PID, PidString.converPidArrayToStr(playlist.getPid()));
            if ((existAttribute & 0x00000002) != 0)//1
                values.put(MediaPlaylist.MEDIA_PLAYLIST_TABLE_COLUME_NAME, playlist.getName());
            if ((existAttribute & 0x00000004) != 0)//2
                values.put(MediaPlaylist.MEDIA_PLAYLIST_TABLE_COLUME_PARENT_PID, PidString.converPidArrayToStr(playlist.getParentPid()));
            if ((existAttribute & 0x00000008) != 0)//3
                values.put(MediaPlaylist.MEDIA_PLAYLIST_TABLE_COLUME_GENIUS_MIX, playlist.getGeniusMix());
            if ((existAttribute & 0x00000010) != 0)//4
                values.put(MediaPlaylist.MEDIA_PLAYLIST_TABLE_COLUME_ISFOLDER, playlist.getFolder());
            if ((existAttribute & 0x00000020) != 0)//5
                values.put(MediaPlaylist.MEDIA_PLAYLIST_TABLE_COLUME_FILE_TRANSFER_PID, playlist.getFileTransferID());
            if ((existAttribute & 0x00000040) != 0)//6
                values.put(MediaPlaylist.MEDIA_PLAYLIST_TABLE_COLUME_IS_RADIO_STATION, playlist.getAppMusicRadioStation());
            if ((existAttribute & 0x00000080) != 0)//
                values.put(MediaPlaylist.MEDIA_PLAYLIST_TABLE_COLUME_EXIST_ATTRIBUTE, playlist.getExistAttributeByte());

            if (tableName != null) {
                if (mIPodDataBaseHelper.isMediaItemExist(tableName, PidString.converPidArrayToStr(playlist.getPid()))) {
                    Log.i(TAG, "onMediaPlaylist playlist 33");
                    mIPodDataBaseHelper.getWritableDatabase().update(tableName, values, "pid=?", new String[]{PidString.converPidArrayToStr(playlist.getPid())});
                } else {
                    Log.i(TAG, "onMediaPlaylist playlist 44");
                    long result = mIPodDataBaseHelper.getWritableDatabase().insert(tableName, null, values);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void printErrorInfoFromException(String tag, Exception e) {
        try {
            StringWriter sw = new StringWriter();
            PrintWriter pw = new PrintWriter(sw);
            e.printStackTrace(pw);
            Log.i(TAG, tag + ": " + sw.toString());
            sw.close();
            pw.close();
        } catch (Exception e2) {
            e2.printStackTrace();
        }
    }

    public boolean tabIsExist(String tabName) {
        boolean result = false;
        if (tabName == null) {
            return false;
        }
        Cursor cursor = null;
        try {

            String sql = "select count(*) as c from sqlite_master where type ='table' and name ='" + tabName.trim() + "' ";
            cursor = mIPodDataBaseHelper.getWritableDatabase().rawQuery(sql, null);
            if (cursor.moveToNext()) {
                int count = cursor.getInt(0);
                if (count > 0) {
                    result = true;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (cursor != null)
            cursor.close();
        return result;
    }

}
