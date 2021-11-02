package com.autoipod.utils;

import com.autoipod.jni.IMediaItemAttributes;

public class NowPlayingUpdatesMediaItemAttributes implements IMediaItemAttributes{
    static final String TAG = "NowPlayingUpdatesMediaItemAttributes";

    byte[] mediaItemPID ;

    @Override
    public void OnMediaItemRawData(int datalen, byte[] data) {

    }

    @Override
    public void OnMediaItemPID(int datalen, byte[] data) {
        mediaItemPID = data;
    }

    public byte[]  getMeidaItemPid()
    {
        return mediaItemPID;
    }

    @Override
    public void  onMediaItemTile(int len,String mediaItemTtile){

    }


    @Override
    public void OnMediaItemMediaType(int type) {

    }

    @Override
    public void onDuration(int duration){

    }

    @Override
    public void OnMediaItemAlbumPID(int datalen, byte[] data) {

    }

    @Override
    public void OnMediaItemRating(int rating) {

    }

    @Override
    public void onMediaItemAlumTitle(int len,String title){


    }

    @Override
    public void OnMediaItemAlbumTrackNumber(int number) {

    }

    @Override
    public void OnMediaItemAlbumTrackCount(int count) {
    }

    @Override
    public void OnMediaItemAlbumDiscNumber(int number) {
    }

    @Override
    public void OnMediaItemAlbumDiscCount(int count) {
    }

    @Override
    public void OnMediaItemArtistPID(int datalen, byte[] data) {
    }

    @Override
    public void onMediaItemArtist(int len,String artist){

    }

    @Override
    public void onMediaItemAlumArtist(int len,String artist){

    }

    @Override
    public void OnMediaItemGenrePID(int len, byte[] utf8Data) {

    }

    @Override
    public void OnMediaItemGenre(int len, String Genre) {

    }

    @Override
    public void OnMediaItemComposerPID(int len, byte[] utf8Data) {

    }

    @Override
    public void OnMediaItemGenreComposer(int len, String composer) {

    }

    @Override
    public void OnMediaItemIsPartOfCompilation(boolean isPartof) {

    }

    @Override
    public void OnMediaItemIsLikeSupported(boolean isSupported) {
    }

    @Override
    public void OnMediaItemIsBeanSupported(boolean isSupported) {

    }

    @Override
    public void OnMediaItemIsLiked(boolean isLiked) {
    }

    @Override
    public void OnMediaItemIsBanned(boolean isBanned) {

    }

    @Override
    public void OnMediaItemIsResidentOnDevice(boolean isResident) {

    }

    @Override
    public void OnMediaItemArtWorkFileTransferID(int id) {

    }

    @Override
    public void OnMediaItemChapterCount(int count) {
    }


    @Override
    public void OnMediaItemAlbumArtistPID(int len, byte[] utf8Data) {

    }

}
