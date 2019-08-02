package com.example.txl.tool.window

import android.annotation.TargetApi
import android.content.Context
import android.content.Intent
import android.graphics.Color
import android.graphics.PixelFormat
import android.net.Uri
import android.os.Build
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.provider.Settings
import android.view.Gravity
import android.view.ViewGroup
import android.view.Window
import android.view.WindowManager
import android.widget.ImageView
import com.example.txl.tool.R

class WindowAndWindowManagerActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_window_and_window_manager)
        getOverlayPermission()
        addViewToWindow()
    }

    private fun addViewToWindow() {
        val image = ImageView(this)
        image.setImageResource(R.drawable.back)
        image.setBackgroundColor(Color.parseColor("#77d475"))
        val layoutParams = WindowManager.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT,WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,0,PixelFormat.TRANSPARENT)
        layoutParams.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE or WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED
        layoutParams.gravity = Gravity.LEFT or Gravity.TOP
        layoutParams.x = 100
        layoutParams.y = 300
//        val windowManager = getSystemService(Context.WINDOW_SERVICE) as WindowManager
        //如何添加到对应的Window中
        windowManager.addView(image,layoutParams)
    }

    //请求悬浮窗权限
    @TargetApi(Build.VERSION_CODES.M)
    private fun getOverlayPermission() {
        //检查是否已经授予权限
        if (Settings.canDrawOverlays(this)) {
            return
        }
        val intent = Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION)
        intent.data = Uri.parse("package:$packageName")
        startActivityForResult(intent, 0)
    }
}
