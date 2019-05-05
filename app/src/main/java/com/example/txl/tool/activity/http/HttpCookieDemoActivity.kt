package com.example.txl.tool.activity.http

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.text.TextUtils
import android.util.Log
import android.view.View
import android.widget.EditText
import android.widget.TextView
import com.example.txl.tool.R
import com.example.txl.tool.api.WanAndroidApi
import org.json.JSONObject
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response

class HttpCookieDemoActivity : AppCompatActivity() {
    private var wanAndroidApi: WanAndroidApi.IWanAndroidAPI = WanAndroidApi.getiWanAndroidAPI()
    private lateinit var mEdUserName: EditText
    private lateinit var mEdUserPassword: EditText
    private lateinit var mTvContent: TextView

    val TAG = HttpCookieDemoActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_http_cookie_demo)
        initView()
    }

    fun initView() {
        mEdUserName = findViewById(R.id.ed_user_name)
        mEdUserPassword = findViewById(R.id.ed_user_password)
        findViewById<View>(R.id.tv_confirm).setOnClickListener {
            if (!TextUtils.isEmpty(mEdUserName.text) && !TextUtils.isEmpty(mEdUserPassword.text)) {
                wanAndroidApi.userLogin(mEdUserName.text.toString(), mEdUserPassword.text.toString()).enqueue(object : Callback<JSONObject>{
                    override fun onFailure(call: Call<JSONObject>, t: Throwable) {
                        Log.d(TAG,"onFailure  ${t.message}")
                    }

                    override fun onResponse(call: Call<JSONObject>, response: Response<JSONObject>) {
                        mTvContent.text = "${response.body()}"
                    }
                })
            }
        }
        mTvContent = findViewById(R.id.tv_content)
    }

}
