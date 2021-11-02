package com.autoipod;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.WorkerThread;
import android.support.v4.media.MediaBrowserCompat;
import android.support.v4.media.MediaDescriptionCompat;

import com.autoipod.data.MediaItem;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import java.util.ArrayList;
import java.util.List;

public class BrowserProvider {

    private static final String TAG = "IpodBrowserProvider";

    @WorkerThread
    public static List<MediaBrowserCompat.MediaItem> loadChildren(/*final IStorageDevice device,*/ String parentId) {
    
	Log.i(TAG,"loadChildren 222 parentId ="+parentId);
        List<MediaBrowserCompat.MediaItem> results = new ArrayList<>();
        IPodDevice device = IPodDeviceMannager.getActiveDevice();
       if (device == null) return results;
/*
        FileGroup fileGroup = null;
        MediaItem parentItem = null;
        List<FolderItem> folderItemList = null;
        List<AlbumItem> albumItemList = null;
        List<ArtistItem> artistItemList = null;
        List<GenreItem> genreItemList = null;
        List<PlaylistItem> playlistItemList = null;
*/
        try {
            JSONObject jsonObject = new JSONObject(parentId);
            String method = jsonObject.getString("method");
            String param = jsonObject.getString("param");
            JSONObject paramObject = new JSONObject(param);
            switch (method) {
                case "getRootFolders": {
                  /*  //{"method":"getRootFolders","param":{"categoryType":1}}
                    if (param.contains("categoryType")) {
                        folderItemList = device.getRootFolders(paramObject.getInt("categoryType"));
                    }
                    */


                    break;
                }
                case "getFiles": {
                    //{"method":"getFiles","param":{"albumName":"周杰伦2004"}}
                    //{"method":"getFiles","param":{"genreName":"摇滚"}}
                    //{"method":"getFiles","param":{"artistName":"周杰伦"}}
                    //{"method":"getFiles","param":{"categoryType":1,"folderId":1,"deep":true}}
                    //{"method":"getFiles","param":{"categoryType":1,"deep":true}}
                    String song = null;
                    String album = null;
                    String genre = null;
                    String artist = null;
                    if (param.contains("albumName")) {
                        album = paramObject.getString("albumName");

                    } else if (param.contains("genreName")) {
                        genre = paramObject.getString("genreName");
                    } else if (param.contains("artistName")) {
                        artist = paramObject.getString("artistName");
                    }

                    MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
                    List<MediaItem> listMediaItem = device.searchSons(song,album,genre,artist);
                    Bundle extras = new Bundle();

                    for (MediaItem albumItem : listMediaItem) {
                        item = generateSearchSongsMediaDescription(item, extras,albumItem, 0);
                        results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
                    }

                    break;
                }
                case "getLastPlayFiles": {
                    //{"method":"getLastPlayFiles","param":{"categoryType":1,"deep":true}}
                  /*  if (param.contains("categoryType") && param.contains("deep")) {
                        fileGroup = device.getLastPlayFiles(paramObject.getInt("categoryType"), paramObject.getBoolean("deep"));
                    }
                    */
                    break;
                }
                case "getFolders": {
                    //{"method":"getFolders","param":{"categoryType":1,"folderId":1}}
                 /*   if (param.contains("categoryType") && param.contains("folderId")) {
                        FolderItem folderItem = device.getFolder(paramObject.getInt("categoryType"), paramObject.getInt("folderId"));
                        folderItemList = device.getFolders(folderItem);
                        parentItem = folderItem;
                    }
                    */
                    break;
                }
                case "getAlbums": {
                    //{"method":"getAlbums","param":{}}
                    //{"method":"getAlbums","param":{"genreName":1}}
                    //{"method":"getAlbums","param":{"artistName":1}}
                 /*   if (param.contains("genreId")) {
                        GenreItem genreItem = device.getGenre(paramObject.getInt("genreId"));
                        albumItemList = device.getAlbums(genreItem);
                        parentItem = genreItem;
                    } else if (param.contains("artistId")) {
                        ArtistItem artistItem = device.getArtist(paramObject.getInt("artistId"));
                        albumItemList = device.getAlbums(artistItem);
                        parentItem = artistItem;
                    } else {
                        albumItemList = device.getAlbums();
                    }
                    */

                    String song = null;
                    String album = null;
                    String genre = null;
                    String artist = null;
                  //  if (param.contains("albumName")) {
                      //  album = paramObject.getString("albumName");

                  //  } else
                    if (param.contains("genreName")) {
                        genre = paramObject.getString("genreName");
                    } else if (param.contains("artistName")) {
                        artist = paramObject.getString("artistName");
                    }

                    MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
                    List<MediaItem> listMediaItem = device.searchSons(song,album,genre,artist);
                    Bundle extras = new Bundle();

                    for (MediaItem albumItem : listMediaItem) {
                        item = generateSearchSongsMediaDescription(item, extras,albumItem, 0);
                        results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
                    }

                    break;
                }
                case "getArtists": {
                    //{"method":"getAlbums","param":{}}
                    //{"method":"getAlbums","param":{"genreName":1}}
                  /*  if (param.contains("genreId")) {
                        GenreItem genreItem = device.getGenre(paramObject.getInt("genreId"));
                        artistItemList = device.getArtists(genreItem);
                        parentItem = genreItem;
                    } else {
                        artistItemList = device.getArtists();
                    }
                    */
                    String song = null;
                    String album = null;
                    String genre = null;
                    String artist = null;
                    //  if (param.contains("albumName")) {
                    //  album = paramObject.getString("albumName");

                    //  } else
                   // if (param.contains("genreName")) {
                    //  genre = paramObject.getString("genreName");
                  //} else
                        if (param.contains("albumName")) {
                        album = paramObject.getString("albumName");
                    }

                    MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
                    List<MediaItem> listMediaItem = device.searchSons(song,album,genre,artist);
                    Bundle extras = new Bundle();

                    for (MediaItem albumItem : listMediaItem) {
                        item = generateSearchSongsMediaDescription(item, extras,albumItem, 0);
                        results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
                    }

                    break;
                }
                case "getGenres": {
                    //{"method":"getGenres","param":{}}
                    String song = null;
                    String album = null;
                    String genre = null;
                    String artist = null;
                    //  if (param.contains("albumName")) {
                    //  album = paramObject.getString("albumName");

                    //  } else
                    if (param.contains("genreName")) {
                        genre = paramObject.getString("genreName");
                    } else if (param.contains("albumName")) {
                        album = paramObject.getString("albumName");
                    }

                    MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
                    List<String> listMediaItem = device.getALLGennre();
                    Bundle extras = new Bundle();
                     int i = 0;
                    for (String albumItem : listMediaItem) {
                        item = generateMediaGenreDescription(item, extras,albumItem, i);
                        i++;
                        results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
                    }
                    break;
                }
                case "getPlaylists": {
                    //{"method":"getPlaylists","param":{"categoryType":1}}
                    //{"method":"getPlaylists","param":{"subFileId":1}}
                   /*  if (param.contains("categoryType")) {
                       playlistItemList = device.getPlaylists(paramObject.getInt("categoryType"));
                    }else if(param.contains("subFileId")){
                        FileItem subFileItem = device.getFile(paramObject.getInt("subFileId"));
                        playlistItemList = device.getPlaylists(subFileItem);
                    }
                    */
                    break;
                }
                case "getPlaylistEntries": {
                    //{"method":"getPlaylistEntries","param":{"playlistId":1}}
                    //{"method":"getPlaylistEntries","param":{"categoryType":1,"playlistName":""}}
                   /* if (param.contains("playlistId")) {
                        fileGroup = device.getPlaylistEntries(device.getPlaylist(paramObject.getInt("playlistId")));
                    } else if (param.contains("categoryType") && param.contains("playlistName")) {
                        fileGroup = device.getPlaylistEntries(device.getPlaylist(paramObject.getInt("categoryType"), paramObject.getString("playlistName")));
                    }
                    */
                    break;
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }

        /*f (fileGroup != null && !fileGroup.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (FileItem fileItem : fileGroup.getAll()) {
                item = generateMediaDescription(item, extras,fileItem, fileGroup.getGroupItem());
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_PLAYABLE));
            }
        } else if (folderItemList != null && !folderItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (FolderItem folderItem : folderItemList) {
                item = generateMediaDescription(item, extras,folderItem, parentItem);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
            }
        } else if (albumItemList != null && !albumItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (AlbumItem albumItem : albumItemList) {
                item = generateMediaDescription(item, extras,albumItem, parentItem);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
            }
        } else if (artistItemList != null && !artistItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (ArtistItem artistItem : artistItemList) {
                item = generateMediaDescription(item, extras,artistItem, parentItem);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
            }
        } else if (genreItemList != null && !genreItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (GenreItem genreItem : genreItemList) {
                item = generateMediaDescription(item, extras,genreItem, parentItem);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
            }
        } else if (playlistItemList != null && !playlistItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (PlaylistItem playlistItem : playlistItemList) {
                item = generateMediaDescription(item, extras,playlistItem, parentItem);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
            }
        }
        */
        return results;
    }

    @WorkerThread
    public static MediaBrowserCompat.MediaItem loadItem(String itemId) {
    
	Log.i(TAG,"loadItem 222 itemId ="+itemId);
      //  if (device == null) return null;
        IPodDevice device = IPodDeviceMannager.getActiveDevice();
        if (device == null) return null;

    //    MediaItem item = null;

        try {
            JSONObject jsonObject = new JSONObject(itemId);
            String method = jsonObject.getString("method");
            String param = jsonObject.getString("param");
            JSONObject paramObject = new JSONObject(param);
            switch (method) {
                case "getFile": {
                    //{"method":"getFile","param":{"fileId":1}}
                    //{"method":"getFile","param":{"filePath":""}}
                  /*  if (param.contains("fileId")) {
                        item = device.getFile(paramObject.getInt("fileId"));
                    } else if (param.contains("filePath")) {
                        item = device.getFile(paramObject.getString("filePath"));
                    }
                    */
                    break;
                }
                case "getFolder": {
                    //{"method":"getFolder","param":{"categoryType":1,"folderId":1}}
                    //{"method":"getFolder","param":{"categoryType":1,"folderPath":""}}
                    //{"method":"getFolder","param":{"subFileId":1}}
                    //{"method":"getFolder","param":{"categoryType":1,"subFolderId":1}}
                 /*   if (param.contains("categoryType")) {
                        if (param.contains("folderId")) {
                            item = device.getFolder(paramObject.getInt("categoryType"), paramObject.getInt("folderId"));
                        } else if (param.contains("folderPath")) {
                            item = device.getFolder(paramObject.getInt("categoryType"), paramObject.getString("folderPath"));
                        } else if (param.contains("subFolderId")) {
                            FolderItem subFolderItem = device.getFolder(paramObject.getInt("categoryType"), paramObject.getInt("subFolderId"));
                            item = device.getFolder(subFolderItem);
                        }
                    }else if (param.contains("subFileId")) {
                        FileItem subFileItem = device.getFile(paramObject.getInt("subFileId"));
                        item = device.getFolder(subFileItem);
                    }
                    */
                    break;
                }
                case "getArtist": {
                    //{"method":"getArtist","param":{"artistId":1}}
                    //{"method":"getArtist","param":{"artistName":1}}
                    //{"method":"getArtist","param":{"subFileId":1}}
                    //{"method":"getArtist","param":{"subAlbumId":1}}
                 /*   if (param.contains("artistId")) {
                        item = device.getArtist(paramObject.getInt("artistId"));
                    } else if (param.contains("artistName")) {
                        item = device.getArtist(paramObject.getString("artistName"));
                    }else if (param.contains("subFileId")) {
                        FileItem subFileItem = device.getFile(paramObject.getInt("subFileId"));
                        item = device.getArtist(subFileItem);
                    }else if (param.contains("subAlbumId")) {
                        AlbumItem subAlbumItem = device.getAlbum(paramObject.getInt("subAlbumId"));
                        item = device.getArtist(subAlbumItem);
                    }
                    */
                    if (param.contains("artistName")) {
                        String artistName = paramObject.getString("artistName");
                        if(artistName!=null)
                        {
                            List<String> artists = device.getAllArtist();

                            for(int i=0;i<artists.size();i++) {
                                if (artists.get(i).equals(artistName))
                                {

                                    MediaDescriptionCompat.Builder builder = new MediaDescriptionCompat.Builder();
                                    Bundle extras = new Bundle();
                                    builder = generateMediaArtistDescription(builder, extras,artistName, i);
                                    return new MediaBrowserCompat.MediaItem(builder.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE);
                                }
                            }

                        }
                    }
                    break;
                }

              //  query = "{\"method\":\"getAlbum\",\"param\":{\"albumName\":\"1701\"}}";
                case "getAlbum": {
                    //{"method":"getAlbum","param":{"albumId":1}}
                    //{"method":"getAlbum","param":{"albumName":1}}
                    //{"method":"getAlbum","param":{"subFileId":1}}
                  /*  if (param.contains("albumId")) {
                        item = device.getAlbum(paramObject.getInt("albumId"));
                    } else if (param.contains("albumName")) {
                        item = device.getAlbum(paramObject.getString("albumName"));
                    }else if (param.contains("subFileId")) {
                        FileItem subFileItem = device.getFile(paramObject.getInt("subFileId"));
                        item = device.getAlbum(subFileItem);
                    }
                    */
                    if (param.contains("albumName")) {
                        String albumName = paramObject.getString("albumName");
                        if(albumName!=null)
                        {
                            List<String> albumes = device.getAllAlumb();

                            for(int i=0;i<albumes.size();i++) {
                                if (albumes.get(i).equals(albumName))
                                {

                                        MediaDescriptionCompat.Builder builder = new MediaDescriptionCompat.Builder();
                                        Bundle extras = new Bundle();
                                        builder = generateMediaAlumbDescription(builder, extras,albumName, i);
                                        return new MediaBrowserCompat.MediaItem(builder.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE);
                                }
                            }

                        }
                    }
                    break;
                }
                case "getGenre": {
                    //{"method":"getGenre","param":{"genreId":1}}
                    //{"method":"getGenre","param":{"genreName":1}}
                    //{"method":"getGenre","param":{"subFileId":1}}
                    //{"method":"getGenre","param":{"subAlbumId":1}}
                    //{"method":"getGenre","param":{"subArtistId":1}}
                  /*  if (param.contains("genreId")) {
                        item = device.getGenre(paramObject.getInt("genreId"));
                    } else if (param.contains("genreName")) {
                        item = device.getGenre(paramObject.getString("genreName"));
                    } else if (param.contains("subFileId")) {
                        FileItem subFileItem = device.getFile(paramObject.getInt("subFileId"));
                        item = device.getGenre(subFileItem);
                    } else if (param.contains("subAlbumId")) {
                        AlbumItem subAlbumItem = device.getAlbum(paramObject.getInt("subAlbumId"));
                        item = device.getGenre(subAlbumItem);
                    } else if (param.contains("subArtistId")) {
                        ArtistItem subArtistItem = device.getArtist(paramObject.getInt("subArtistId"));
                        item = device.getGenre(subArtistItem);
                    }
                    */

                    if (param.contains("genreName")) {
                        String genreName = paramObject.getString("genreName");
                        if(genreName!=null)
                        {
                            List<String> genres = device.getALLGennre();


                            for(int i=0;i<genres.size();i++) {
                                if (genres.get(i).equals(genreName))
                                {

                                    MediaDescriptionCompat.Builder builder = new MediaDescriptionCompat.Builder();
                                    Bundle extras = new Bundle();
                                    builder = generateMediaGenreDescription(builder, extras,genreName, i);
                                    return new MediaBrowserCompat.MediaItem(builder.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE);
                                }
                            }

                        }
                    }
                    break;
                }
                case "getPlaylist": {
                    //{"method":"getPlaylist","param":{"playlistId":1}}
                    //{"method":"getPlaylist","param":{"categoryType":1,"playlistName":""}}
                   /* if (param.contains("playlistId")) {
                        item = device.getPlaylist(paramObject.getInt("playlistId"));
                    } else if (param.contains("categoryType") && param.contains("playlistName")) {
                        item = device.getPlaylist(paramObject.getInt("categoryType"), paramObject.getString("playlistName"));
                    }
                    */
                    break;
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }

      /*  if (item != null) {
            MediaDescriptionCompat.Builder builder = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            builder = generateMediaDescription(builder, extras,item, null);
            return new MediaBrowserCompat.MediaItem(builder.build(),
                    item.getItemType() == MediaItem.ITEM_TYPE_FILE ? MediaBrowserCompat.MediaItem.FLAG_PLAYABLE : MediaBrowserCompat.MediaItem.FLAG_BROWSABLE);
        }
        */

        return null;
    }

    @WorkerThread
    public static List<MediaBrowserCompat.MediaItem> search(/*final IStorageDevice device,*/ String query) {
        List<MediaBrowserCompat.MediaItem> results = new ArrayList<>();
        IPodDevice device = IPodDeviceMannager.getActiveDevice();
        List<String> listArtist = null;
        List<String> listAlum = null;
        List<String> listGenre = null;
        if (device == null) return results;

	  Log.i(TAG,"search 222 query ="+query);
      //  FileGroup fileGroup = null;
      //  List<FileItem> fileItemList = null;
      //  List<FolderItem> folderItemList = null;
      //  List<AlbumItem> albumItemList = null;
       // List<ArtistItem> artistItemList = null;
      //  List<GenreItem> genreItemList = null;
      //  List<PlaylistItem> playlistItemList = null;

        try {
            JSONObject jsonObject = new JSONObject(query);
            String method = jsonObject.getString("method");
            String param = jsonObject.getString("param");
            JSONObject paramObject = new JSONObject(param);
            switch (method) {
                //---------------------查询-------------------
                //歌曲名
                //(我要) 听忘情水
               //String query = "{\"method\":\"searchFiles\",\"param\":{\"albumName\":"xxx",\"genreName\":\"xxx\",\"artistName\":\"xxx\",\"songName\":\"xxx\"}}";
                case "searchFiles": {
                    String songs = null,album = null,genre = null,artist = null;
                   //{"method":"searchFiles","param":{"categoryType":1,"pattern":"饿狼传说"}}
                    //{"method":"searchFiles","param":{"albumId":1,"pattern":"饿狼传说"}}
                    //{"method":"searchFiles","param":{"genreId":1,"pattern":"情歌"}}
                    //{"method":"searchFiles","param":{"artistId":1,"pattern":"张学友"}}
                    //{"method":"searchFiles","param":{"categoryType":"1","folderId":1,"pattern":""}}

              
  				 try {
					album = paramObject.getString("albumName");
				}catch (Exception e)
				{
				
				Log.i(TAG,"get albumName error!");
					e.printStackTrace();
				}
   
				  try {
					  genre = paramObject.getString("genreName");
				  }catch (Exception e)
				  {
				    Log.i(TAG,"get genreName error!");
					  e.printStackTrace();
				  }
   
				  try {
					  artist = paramObject.getString("artistName");
				  }catch (Exception e)
				  {
				  
				  Log.i(TAG,"get artistName error!");
					  e.printStackTrace();
				  }
   
				  try {
					  songs = paramObject.getString("songName");
				  }catch (Exception e)
				  {
				   Log.i(TAG,"get songName error!");
					
					  e.printStackTrace();
				  }
                    List<MediaItem> listMediaItem = device.searchSons(songs,album,genre,artist);
					
				    if(listMediaItem==null)						
					Log.i(TAG,"search listMediaItem = null");
					else
							Log.i(TAG,"search listMediaItem.size() = "+listMediaItem.size());
                    MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
                    Bundle extras = new Bundle();
                    for (int i = 0;i<  listMediaItem.size();i++) {
                        //String title = listMediaItem.get(i);
                        item = generateSearchSongsMediaDescription(item, extras, listMediaItem.get(i),i);
                        results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
                    }


                    break;
                }
                case "searchFolders": {
                  /*  //{"method":"searchFolders","param":{"categoryType":1,"folderPath":""}}
                    if (param.contains("categoryType") && param.contains("folderPath")) {
                        folderItemList = device.searchFolders(paramObject.getInt("categoryType"), paramObject.getString("folderPath"));
                    }
                    */
                    break;
                }
                case "searchAlbums": {
                    /*//{"method":"searchAlbums","param":{"albumName":""}}
                    if (param.contains("albumName")) {
                        albumItemList = device.searchAlbums(paramObject.getString("albumName"));
                    }
                    */
                    
					  

              
                    if (paramObject!=null) {
                        List<String> listAlumbAll = device.getAllAlumb();
                        if(listAlumbAll!=null && listAlumbAll.size() > 0)
                        {
                            for(int i = 0;i<listAlumbAll.size();i++){
                                if(listAlumbAll.get(i).contains(paramObject.getString("albumName")))
                                    listAlum.add(listAlumbAll.get(i));
                            }
                        }
                        MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
                        Bundle extras = new Bundle();
                        for (int i = 0;i<  listAlum.size();i++) {
                            String title = listAlum.get(i);
                            item = generateMediaAlumbDescription(item, extras, title, i);
                            results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
                        }
                        //artistItemList = device.searchArtists(paramObject.getString("artistName"));
                    }
                    break;
                }

                //歌手名上下文识别
                //播放他的其他歌曲/播放他的摇滚歌曲/播放他的彩虹/播放他的合唱歌曲/有没有他和蔡依林的合唱歌曲
                //播放他的其他歌曲/播放他的摇滚歌曲/播放他的彩虹(内部判断)
                //播放他的合唱歌曲/有没有他和蔡依林的合唱歌曲（内部判断 或者调用外部接口）
               // query = "{\"method\":\"searchArtists\",\"param\":{\"artistName\":\"志\"}}";
                case "searchArtists": {
                    //{"method":"searchArtists","param":{"artistName":""}}
                 //   if (param.contains("artistName")) {
                  if(paramObject!=null){
                        List<String> listArtistAll = device.getAllArtist();
                         if(listArtistAll!=null && listArtistAll.size() > 0)
                         {
                             for(int i = 0;i<listArtistAll.size();i++){
                                 if(listArtistAll.get(i).contains(paramObject.getString("artistName")))
                                     listArtist.add(listArtistAll.get(i));
                             }
                         }
                        MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
                        Bundle extras = new Bundle();
                        for (int i = 0;i<  listArtist.size();i++) {
                            String title = listArtist.get(i);
                            item = generateMediaArtistDescription(item, extras, title, i);
                            results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
                        }
                        //artistItemList = device.searchArtists(paramObject.getString("artistName"));
                    }

                    break;
                }
                case "searchGenres": {
                    /*//{"method":"searchGenres","param":{"genreName":""}}
                    if (param.contains("genreName")) {
                        genreItemList = device.searchGenres(paramObject.getString("genreName"));
                    }
                    */
                  //  if (param.contains("genreName")) {
                   /* if (param.contains("genreName")) */ if(paramObject!=null){
                        List<String> listGenrebAll = device.getALLGennre();
                        if(listGenrebAll!=null && listGenrebAll.size() > 0)
                        {
                            for(int i = 0;i<listGenrebAll.size();i++){
                                if(listGenrebAll.get(i).contains(paramObject.getString("genreName")))
                                    listGenre.add(listGenrebAll.get(i));
                            }
                        }
                        MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
                        Bundle extras = new Bundle();
                        for (int i = 0;i<  listGenre.size();i++) {
                            String title = listGenre.get(i);
                            item = generateMediaGenreDescription(item, extras, title, i);
                            results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
                        }
                        //artistItemList = device.searchArtists(paramObject.getString("artistName"));
                    }
                    break;
                }
                case "searchPlaylists": {
                  /*  //{"method":"searchPlaylists","param":{"categoryType":1,"playlistName":""}}
                    if (param.contains("categoryType") && param.contains("playlistName")) {
                        playlistItemList = device.searchPlaylists(paramObject.getInt("categoryType"), paramObject.getString("playlistName"));
                    }
                    */
                    break;
                }
                case "searchPlaylistEntries": {
                /*    //{"method":"searchPlaylistEntries","param":{"playlistId":1,"pattern":""}}
                    //{"method":"getPlaylistEntries","param":{"categoryType":1,"playlistName":"","pattern":""}}
                    if (param.contains("pattern")) {
                        if (param.contains("playlistId")) {
                            fileGroup = device.searchPlaylistEntries(device.getPlaylist(paramObject.getInt("playlistId")),
                                    paramObject.getString("pattern"));
                        } else if (param.contains("categoryType") && param.contains("playlistName")) {
                            fileGroup = device.searchPlaylistEntries(device.getPlaylist(paramObject.getInt("categoryType"),
                                    paramObject.getString("playlistName")), paramObject.getString("pattern"));
                        }
                    }*/
                    break;
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
/*
        if (fileGroup != null && !fileGroup.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (FileItem fileItem : fileGroup.getAll()) {
                item = generateMediaDescription(item, extras, fileItem, fileGroup.getGroupItem());
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_PLAYABLE));
            }
        } else if (fileItemList != null && !fileItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (FileItem fileItem : fileItemList) {
                item = generateMediaDescription(item, extras, fileItem, null);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_PLAYABLE));
            }
        } else if (folderItemList != null && !folderItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (FolderItem folderItem : folderItemList) {
                item = generateMediaDescription(item, extras, folderItem, null);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
            }
        } else if (albumItemList != null && !albumItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (AlbumItem albumItem : albumItemList) {
                item = generateMediaDescription(item, extras, albumItem, null);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
            }
        } else if (artistItemList != null && !artistItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (ArtistItem artistItem : artistItemList) {
                item = generateMediaDescription(item, extras, artistItem, null);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
            }
        } else if (genreItemList != null && !genreItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (GenreItem genreItem : genreItemList) {
                item = generateMediaDescription(item, extras, genreItem, null);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
            }
        } else if (playlistItemList != null && !playlistItemList.isEmpty()) {
            MediaDescriptionCompat.Builder item = new MediaDescriptionCompat.Builder();
            Bundle extras = new Bundle();
            for (Play listItem playlistItem : playlistItemList) {
                item = generateMediaDescription(item, extras, playlistItem, null);
                results.add(new MediaBrowserCompat.MediaItem(item.build(), MediaBrowserCompat.MediaItem.FLAG_BROWSABLE));
            }
        }
*/
        return results;
    }

    @WorkerThread
    public static Bundle customAction(/*final IStorageDevice device, */String action) {
    
	Log.i(TAG,"customAction 222 action ="+action);
         IPodDevice device = IPodDeviceMannager.getActiveDevice();
        if (device == null) return null;

        try {
            JSONObject jsonObject = new JSONObject(action);
            String method = jsonObject.getString("method");
            String param = jsonObject.getString("param");
            JSONObject paramObject = new JSONObject(param);
            switch (method) {
                case "getLastPlayValue": {
                    //{"method":"getLastPlayValue","param":{"categoryType":1}}
                  /*  if (param.contains("categoryType")) {
                        LastPlayValue lastPlayValue = device.getLastPlayValue(paramObject.getInt("categoryType"));
                        if (lastPlayValue != null) {
                            Bundle bundle = new Bundle();
                            bundle.putInt("categoryType", lastPlayValue.getCategoryType());
                            bundle.putInt("time", lastPlayValue.getTime());
                            bundle.putInt("length", lastPlayValue.getLength());
                            bundle.putString("filePath", lastPlayValue.getFilePath());
                            bundle.putString("videoImage", lastPlayValue.getVideoImage());
                            bundle.putString("folderPath", lastPlayValue.getFolderPath());
                            bundle.putString("album", lastPlayValue.getAlbum());
                            bundle.putString("artist", lastPlayValue.getArtist());
                            bundle.putString("genre", lastPlayValue.getGenre());
                            bundle.putString("playlist", lastPlayValue.getPlaylist());
                            return bundle;
                        }
                    }
                    */
                    break;
                }
                case "setLastPlayValue": {
                    //{"method":"setLastPlayValue","param":{"categoryType":4,"folderPath":"","filePath":""}}
                    //{"method":"setLastPlayValue","param":{"categoryType":4,"playlist":"","filePath":""}}
                   /* if (param.contains("categoryType") && param.contains("filePath")) {
                        if (param.contains("folderPath")) {
                            if (paramObject.getInt("categoryType") == 4)
                                device.setLastPlayValue(LastPlayValue.createImageLastPlayValueFromFolder(paramObject.getString("folderPath"),
                                        paramObject.getString("filePath")));
                        } else if (param.contains("playlist")) {
                            if (paramObject.getInt("categoryType") == 4)
                                device.setLastPlayValue(LastPlayValue.createImageLastPlayValueFromFolder(paramObject.getString("playlist"),
                                        paramObject.getString("filePath")));
                        }
                    }
                    */
                    break;
                }
                case "clearLastPlayValue": {
                    //{"method":"clearLastPlayValue","param":{"categoryType":1}}
                 /*   if (param.contains("categoryType")) {
                        device.clearLastPlayValue(paramObject.getInt("categoryType"));
                    }*/
                    break;
                }
                case "getFilesCount": {
                    //{"method":"getFiles","param":{"albumId":1}}
                    //{"method":"getFiles","param":{"genreId":1}}
                    //{"method":"getFiles","param":{"artistId":1}}
                    //{"method":"getFiles","param":{"categoryType":1,"folderId":1,"deep":true}}
                    //{"method":"getFiles","param":{"categoryType":1}}
                 /*   int filesCount = 0;
                    if (param.contains("albumId")) {
                        filesCount = device.getFilesCount(device.getAlbum(paramObject.getInt("albumId")));
                    } else if (param.contains("genreId")) {
                        filesCount = device.getFilesCount(device.getGenre(paramObject.getInt("genreId")));
                    } else if (param.contains("artistId")) {
                        filesCount = device.getFilesCount(device.getArtist(paramObject.getInt("artistId")));
                    } else if (param.contains("categoryType")) {
                        if (param.contains("folderId")) {
                            FolderItem folderItem = device.getFolder(paramObject.getInt("categoryType"), paramObject.getInt("folderId"));
                            filesCount = device.getFilesCount(folderItem, paramObject.getBoolean("deep"));
                        } else {
                            filesCount = device.getFilesCount(paramObject.getInt("categoryType"));
                        }
                    }
                    Bundle bundle = new Bundle();
                    bundle.putInt("filesCount", filesCount);

                    return bundle;
                    */
                }
                case "getFoldersCount": {
                    //{"method":"getFoldersCount","param":{"categoryType":1,"folderId":1}}
                  /*  if (param.contains("categoryType") && param.contains("folderId")) {
                        FolderItem folderItem = device.getFolder(paramObject.getInt("categoryType"), paramObject.getInt("folderId"));
                        int foldersCount = device.getFoldersCount(folderItem);
                        Bundle bundle = new Bundle();
                        bundle.putInt("foldersCount", foldersCount);
                        return bundle;
                    }
                    */
                    break;
                }
                case "createPlaylist": {
                    //{"method":"getPlaylistEntriesCount","param":{"categoryType":1,"playlistName":""}}
             /*       if (param.contains("categoryType") && param.contains("playlistName")) {
                        PlaylistItem playlistItem = device.createPlaylist(paramObject.getInt("categoryType"),
                                paramObject.getString("playlistName"));
                        if (playlistItem != null) {
                            Bundle bundle = new Bundle();
                            bundle.putInt("playlistId", (int) playlistItem.getId());
                            bundle.putString("playlistName", playlistItem.getTitle());
                            return bundle;
                        }
                    }
                    */
                    break;
                }
                case "destroyPlaylist": {
                    //{"method":"destroyPlaylist","param":{"playlistId":1}}
                 /*   if (param.contains("playlistId")) {
                        PlaylistItem playlistItem = device.getPlaylist(paramObject.getInt("playlistId"));
                        device.destroyPlaylist(playlistItem);
                    }
                    */
                    break;
                }
                case "updatePlaylistName": {
                    //{"method":"getPlaylistEntriesCount","param":{"playlistId":1,"newPlaylistName":""}}
                   /* if (param.contains("playlistId") && param.contains("newPlaylistName")) {
                        PlaylistItem playlistItem = device.getPlaylist(paramObject.getInt("playlistId"));
                        device.updatePlaylistName(playlistItem, paramObject.getString("newPlaylistName"));
                        Bundle bundle = new Bundle();
                        bundle.putInt("playlistId", paramObject.getInt("playlistId"));
                        return bundle;

                    }
                    */
                    break;
                }
                case "getPlaylistEntriesCount": {
                    //{"method":"getPlaylistEntriesCount","param":{"playlistId":1}}
                   /* if (param.contains("playlistId")) {
                        PlaylistItem playlistItem = device.getPlaylist(paramObject.getInt("playlistId"));
                        int playlistEntriesCount = device.getPlaylistEntriesCount(playlistItem);
                        Bundle bundle = new Bundle();
                        bundle.putInt("playlistEntriesCount", playlistEntriesCount);
                        return bundle;
                    }
                    */
                    break;
                }
                case "addPlaylistEntry": {
                    //{"method":"addPlaylistEntry","param":{"playlistId":1,"fileId":1}}
                   /* if (param.contains("playlistId") && param.contains("fileId")) {
                        PlaylistItem playlistItem = device.getPlaylist(paramObject.getInt("playlistId"));
                        FileItem fileItem = device.getFile(paramObject.getInt("fileId"));
                        device.addPlaylistEntry(playlistItem, fileItem);
                    }
                    */
                    break;
                }
                case "removePlaylistEntry": {
                    //{"method":"removePlaylistEntry","param":{"playlistId":1,"fileId":1}}
                   /* if (param.contains("playlistId") && param.contains("fileId")) {
                        PlaylistItem playlistItem = device.getPlaylist(paramObject.getInt("playlistId"));
                        FileItem fileItem = device.getFile(paramObject.getInt("fileId"));
                        device.removePlaylistEntry(playlistItem, fileItem);
                    }
                    */
                    break;
                }
                case "removePlaylistEntries": {
                    //{"method":"removePlaylistEntrys","param":{"playlistId":1}}
                   /* if (param.contains("playlistId")) {
                        PlaylistItem playlistItem = device.getPlaylist(paramObject.getInt("playlistId"));
                        device.removePlaylistEntries(playlistItem);
                    }
                    */
                    break;
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }

        return null;
    }



    public static MediaDescriptionCompat.Builder generateMediaAlumbDescription(@NonNull MediaDescriptionCompat.Builder builder, @NonNull Bundle extras,
                                                                                String title,int id) {
		Log.i(TAG,"generateMediaAlumbDescription 222 title ="+title);
                                                                                
        builder.setTitle(title);
        builder.setMediaId(String.valueOf(id));
        builder.setIconUri(null);
        extras.putInt("categoryType",  MediaItem.CATEGORY_TYPE_AUDIO);
        extras.putInt("itemType", MediaItem.ITEM_TYPE_ALBUM);
        //  if (parentItem != null) {
        //   extras.putInt("parentItemType", parentItem.getItemType());
        // extras.putInt("parentItemId", (int) parentItem.getId());
        //  } else {
        extras.remove("parentItemType");
        extras.remove("parentItemId");
        // }
        // if (mediaItem instanceof FileItem && mediaItem.getCategoryType() == MediaItem.CATEGORY_TYPE_AUDIO) {
        //String genre = ((FileItem) mediaItem).getGenre();
        //  String artist = ((FileItem) mediaItem).getArtist();
        //  String album = ((FileItem) mediaItem).getAlbum();
        //  extras.putString("genre", genre == null ? "" : genre);
        // extras.putString("artist", artist == null ? "" : artist);
        //  extras.putString("album", album == null ? "" : album);
        // } else {
        extras.remove("genre");
        extras.remove("artist");
        extras.remove("album");
        //  }
        builder.setExtras(extras);

        return builder;
    }


    public static MediaDescriptionCompat.Builder generateMediaGenreDescription(@NonNull MediaDescriptionCompat.Builder builder, @NonNull Bundle extras,
                                                                                String title,int id) {
        builder.setTitle(title);
        builder.setMediaId(String.valueOf(id));
        builder.setIconUri(null);
        extras.putInt("categoryType",  MediaItem.CATEGORY_TYPE_AUDIO);
        extras.putInt("itemType", MediaItem.ITEM_TYPE_GENRE);
        //  if (parentItem != null) {
        //   extras.putInt("parentItemType", parentItem.getItemType());
        // extras.putInt("parentItemId", (int) parentItem.getId());
        //  } else {
        extras.remove("parentItemType");
        extras.remove("parentItemId");
        // }
        // if (mediaItem instanceof FileItem && mediaItem.getCategoryType() == MediaItem.CATEGORY_TYPE_AUDIO) {
        //String genre = ((FileItem) mediaItem).getGenre();
        //  String artist = ((FileItem) mediaItem).getArtist();
        //  String album = ((FileItem) mediaItem).getAlbum();
        //  extras.putString("genre", genre == null ? "" : genre);
        // extras.putString("artist", artist == null ? "" : artist);
        //  extras.putString("album", album == null ? "" : album);
        // } else {
        extras.remove("genre");
        extras.remove("artist");
        extras.remove("album");
        //  }
        builder.setExtras(extras);

        return builder;
    }
    public static MediaDescriptionCompat.Builder generateMediaArtistDescription(@NonNull MediaDescriptionCompat.Builder builder, @NonNull Bundle extras,
                                                                                String title,int id) {
                                                                                
																				Log.i(TAG,"generateMediaArtistDescription 222 title ="+title);
        builder.setTitle(title);
        builder.setMediaId(String.valueOf(id));
        builder.setIconUri(null);
        extras.putInt("categoryType",  MediaItem.CATEGORY_TYPE_AUDIO);
        extras.putInt("itemType", MediaItem.ITEM_TYPE_ARTIST);
      //  if (parentItem != null) {
         //   extras.putInt("parentItemType", parentItem.getItemType());
           // extras.putInt("parentItemId", (int) parentItem.getId());
      //  } else {
            extras.remove("parentItemType");
            extras.remove("parentItemId");
       // }
       // if (mediaItem instanceof FileItem && mediaItem.getCategoryType() == MediaItem.CATEGORY_TYPE_AUDIO) {
            //String genre = ((FileItem) mediaItem).getGenre();
          //  String artist = ((FileItem) mediaItem).getArtist();
          //  String album = ((FileItem) mediaItem).getAlbum();
          //  extras.putString("genre", genre == null ? "" : genre);
           // extras.putString("artist", artist == null ? "" : artist);
          //  extras.putString("album", album == null ? "" : album);
       // } else {
            extras.remove("genre");
            extras.remove("artist");
            extras.remove("album");
      //  }
        builder.setExtras(extras);

        return builder;
    }
    public static MediaDescriptionCompat.Builder generateSearchSongsMediaDescription(MediaDescriptionCompat.Builder builder,
                                                                           Bundle extras, MediaItem mediaItem,int id){
                                                                           
																		   Log.i(TAG,"generateSearchSongsMediaDescription 222 ");
        builder.setTitle(mediaItem.getMediaItemTitle());
        builder.setMediaId(String.valueOf(id));
        builder.setMediaUri(null);
        builder.setIconUri(null);
        extras.putInt("categoryType",  MediaItem.CATEGORY_TYPE_AUDIO);
        extras.putInt("itemType",MediaItem.ITEM_TYPE_FILE);


            extras.remove("parentItemType");
            extras.remove("parentItemId");
            String  genre = mediaItem.getMediaItemGenre();
            String album = mediaItem.getMediaItemAlbumTitle();
            String artist = mediaItem.getMediaItemArtist();

            extras.putString("genre", genre == null ? "" : genre);
            extras.putString("artist", artist == null ? "" : artist);
            extras.putString("album", album == null ? "" : album);

        builder.setExtras(extras);

        return builder;
    }
    public static MediaDescriptionCompat.Builder generateMediaDescription(@NonNull MediaDescriptionCompat.Builder builder, @NonNull Bundle extras, MediaItem mediaItem, MediaItem parentItem) {
      /*  builder.setTitle(getMediaTitle(mediaItem));
        builder.setMediaId(String.valueOf(mediaItem.getId()));
        if (mediaItem.getItemType() == MediaItem.ITEM_TYPE_FILE)
            builder.setMediaUri(((FileItem) mediaItem).getUri());
        else if (mediaItem.getItemType() == MediaItem.ITEM_TYPE_DIR)
            builder.setMediaUri(Uri.parse(((FolderItem) mediaItem).getPath()));
        if (mediaItem.getArtworkMrl() != null) {
            builder.setIconUri(Uri.parse(mediaItem.getArtworkMrl()));
        } else {
            builder.setIconUri(null);
        }
        extras.putInt("categoryType", mediaItem.getCategoryType());
        extras.putInt("itemType", mediaItem.getItemType());
        if (parentItem != null) {
            extras.putInt("parentItemType", parentItem.getItemType());
            extras.putInt("parentItemId", (int) parentItem.getId());
        } else {
            extras.remove("parentItemType");
            extras.remove("parentItemId");
        }
        if (mediaItem instanceof FileItem && mediaItem.getCategoryType() == MediaItem.CATEGORY_TYPE_AUDIO) {
            String genre = ((FileItem) mediaItem).getGenre();
            String artist = ((FileItem) mediaItem).getArtist();
            String album = ((FileItem) mediaItem).getAlbum();
            extras.putString("genre", genre == null ? "" : genre);
            extras.putString("artist", artist == null ? "" : artist);
            extras.putString("album", album == null ? "" : album);
        } else {
            extras.remove("genre");
            extras.remove("artist");
            extras.remove("album");
        }
        builder.setExtras(extras);
*/
        return builder;

    }
}
