package com.example.txl.tool;


import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.ExpandableListView;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;

import com.example.txl.tool.custom.BannerActivity;
import com.example.txl.tool.custom.DragViewActivity;
import com.example.txl.tool.gank.io.video.MoreTextureActivity;
import com.example.txl.tool.gank.io.video.VideoActivity;
import com.example.txl.tool.gesture.GestureActivity;
import com.example.txl.tool.inter.process.communication.IPCActivity;
import com.example.txl.tool.inter.process.communication.User;
import com.example.txl.tool.recycler.RecyclerViewActivity;
import com.example.txl.tool.sqlite.OpenSQLiteActivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends BaseActivity{

    public static String TAG = MainActivity.class.getSimpleName();

    private ExpandableListView expandableListView;
    private SimpleExpandableListAdapter adapter;

    String[] groupKeys = {"group"};
    HashMap<String, String> groupItemsMap;
    List<Map<String,?>> groupData = new ArrayList<>(  );
    List<List<Map<String, ?>>> childData= new ArrayList<>(  );

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_main );
        initView();
        initData();
    }

    private void initData(){
        //数据库
        groupItemsMap = new HashMap<>(  );
        groupItemsMap.put( "group", getResources().getString( R.string.SQLite ) );
        groupData.add( groupItemsMap );
        String[] SQLiteItems = getResources().getStringArray( R.array.SQLiteItem );
        List<Map<String,?>> groupList = new ArrayList(  );
        for(int i=0; i<SQLiteItems.length; i++){
            HashMap<String, String> map = new HashMap(  );
            map.put( "item", SQLiteItems[i]);
            //可以配置action来使，元素被点击读取action
            map.put( "action",SQLiteItems[i] );
            groupList.add( map );
        }
        childData.add(groupList );

        //ExpandableListView的使用
        groupItemsMap = new HashMap<>(  );
        groupItemsMap.put( "group", getResources().getString( R.string.expandableListView ) );
        groupData.add( groupItemsMap );
        String[] expandable = getResources().getStringArray( R.array.ExpandableListView );
        List<Map<String,?>> expandableListViewList = new ArrayList(  );
        for(int i=0; i<expandable.length; i++){
            HashMap<String, String> map = new HashMap(  );
            map.put( "item", expandable[i]);
            map.put( "action","" );
            expandableListViewList.add( map );
        }
        childData.add(expandableListViewList );

        //RecyclerView
        groupItemsMap = new HashMap<>(  );
        groupItemsMap.put( "group", getResources().getString( R.string.RecyclerView ) );
        groupData.add( groupItemsMap );
        String[] recycler = getResources().getStringArray( R.array.RecyclerView );
        List<Map<String,?>> recyclerViewList = new ArrayList(  );
        for(int i=0; i<recycler.length; i++){
            HashMap<String, String> map = new HashMap(  );
            map.put( "item", recycler[i]);
            map.put( "action",recycler[i] );
            recyclerViewList.add( map );
        }
        childData.add(recyclerViewList );

        groupItemsMap = new HashMap<>(  );
        groupItemsMap.put( "group", getResources().getString( R.string.GestureDetector ) );
        groupData.add( groupItemsMap );
        String[] gestureDetectors = getResources().getStringArray( R.array.GestureDetector );
        List<Map<String,?>> gestureDetectorsList = new ArrayList(  );
        for(int i=0; i<gestureDetectors.length; i++){
            HashMap<String, String> map = new HashMap(  );
            map.put( "item", gestureDetectors[i]);
            //可以配置action来使，元素被点击读取action
            map.put( "action",gestureDetectors[i] );
            gestureDetectorsList.add( map );
        }
        childData.add(gestureDetectorsList );

        //视频
        groupItemsMap = new HashMap<>(  );
        groupItemsMap.put( "group", getResources().getString( R.string.Video ) );
        groupData.add( groupItemsMap );
        String[] videos = getResources().getStringArray( R.array.Video );
        List<Map<String,?>> videoList = new ArrayList(  );
        for(int i=0; i<videos.length; i++){
            HashMap<String, String> map = new HashMap(  );
            map.put( "item", videos[i]);
            //可以配置action来使，元素被点击读取action
            map.put( "action",videos[i] );
            videoList.add( map );
        }
        childData.add(videoList );

        //跨进程通信
        groupItemsMap = new HashMap<>(  );
        groupItemsMap.put( "group", getResources().getString( R.string.IPC ) );
        groupData.add( groupItemsMap );
        String[] IPC = getResources().getStringArray( R.array.IPC );
        List<Map<String,?>> ipcList = new ArrayList(  );
        for(int i=0; i<IPC.length; i++){
            HashMap<String, String> map = new HashMap(  );
            map.put( "item", IPC[i]);
            //可以配置action来使，元素被点击读取action
            map.put( "action",IPC[i] );
            ipcList.add( map );
        }
        childData.add(ipcList );

        //自定义view
        groupItemsMap = new HashMap<>(  );
        groupItemsMap.put( "group", getResources().getString( R.string.CustomView ) );
        groupData.add( groupItemsMap );
        String[] customView = getResources().getStringArray( R.array.CustomView );
        List<Map<String,?>> customViewList = new ArrayList(  );
        for(int i=0; i<customView.length; i++){
            HashMap<String, String> map = new HashMap(  );
            map.put( "item", customView[i]);
            //可以配置action来使，元素被点击读取action
            map.put( "action",customView[i] );
            customViewList.add( map );
        }
        childData.add(customViewList );

        //断点续传

        adapter.notifyDataSetChanged();
    }

    private void initView() {
        expandableListView = findViewById( R.id.expandableListView );
        TextView emptyTextView = new TextView( this );
        emptyTextView.setText( "暂无数据" );
        emptyTextView.setTextSize( 30 );
        emptyTextView.setTextColor( Color.RED );
        expandableListView.setEmptyView( emptyTextView );
//        expandableListView.setGroupIndicator( null );
        adapter = new SimpleExpandableListAdapter(this,groupData,R.layout.expandablelistview_group,
                groupKeys, new int[]{R.id.expandable_group_textview},childData,R.layout.expandablelistview_item,new String[]{"item"},new int[]{R.id.expandable_item_textView});
        expandableListView.setAdapter( adapter );
        expandableListView.setOnChildClickListener( new ExpandableListView.OnChildClickListener() {
            @Override
            public boolean onChildClick(ExpandableListView parent, View v, int groupPosition, int childPosition, long id) {
                Log.d( TAG,"expandableListView onChildClick parent = "+parent+" v = "+v+" groupPosition = "+groupPosition+" childPosition = "+childPosition+" id = "+id );
                String action = (String) childData.get( groupPosition ).get( childPosition ).get( "action" );
                switch (action){
                    case "CreateOrOpen":
                        startActivity( OpenSQLiteActivity.class );
                        return true;
                    case "Insert":
                        return true;
                    case "Update":
                        return true;

                    case "StaggeredGridLayoutManager":
                        Intent staggeredIntent = new Intent(  );
                        staggeredIntent.putExtra( "manager","StaggeredGridLayoutManager" );
                        ComponentName staggeredcomponentName = new ComponentName( MainActivity.this, RecyclerViewActivity.class);
                        staggeredIntent.setComponent( staggeredcomponentName );
                        startActivityByIntent( staggeredIntent );
                        return true;
                    case "LinearLayoutManager":
                        Intent linearIntent = new Intent(  );
                        linearIntent.putExtra( "manager","LinearLayoutManager" );
                        ComponentName componentName = new ComponentName( MainActivity.this, RecyclerViewActivity.class);
                        linearIntent.setComponent( componentName );
                        startActivityByIntent( linearIntent );
                        break;
                    case "GridLayoutManager":
                        Intent gridIntent = new Intent(  );
                        gridIntent.putExtra( "manager","GridLayoutManager" );
                        ComponentName gridComponentName = new ComponentName( MainActivity.this, RecyclerViewActivity.class);
                        gridIntent.setComponent( gridComponentName );
                        startActivityByIntent( gridIntent );
                        break;
                    case "GestureDetector":
                        startActivity( GestureActivity.class );
                        break;
                    case "TextureView":
                        startActivity( VideoActivity.class);
                        break;
                    case "MoreTextureView":
                        startActivity( MoreTextureActivity.class);
                        break;
                    case "Bundle":
                        Intent bundleIntent = new Intent( MainActivity.this, IPCActivity.class );
                        bundleIntent.putExtra( "msg","我是来自Bundle的消息" );
                        startActivityByIntent( bundleIntent );
                        break;
                    case "File":
                        new Thread( new Runnable() {
                            @Override
                            public void run() {
                                User user = new User( 1,"文件共享",false);
                                File cacheFileDir = getDiskCacheDir( MainActivity.this,"file_ipc" );
                                if(!cacheFileDir.exists()){
                                    cacheFileDir.mkdirs();
                                }
                                File cacheFile = new File( cacheFileDir.getPath()+File.separator+"user1" );
                                ObjectOutputStream objectOutputStream = null;
                                try {
                                    objectOutputStream = new ObjectOutputStream( new FileOutputStream( cacheFile ) );
                                    objectOutputStream.writeObject( user );
                                    objectOutputStream.close();
                                    Log.d( TAG,"persist user: "+user );
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }finally {
                                   runOnUiThread( new Runnable() {
                                       @Override
                                       public void run() {
                                           Intent fileIntent = new Intent( MainActivity.this, IPCActivity.class );
                                           startActivityByIntent( fileIntent );
                                       }
                                   } );
                                }
                            }
                        } ).start();
                        break;
                    case "messenger":
                    case "AIDL":
                    case "ContentProvider":
                    case "Socket":
                        break;
                    case "DragTextView":
                        startActivity( DragViewActivity.class );
                        break;
                    case "BannerView":
                        startActivity( BannerActivity.class );
                        break;

                }
                return false;
            }
        } );
    }

    public File getDiskCacheDir(Context context, String uniqueName){
        boolean externalStorageAvailable = Environment.getExternalStorageState().equals( Environment.MEDIA_MOUNTED );
        final String cachePath;
        if(externalStorageAvailable){
            cachePath = context.getExternalCacheDir().getPath();
        }else {
            cachePath = context.getCacheDir().getPath();
        }
        return new File( cachePath+File.separator+uniqueName );
    }
}
