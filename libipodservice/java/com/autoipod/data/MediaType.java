package com.autoipod.data;

import android.os.Parcel;
import android.os.Parcelable;

public class MediaType implements Parcelable {

    private String mediaTitle;
    private int mediaType;

    public MediaType(String mediaTitle,int meidaType)
    {
        this.mediaTitle = mediaTitle;
        this.mediaType = meidaType;
    }

    public MediaType()
    {
        this.mediaType = -1;
        this.mediaTitle = "";
    }

    public String getMediaTitle() {
        return mediaTitle;
    }

    public void setMediaTitle(String mediaTitle) {
        this.mediaTitle = mediaTitle;
    }

    public int getMediaType() {
        return mediaType;
    }

    public void setMediaType(int mediaType) {
        this.mediaType = mediaType;
    }

    public static final Creator<MediaType> CREATOR
            = new Creator<MediaType>(){
        @Override
        public MediaType createFromParcel(Parcel source) {
            return new MediaType(source);
        }

        @Override
        public MediaType[] newArray(int size) {
            return new MediaType[size];
        }
    };



    private MediaType(Parcel in){
        mediaTitle = in.readString();

        mediaType = in.readInt();

    }
    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(mediaTitle);
        dest.writeInt(mediaType);
       // dest.writeString(age);
    }

    @Override
    public int describeContents() {
        return 0;
    }
}
