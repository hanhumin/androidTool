package com.autoipod.data;

import android.os.Parcel;
import android.os.Parcelable;

import com.autoipod.utils.PidString;

import java.util.ArrayList;
import java.util.List;

public class MediaItem implements Parcelable {
    final static String TAG = "MediaItem";

    final public static int MSG_MEDIA_ITEM_PID = 0;
    final public static int MSG_MEDIA_ITEM_TITLE = 1;
    final public static int MSG_MEDIA_ITEM_MEDIA_TYPE = 2;
    final public static int MSG_MEDIA_ITEM_RATING = 3;
    final public static int MSG_MEDIA_ITEM_PLAYBACK_DURATION = 4;
    final public static int MSG_MEDIA_ITEM_ALBUM_PID = 5;
    final public static int MSG_MEDIA_ITEM_ALBUM_TITLE = 6;
    final public static int MSG_MEDIA_ITEM_ALBUM_TRACK_NUMBER = 7;
    final public static int MSG_MEDIA_ITEM_ALBUM_TRACK_COUNT = 8;
    final public static int MSG_MEDIA_ITEM_ALBUM_DISC_NUMBER = 9;
    final public static int MSG_MEDIA_ITEM_ALBUM_DISC_COUNT = 10;
    final public static int MSG_MEDIA_ITEM_ARTIST_PID =11;
    final public static int MSG_MEDIA_ITEM_ARTIST = 12;
    final public static int MSG_MEDIA_ITEM_ALBUM_ARTIST_PID = 13;
    final public static int MSG_MEDIA_ITEM_ALBUM_ARTIST = 14;
    final public static int MSG_MEDIA_ITEM_GENRE_PID = 15;
    final public static int MSG_MEDIA_ITEM_GENRE = 16;
    final public static int MSG_MEDIA_ITEM_COMPOSER_PID = 17;
    final public static int MSG_MEDIA_ITEM_COMPOSER = 18;
    final public static int MSG_MEDIA_ITEM_IS_PART_OF_COMPILATION = 19;
    final public static int MSG_MEDIA_ITEM_IS_LIKE_SUPPORTED = 21;
    final public static int MSG_MEDIA_ITEM_IS_BAN_SUPPORTED = 22;
    final public static int MSG_MEDIA_ITEM_IS_LIKED = 23;
    final public static int MSG_MEDIA_ITEM_IS_BANNED = 24;
    final public static int MSG_MEDIA_ITEM_IS_RESIDENT_ON_DEVICE = 25;
    final public static int MSG_MEDIA_ITEM_ARTWORK_FILE_TRASFER_ID = 26;
    final public static int MSG_MEDIA_ITEM_CHAPTER_COUNT = 27;

    public  static final String  MEDIA_ITEM_TABLE_COLUME_PID = "pid";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_TITLE = "title";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_MEDIATYPE = "mediatype";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_RATING = "rating";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_DURATION = "duration";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ALBUM_PID = "album_pid";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ALBUM_TITLE = "album_tile";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ALBUM_TRACK_NUMBER = "album_track_number";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ALBUM_TRACK_COUNT = "album_track_count";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ALBUM_DISC_NUMBER = "album_disc_number";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ALBUM_DISC_COUNT = "album_disc_count";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ARTIST_PID = "artist_pid";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ARTIST = "artist";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ALBUM_ARTIST_PID = "album_artist_pid";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ALBUM_ARTIST = "album_artist";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_GENRE_PID = "genre_pid";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_GENRE = "genre";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_COMPOSER_PID = "composer_pid";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_COMPOSER = "composer";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_IS_PART_COMPILATION = "is_part_compilation";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_IS_LIKE_SUPPORT = "is_like_support";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_IS_BAN_SUPPORT = "is_ban_supported";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ISLIKE = "islike";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ISBANED = "isbanned";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_IS_RESIDENT_ON_DEVICE = "isresident_on_device";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_ARTWORK_FS_ID = "artwork_filetransfer_id";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_CHAPTER_COUNT = "chapter_count";
    public  static final String  MEDIA_ITEM_TABLE_COLUME_EXIST_ATTRIBUTE = "exist_attribute";

