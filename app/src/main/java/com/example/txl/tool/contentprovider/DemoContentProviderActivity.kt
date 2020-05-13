package com.example.txl.tool.contentprovider

import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.example.txl.tool.R
import kotlinx.android.synthetic.main.activity_demo_content_provider.*

/**
 * 查询ContentProvider
 * */
class DemoContentProviderActivity : AppCompatActivity() {
    private val tag = DemoContentProviderActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_demo_content_provider)

        tv_same_process.setOnClickListener {
            val cursor = contentResolver.query(Uri.parse("content://com.txl.demo.content.provider"),null,null,null,null)
            Log.d(tag,"开始同进程查询  query cursor is $cursor")
            if(cursor != null){
                while (cursor.moveToNext()){
                    Log.d(tag,"query result ${cursor.columnNames}")
                }
            }
            cursor?.close()
        }
    }
}
