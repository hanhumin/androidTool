package com.example.txl.tool;


import android.content.ComponentName;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ExpandableListView;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;

import com.example.txl.tool.recycler.RecyclerViewActivity;
import com.example.txl.tool.sqlite.OpenSQLiteActivity;

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

                }
                return false;
            }
        } );
    }
}
