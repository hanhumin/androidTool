package com.example.txl.tool.sqlite;

import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

import com.example.txl.tool.BaseActivity;
import com.example.txl.tool.R;
import com.example.txl.tool.utils.FileUtils;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;

public class OpenSQLiteActivity extends BaseActivity implements View.OnClickListener{

    private String TAG = OpenSQLiteActivity.class.getSimpleName();

    private Button submitButton;
    private EditText editText;
    private ListView containListView;
    private ArrayAdapter<String> adapter;
    private List<String> dbs;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_open_sqlite );
        initView();

    }

    private void initView(){
        submitButton = findViewById( R.id.open_sqlite_submit_button );
        submitButton.setOnClickListener( this );
        editText = findViewById( R.id.open_sqlite_edittext );
        containListView = findViewById( R.id.open_sqlite_contain_listview );
        containListView.addHeaderView( new View( this ) );
        containListView.addFooterView( new View( this ) );
        try {
            dbs = FileUtils.getAppDataBases(this);
        } catch (FileNotFoundException e) {
            Log.d( TAG,"getAppDataBases FileNotFoundException" );
            e.printStackTrace();
            dbs = new ArrayList<>(  );
        }
        adapter = new ArrayAdapter<String>( this, R.layout.expandablelistview_item,dbs);
        containListView.setAdapter( adapter );
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.open_sqlite_submit_button:
                String name = editText.getText().toString().trim();
                if(TextUtils.isEmpty( name )){
                    Toast.makeText( this,"请检查要创建的数据库名称",Toast.LENGTH_SHORT ).show();
                    return;
                }else {
                    MySQLiteOpenHelper mySQLiteOpenHelper = new MySQLiteOpenHelper( this,name,null,1);
                    SQLiteDatabase sqLiteDatabase = mySQLiteOpenHelper.getWritableDatabase();

                    if(sqLiteDatabase != null){
                        if(dbs.contains( name )||dbs.contains( name+".db" )){
                            Toast.makeText( this,String.format( "数据库%s已存在",name ),Toast.LENGTH_SHORT ).show();
                        }else {
                            dbs.add( name );
                            adapter.notifyDataSetChanged();
                            Toast.makeText( this,String.format( "创建%s数据库成功",name ),Toast.LENGTH_SHORT ).show();
                        }
                    }
                    if(sqLiteDatabase != null && sqLiteDatabase.isOpen()){
                        sqLiteDatabase.close();
                    }
                }
        }
    }
}