    public static final int ITEM_TYPE_ALBUM = 1 << 1;
    public static final int ITEM_TYPE_ARTIST = 1 << 2;
    public static final int ITEM_TYPE_GENRE = 1 << 3;
    public static final int ITEM_TYPE_PLAYLIST = 1 << 4;
    public static final int ITEM_TYPE_MEDIA = 1 << 5;
    public static final int ITEM_TYPE_DUMMY = 1 << 6;
    public static final int ITEM_TYPE_EMPTY = 1 << 7;
    public static final int ITEM_TYPE_DIR = 1 << 8;
    public static final int ITEM_TYPE_FILE = 1 << 9;

    public final static int CATEGORY_TYPE_ALL = 0;
    public static final int CATEGORY_TYPE_AUDIO = 1;
    public static final int CATEGORY_TYPE_VIDEO = 1 << 1;
    public static final int CATEGORY_TYPE_IMAGE = 1 << 2;
    public static final int CATEGORY_TYPE_STREAM = 1 << 3;

    @Override
    public String toString() {
        return String.format("Title : %s Arist : %s during: %s AblumTilte : %s",mediaItemTile,mediaItemArtist,duration,mediaItemAlbumTitle);
    }

    private byte[] mediaItemPid;
    private String mediaItemTile;
    private int mediaItemType;
    private int mediaItemRating;
    private int duration;
    private byte[] mediaItemAlbumPid;
    private String mediaItemAlbumTitle;
    private int mediaItemAlbumTrackNumber;
    private int mediaItemAlbumTrackCount;
    private int mediaItemAlbumDiscNumber;
    private int mediaItemAlbumDiscCount;
    private byte[] mediaItemArtistPid;
    private String mediaItemArtist;
    private byte[] mediaItemAlbumArtistPid;
    private String mediaItemAlbumArtist;
    private byte[] mediaItemGenrePid;
    private String mediaItemGenre;
    private byte[] mediaItemComposerPid;
    private String mediaItemComposer;
    private byte[] uid;
    boolean isPartOfCompilation;
    boolean isLikeSupported;
    boolean isBanSupported;
    boolean isLiked;
    boolean isBaned;
    boolean isResidentOnDevice;
    int      artWorkFileTransferId;
    int      chapterCount;
    private int mExistAttributeByte = 0x00000000;


    public void setUid(byte[] uid)
    {
        this.uid = uid;
    }

    public byte[] getUid()
    {
        return this.uid;
    }

    public List<String> getMediaTypeStrings()
    {
        int type = getMediaItemType();

        List<String> strType = new ArrayList<String>();
        if((type & 1)!=0)
        {
            strType.add("Music");
        }
        if((type & 2)!=0)
        {
            strType.add("Podcast");
        }
        if((type & 4)!=0)
        {
            strType.add("AudioBook");
        }
       return strType;

    }
    public void setChapterCount( int id)
    {
        chapterCount = id;
        setExistAttribute(MSG_MEDIA_ITEM_CHAPTER_COUNT);
    }
    public int getChapterCount()
    {
        return chapterCount;
    }
    /*
    public void setArtWorkFileTransferId( int id)
    {
        artWorkFileTransferId = id;
    }
    public int getArtWorkFileTransferId()
    {
        return artWorkFileTransferId;
    }

    */
    public void setArtWorkFileTransferId( int id)
    {
        artWorkFileTransferId = id;
        setExistAttribute(MSG_MEDIA_ITEM_ARTWORK_FILE_TRASFER_ID);
    }
    public int getArtWorkFileTransferId()
    {
        return artWorkFileTransferId;
    }
    public void setResidentOnDevice( boolean status)
    {
        isResidentOnDevice = status;
        setExistAttribute(MSG_MEDIA_ITEM_IS_RESIDENT_ON_DEVICE);
    }
    public boolean getResidentOnDevice()
    {
        return isResidentOnDevice;
    }
    public void setBaned( boolean status)
    {
        isBaned = status;
        setExistAttribute(MSG_MEDIA_ITEM_IS_BANNED);
    }
    public boolean getBaned()
    {
        return isBaned;
    }
    public void setLiked( boolean status)
    {
        isLiked = status;
        setExistAttribute(MSG_MEDIA_ITEM_IS_LIKED);
    }
    public boolean getLiked()
    {
        return isLiked;
    }
    public void setBanSupported( boolean status)
    {
        isBanSupported = status;
        setExistAttribute(MSG_MEDIA_ITEM_IS_BAN_SUPPORTED);
    }
    public boolean getBanSupported()
    {
        return isBanSupported;
    }
    public void setLikeSupported( boolean status)
    {
        isLikeSupported = status;
        setExistAttribute(MSG_MEDIA_ITEM_IS_LIKE_SUPPORTED);
    }
    public boolean getLikeSupported()
    {
        return isLikeSupported;
    }

