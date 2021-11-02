package com.autoipod.jni;

public interface IMediaItemAttributes {
   public void OnMediaItemRawData(int datalen,byte data[]);
   public void OnMediaItemPID(int datalen,byte data[]);
   public void  onMediaItemTile(int len,String title);
   public void OnMediaItemMediaType(int type);
   public void OnMediaItemRating(int rating);
   public void onDuration(int duration);
   public void OnMediaItemAlbumPID(int datalen,byte data[]);
   public void onMediaItemAlumTitle(int len,String title);
   public void OnMediaItemAlbumTrackNumber(int number);
   public void OnMediaItemAlbumTrackCount(int count);
   public void OnMediaItemAlbumDiscNumber(int number);
   public void OnMediaItemAlbumDiscCount(int count);
   public void OnMediaItemArtistPID(int datalen,byte data[]);
   public void onMediaItemArtist(int len,String artist);
   public void OnMediaItemAlbumArtistPID(int len,byte[] utf8Data);
   public void onMediaItemAlumArtist(int len,String artist);
   public void OnMediaItemGenrePID(int len,byte[] utf8Data);
   public void OnMediaItemGenre(int len,String genre);
   public void OnMediaItemComposerPID(int len,byte[] utf8Data);
   public void OnMediaItemGenreComposer(int len,String genre);
   public void OnMediaItemIsPartOfCompilation(boolean isPartof);
   public void OnMediaItemIsLikeSupported(boolean isSupported);
   public void OnMediaItemIsBeanSupported(boolean isSupported);
   public void OnMediaItemIsLiked(boolean isLiked);
   public void OnMediaItemIsBanned(boolean isBanned);
   public void OnMediaItemIsResidentOnDevice(boolean isResident);
   public void OnMediaItemArtWorkFileTransferID(int id);
   public void OnMediaItemChapterCount(int count);
}