     public void setIsPartOfCompilation( boolean status)
     {
         isPartOfCompilation = status;
         setExistAttribute(MSG_MEDIA_ITEM_IS_PART_OF_COMPILATION);
     }
     public boolean getIsPartOfCompilation()
     {
         return isPartOfCompilation;
     }
    public void setMediaItemComposerPid(byte[] pid)
    {
        mediaItemComposerPid = pid;
        setExistAttribute(MSG_MEDIA_ITEM_COMPOSER_PID);
    }
    public byte[] getMediaItemComposerPid()
    {
        return mediaItemComposerPid;
    }
    public void setMediaItemComposer(String composer)
    {
        mediaItemComposer = composer;
        setExistAttribute(MSG_MEDIA_ITEM_COMPOSER);
    }
    public String getMediaItemComposer()
    {
        return mediaItemComposer;
    }

    public void setMediaItemGenrePid(byte[] pid)
    {
        mediaItemGenrePid = pid;
        setExistAttribute(MSG_MEDIA_ITEM_GENRE_PID);
    }
    public byte[] getMediaItemGenrePid()
    {
        return mediaItemGenrePid;
    }
    public void setMediaItemGenre(String genre)
    {
        mediaItemGenre = genre;
        setExistAttribute(MSG_MEDIA_ITEM_GENRE);
    }
    public String getMediaItemGenre()
    {
        return mediaItemGenre;
    }
    public void setMediaItemAlbumArtistPid(byte[] pid)
    {
        mediaItemAlbumArtistPid = pid;
        setExistAttribute(MSG_MEDIA_ITEM_ALBUM_ARTIST_PID);
    }
    public byte[] getMediaItemAlbumArtistPid()
    {
        return mediaItemAlbumArtistPid;
    }

    public void setMediaItemAlbumArtist(String artist)
    {
        mediaItemAlbumArtist = artist;
        setExistAttribute(MSG_MEDIA_ITEM_ALBUM_ARTIST);
    }
    public String getMediaItemAlbumArtist()
    {
        return mediaItemAlbumArtist;
    }
    public void setMediaItemArtistPid(byte[] pid)
    {
        mediaItemArtistPid = pid;
        setExistAttribute(MSG_MEDIA_ITEM_ARTIST_PID);
    }
    public byte[] getMediaItemArtistPid()
    {
        return mediaItemArtistPid;
    }

    public void setMediaItemArtist(String artist)
    {
        mediaItemArtist = artist;
        setExistAttribute(MSG_MEDIA_ITEM_ARTIST);
    }
    public String getMediaItemArtist()
    {
        return mediaItemArtist;

    }

    public void setMediaItemAlbumDiscCount(int count )
    {
        mediaItemAlbumDiscCount = count;
        setExistAttribute(MSG_MEDIA_ITEM_ALBUM_DISC_COUNT);
    }
    public int getMediaItemAlbumDiscCount()
    {
        return  mediaItemAlbumDiscCount;
    }

    public void setMediaItemAlbumDiscNumber(int number )
    {
        mediaItemAlbumDiscNumber = number;
        setExistAttribute(MSG_MEDIA_ITEM_ALBUM_DISC_NUMBER);
    }
    public int getMediaItemAlbumDiscNumber()
    {
        return  mediaItemAlbumDiscNumber;

    }
    public void setMediaItemAlbumTrackCount(int count )
    {
        mediaItemAlbumTrackCount = count;
        setExistAttribute(MSG_MEDIA_ITEM_ALBUM_TRACK_COUNT);
    }
    public int getMediaItemAlbumTrackCount()
    {
        return  mediaItemAlbumTrackCount;
    }

    public void setMediaItemAlbumTrackNumber(int number )
    {
        mediaItemAlbumTrackNumber = number;
        setExistAttribute(MSG_MEDIA_ITEM_ALBUM_TRACK_NUMBER);
    }
    public int getMediaItemAlbumTrackNumber()
    {
        return  mediaItemAlbumTrackNumber;
    }


    public void setMediaItemAlbumTitle(String title)
    {
        mediaItemAlbumTitle = title;
        setExistAttribute(MSG_MEDIA_ITEM_ALBUM_TITLE);
    }
    public String getMediaItemAlbumTitle()
    {
        return  mediaItemAlbumTitle;
    }
    public void setMediaItemmAlbumPid(byte[] pid )
    {
        mediaItemAlbumPid = pid;
        setExistAttribute(MSG_MEDIA_ITEM_ALBUM_PID);
    }
    public byte[] getMediaItemAlbumPid()
    {
        return  mediaItemAlbumPid;
    }

    public void setMediaItemDuration(int duration)
    {
        this.duration = duration;
        setExistAttribute(MSG_MEDIA_ITEM_PLAYBACK_DURATION);
    }
    public int getMediaItemDuration()
    {
        return  this.duration;
    }

    public void setMediaItemRating(int rate)
    {
        mediaItemRating = rate;
        setExistAttribute(MSG_MEDIA_ITEM_RATING);
    }
    public int getMediaItemRating()
    {
        return mediaItemRating;
    }

    public void setMediaItemType(int type)
    {
        mediaItemType = type;
        setExistAttribute(MSG_MEDIA_ITEM_MEDIA_TYPE);
    }
    public int getMediaItemType()
    {
        return mediaItemType;
    }

    public void setMediaItemPid(byte[] pid)
    {
       // Log.i("app","setMediaItemPid");
       // Log.i("app","setMediaItemPid pid.len="+pid.length);
      //  for(int i=0;i<pid.length;i++)
         //   Log.i("app","setMediaItemPid pid["+i+"]="+pid[i]);
        mediaItemPid = pid;
        setExistAttribute(MSG_MEDIA_ITEM_PID);
    }
    public byte[] getMediaItemPid()
    {
        return mediaItemPid;
    }

    public void setMediaItemTitle(String title)
    {
        mediaItemTile = title;
        setExistAttribute(MSG_MEDIA_ITEM_TITLE);
    }
    public String getMediaItemTitle()
    {
        return mediaItemTile;
    }


    public void setExistAttributeByte(int existAttributeByte)
    {
        mExistAttributeByte = existAttributeByte;
    }
    public int getExistAttributeByte()
    {
        return mExistAttributeByte;
    }

    public void setExistAttribute(int item)
    {
       int i = 0x00000001;
       i= i << item;
        mExistAttributeByte = mExistAttributeByte | i;
    }

    public boolean isExistAttribute(int item)
    {
        int i = 0x00000001;
        i= i << item;
        mExistAttributeByte = mExistAttributeByte & i;
        if(mExistAttributeByte==0)
            return false;
        return  true;
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

    public static final Creator<MediaItem> CREATOR
            = new Creator<MediaItem>(){
        @Override
        public MediaItem createFromParcel(Parcel source) {
            return new MediaItem(source);
        }

        @Override
        public MediaItem[] newArray(int size) {
            return new MediaItem[size];
        }
    };

    public MediaItem()
    {}

    public MediaItem(Parcel in){

        mExistAttributeByte = in.readInt();

        if(isExistAttribute(MSG_MEDIA_ITEM_PID))
            mediaItemPid = PidString.covertStrToByteArray(in.readString());
          //  dest.writeString(PidString.converPidArrayToStr(mediaItemPid));
        if(isExistAttribute(MSG_MEDIA_ITEM_TITLE))
            mediaItemTile = in.readString();
        if(isExistAttribute(MSG_MEDIA_ITEM_MEDIA_TYPE))
            mediaItemType = in.readInt();
        if(isExistAttribute(MSG_MEDIA_ITEM_RATING))
            mediaItemRating = in.readInt();
        if(isExistAttribute(MSG_MEDIA_ITEM_PLAYBACK_DURATION))
            duration = in.readInt();
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_PID))
              mediaItemAlbumPid = PidString.covertStrToByteArray(in.readString());
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_TITLE))
            mediaItemAlbumTitle = in.readString();
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_TRACK_NUMBER))
            mediaItemAlbumTrackNumber = in.readInt();
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_TRACK_COUNT))
            mediaItemAlbumTrackCount = in.readInt();
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_DISC_NUMBER))
            mediaItemAlbumDiscNumber = in.readInt();
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_DISC_COUNT))
            mediaItemAlbumDiscCount = in.readInt();
        if(isExistAttribute(MSG_MEDIA_ITEM_ARTIST_PID))
            mediaItemArtistPid = PidString.covertStrToByteArray(in.readString());
        if(isExistAttribute(MSG_MEDIA_ITEM_ARTIST))
            mediaItemArtist = in.readString();
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_ARTIST_PID))
            mediaItemAlbumArtistPid = PidString.covertStrToByteArray(in.readString());
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_ARTIST))
            mediaItemAlbumArtist = in.readString();

        if(isExistAttribute(MSG_MEDIA_ITEM_GENRE_PID))
            mediaItemGenrePid = PidString.covertStrToByteArray(in.readString());

        if(isExistAttribute(MSG_MEDIA_ITEM_GENRE))
            mediaItemGenre = in.readString();

        if(isExistAttribute(MSG_MEDIA_ITEM_COMPOSER_PID))
            mediaItemComposerPid = PidString.covertStrToByteArray(in.readString());

        if(isExistAttribute(MSG_MEDIA_ITEM_COMPOSER))
            mediaItemComposer = in.readString();

        if(isExistAttribute(MSG_MEDIA_ITEM_IS_PART_OF_COMPILATION)) {
            int i = in.readInt();
            if(i == 1)
                isPartOfCompilation = true;
            else
                isPartOfCompilation = false;
        }

        if(isExistAttribute(MSG_MEDIA_ITEM_IS_LIKE_SUPPORTED)) {
            int i = in.readInt();
            if(i == 1)
                isLikeSupported = true;
            else
                isLikeSupported = false;
        }
        if(isExistAttribute(MSG_MEDIA_ITEM_IS_BAN_SUPPORTED)) {
            int i = in.readInt();
            if(i == 1)
                isBanSupported = true;
            else
                isBanSupported = false;
        }
        if(isExistAttribute(MSG_MEDIA_ITEM_IS_LIKED)) {
            int i = in.readInt();
            if(i == 1)
                isLiked = true;
            else
                isLiked = false;
        }

        if(isExistAttribute(MSG_MEDIA_ITEM_IS_BANNED)) {
            int i = in.readInt();
            if(i == 1)
                isBaned  = true;
            else
                isBaned = false;
        }

        if(isExistAttribute(MSG_MEDIA_ITEM_IS_RESIDENT_ON_DEVICE)) {
            int i = in.readInt();
            if(i == 1)
                isResidentOnDevice = true;
            else
                isResidentOnDevice = false;
        }
        if(isExistAttribute(MSG_MEDIA_ITEM_ARTWORK_FILE_TRASFER_ID)) {
            artWorkFileTransferId = in.readInt();

        }
        if(isExistAttribute(MSG_MEDIA_ITEM_CHAPTER_COUNT)) {
            chapterCount = in.readInt();

        }

    }
    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(mExistAttributeByte);
        if(isExistAttribute(MSG_MEDIA_ITEM_PID))
            dest.writeString(PidString.converPidArrayToStr(mediaItemPid));
        if(isExistAttribute(MSG_MEDIA_ITEM_TITLE))
            dest.writeString(mediaItemTile);
        if(isExistAttribute(MSG_MEDIA_ITEM_MEDIA_TYPE))
            dest.writeInt(mediaItemType);
        if(isExistAttribute(MSG_MEDIA_ITEM_RATING))
            dest.writeInt(mediaItemRating);
        if(isExistAttribute(MSG_MEDIA_ITEM_PLAYBACK_DURATION))
            dest.writeInt(duration);
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_PID))
            dest.writeString(PidString.converPidArrayToStr(mediaItemAlbumPid));
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_TITLE))
            dest.writeString(mediaItemAlbumTitle);
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_TRACK_NUMBER))
            dest.writeInt(mediaItemAlbumTrackNumber);
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_TRACK_COUNT))
            dest.writeInt(mediaItemAlbumTrackCount);
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_DISC_NUMBER))
            dest.writeInt(mediaItemAlbumDiscNumber);
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_DISC_COUNT))
            dest.writeInt(mediaItemAlbumDiscCount);
        if(isExistAttribute(MSG_MEDIA_ITEM_ARTIST_PID))
            dest.writeString(PidString.converPidArrayToStr(mediaItemArtistPid));
        if(isExistAttribute(MSG_MEDIA_ITEM_ARTIST))
            dest.writeString(mediaItemArtist);
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_ARTIST_PID))
            dest.writeString(PidString.converPidArrayToStr(mediaItemAlbumArtistPid));
        if(isExistAttribute(MSG_MEDIA_ITEM_ALBUM_ARTIST))
            dest.writeString(mediaItemAlbumArtist);
        if(isExistAttribute(MSG_MEDIA_ITEM_GENRE_PID))
            dest.writeString(PidString.converPidArrayToStr(mediaItemGenrePid));
        if(isExistAttribute(MSG_MEDIA_ITEM_GENRE))
            dest.writeString(mediaItemGenre);
        if(isExistAttribute(MSG_MEDIA_ITEM_COMPOSER_PID))
            dest.writeString(PidString.converPidArrayToStr(mediaItemComposerPid));
        if(isExistAttribute(MSG_MEDIA_ITEM_COMPOSER))
            dest.writeString(mediaItemComposer);
        if(isExistAttribute(MSG_MEDIA_ITEM_IS_PART_OF_COMPILATION)) {
           if(isPartOfCompilation)
            dest.writeInt(1);
           else
               dest.writeInt(0);
        }
        if(isExistAttribute(MSG_MEDIA_ITEM_IS_PART_OF_COMPILATION)) {
            if(isPartOfCompilation)
                dest.writeInt(1);
            else
                dest.writeInt(0);
        }
        if(isExistAttribute(MSG_MEDIA_ITEM_IS_LIKE_SUPPORTED)) {
            if(isLikeSupported)
                dest.writeInt(1);
            else
                dest.writeInt(0);
        }
        if(isExistAttribute(MSG_MEDIA_ITEM_IS_BAN_SUPPORTED)) {
            if(isBanSupported)
                dest.writeInt(1);
            else
                dest.writeInt(0);
        }
        if(isExistAttribute(MSG_MEDIA_ITEM_IS_LIKED)) {
            if(isLiked)
                dest.writeInt(1);
            else
                dest.writeInt(0);
        }

        if(isExistAttribute(MSG_MEDIA_ITEM_IS_BANNED)) {
            if(isBaned)
                dest.writeInt(1);
            else
                dest.writeInt(0);
        }

        if(isExistAttribute(MSG_MEDIA_ITEM_IS_RESIDENT_ON_DEVICE)) {
            if(isResidentOnDevice)
                dest.writeInt(1);
            else
                dest.writeInt(0);
        }
        if(isExistAttribute(MSG_MEDIA_ITEM_ARTWORK_FILE_TRASFER_ID)) {

                dest.writeInt(artWorkFileTransferId);
        }
        if(isExistAttribute(MSG_MEDIA_ITEM_CHAPTER_COUNT)) {

            dest.writeInt(chapterCount);
        }


        // dest.writeString(age);
    }

    @Override
    public int describeContents() {
        return 0;
    }
}
