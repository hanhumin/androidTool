package com.example.txl.tool

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.content.res.Configuration
import android.os.Build
import android.os.Bundle
import android.util.DisplayMetrics
import android.util.Log
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.NotificationCompat
import com.example.txl.tool.activity.FlexboxLayoutActivity
import com.example.txl.tool.activity.http.HttpCookieDemoActivity
import com.example.txl.tool.activity.point9.Point9Activity
import com.example.txl.tool.activity.slider.SliderActivity
import com.example.txl.tool.bluetooth.BluetoothActivity
import com.example.txl.tool.bluetooth.BluetoothSocketDemoActivity
import com.example.txl.tool.broadcast.BroadCastReceiverActivity
import com.example.txl.tool.contentprovider.DemoContentProviderActivity
import com.example.txl.tool.coordinatorlayout.CoordinatorLayoutNavigationActivity
import com.example.txl.tool.custom.TestCustomRadioButtonDrawActivity
import com.example.txl.tool.draworder.DrawOrderActivity
import com.example.txl.tool.gif.GifActivity
import com.example.txl.tool.glide.GlideDemoActivity
import com.example.txl.tool.handler.HandlerDemoActivity
import com.example.txl.tool.inter.process.communication.AidlDemoActivity
import com.example.txl.tool.ipod.IpodActivity
import com.example.txl.tool.jetpack.lifecycle.LifecycleActivity
import com.example.txl.tool.jetpack.livedata.LiveDataActivity
import com.example.txl.tool.keyevent.KeyEventDemoActivity
import com.example.txl.tool.mediaprovider.MediaProviderActivity
import com.example.txl.tool.mediasession.MediaSessionVideoPlayActivity
import com.example.txl.tool.okhttp.OkHttpDemoActivity
import com.example.txl.tool.partition.storage.StorageDemoActivity
import com.example.txl.tool.radiobuttonwithgif.RadioButtonGifActivity
import com.example.txl.tool.recyclerView.ItemDecorationDemoActivity
import com.example.txl.tool.recyclerView.RecyclerViewDemoActivity
import com.example.txl.tool.resimage.TestResImageActivity
import com.example.txl.tool.rxjava.RxJavaDemoActivity
import com.example.txl.tool.service.ServiceDemoActivity
import com.example.txl.tool.twmediaplayer.TwMediaPlayerTestActivity
import com.example.txl.tool.usb.UsbActivity
import com.example.txl.tool.window.WindowAndWindowManagerActivity
import com.hjq.permissions.OnPermissionCallback
import com.hjq.permissions.Permission
import com.hjq.permissions.XXPermissions
import com.kotlinlearn.demo.xiecheng.CoroutineDemoActivity

class NavigationActivity : AppCompatActivity(), View.OnClickListener {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_navigation)
        var displayMetrics:DisplayMetrics=resources.displayMetrics
        getWindowManager().getDefaultDisplay().getRealMetrics(displayMetrics)
        Log.d("App", "sh: ${resources.configuration.screenHeightDp}   NavigationActivity density: " + displayMetrics.density + " densityDpi " + displayMetrics.densityDpi + " width " + displayMetrics.widthPixels + "  height " + displayMetrics.heightPixels)
        initView()
        XXPermissions.with(this)
            .permission(Permission.WRITE_EXTERNAL_STORAGE)
            .permission(Permission.READ_EXTERNAL_STORAGE)
            .request(object : OnPermissionCallback {
                override fun onGranted(permissions: List<String?>?, all: Boolean) {
                    if (!all) {
                        Toast.makeText(this@NavigationActivity, "?????????????????????????????????????????????????????????", Toast.LENGTH_SHORT)
                            .show()
                    }
                }

                override fun onDenied(permissions: List<String?>?, never: Boolean) {
                    if (never) {
                        Toast.makeText(
                            this@NavigationActivity,
                            "???????????????????????????????????????????????????????????????",
                            Toast.LENGTH_SHORT
                        ).show()
                        // ???????????????????????????????????????????????????????????????
                    } else {
                        Toast.makeText(this@NavigationActivity, "??????????????????????????????", Toast.LENGTH_SHORT).show()
                    }
                }
            })
    }

    fun initView(){
        findViewById<View>(R.id.tv_jump_CoordinatorLayout).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_Http).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_nine_picture).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_slider).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_aidl_simple_use).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_window_and_windowmanager).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_glide_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_rxjava_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_handler_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_keyevent_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_flexbox_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_radio_button_use_gif_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_custom_radio_button_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_gif_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_recycler_view_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_okhttp_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_content_provider_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_media_player_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_image_in_other_res).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_jetpack_lifecycle).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_jetpack_livedata).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_draw_order).setOnClickListener(this)
        findViewById<View>(R.id.tv_toast_notification).setOnClickListener(this)
        findViewById<View>(R.id.tv_get_usb).setOnClickListener(this)
        findViewById<View>(R.id.tv_bluetooth_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_service_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_broad_cast_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_recycler_view_demo_demo).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_ipod).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_mediasession).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_mediaProvider).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_storage).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_coroutine).setOnClickListener(this)
    }

    override fun onClick(v: View?) {
        when(v?.id){
            R.id.tv_jump_CoordinatorLayout->{
                val intent = Intent(this, CoordinatorLayoutNavigationActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_Http->{
                val intent = Intent(this, HttpCookieDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_nine_picture->{
                val intent = Intent(this, Point9Activity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_slider->{
                val intent = Intent(this, SliderActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_aidl_simple_use->{
                val intent = Intent(this, AidlDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_window_and_windowmanager->{
                val intent = Intent(this, WindowAndWindowManagerActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_glide_demo->{
                val intent = Intent(this, GlideDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_rxjava_demo->{
                val intent = Intent(this, RxJavaDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_handler_demo->{
                val intent = Intent(this, HandlerDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_keyevent_demo->{
                val intent = Intent(this, KeyEventDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_flexbox_demo->{
                val intent = Intent(this, FlexboxLayoutActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_radio_button_use_gif_demo->{
                val intent = Intent(this, RadioButtonGifActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_custom_radio_button_demo->{
                val intent = Intent(this, TestCustomRadioButtonDrawActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_gif_demo->{
                val intent = Intent(this, GifActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_recycler_view_demo->{
                val intent = Intent(this, RecyclerViewDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_okhttp_demo->{
                val intent = Intent(this, OkHttpDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_content_provider_demo->{
                val intent = Intent(this, DemoContentProviderActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_media_player_demo->{
                val intent = Intent(this, TwMediaPlayerTestActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_image_in_other_res->{
                val intent = Intent(this, TestResImageActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_jetpack_lifecycle->{
                val intent = Intent(this, LifecycleActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_jetpack_livedata->{
                val intent = Intent(this, LiveDataActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_draw_order->{
                val intent = Intent(this, DrawOrderActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_toast_notification->{

//                CrashReport.testJavaCrash();
                val manager = this@NavigationActivity.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
                var notify: Notification? = null
                val code = 5
                val intent = Intent(this@NavigationActivity, DrawOrderActivity::class.java)
                intent.flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
                val pendingIntent3 = PendingIntent.getActivity(this@NavigationActivity.getApplicationContext(), code, intent, PendingIntent.FLAG_CANCEL_CURRENT)
                val builder = NotificationCompat.Builder(this@NavigationActivity.getApplicationContext())
                        .setSmallIcon(R.drawable.ic_launcher_background)
                        .setContentTitle("tttt")
                        .setContentText("ssssss")
                        .setContentIntent(pendingIntent3)
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    val channel = NotificationChannel("to-do"
                            , "????????????",
                            NotificationManager.IMPORTANCE_HIGH)
                    channel.enableVibration(true)
                    channel.vibrationPattern = longArrayOf(500)
                    manager.createNotificationChannel(channel)
                    builder.setChannelId("to-do")
                    notify = builder.build()
                } else {
                    notify = builder.build()
                }
                //?????????????????????
                //?????????????????????
                notify.defaults = notify.defaults or Notification.DEFAULT_SOUND
//                notify.sound = Uri.parse("android.resource://" + HomeActivityBase.this.getPackageName() + "/" + R.raw.doorbell);
                //                notify.sound = Uri.parse("android.resource://" + HomeActivityBase.this.getPackageName() + "/" + R.raw.doorbell);
                notify.defaults = notify.defaults or Notification.DEFAULT_VIBRATE
                notify.flags = notify.flags or Notification.FLAG_AUTO_CANCEL // ???????????????????????????????????????????????????????????????


                manager.notify(code, notify) // ??????4????????????????????????????????????????????????id???????????????click??????status????????????????????????

            }
            R.id.tv_get_usb->{
                val intent = Intent(this, UsbActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_bluetooth_demo->{
                val intent = Intent(this, BluetoothSocketDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_service_demo->{
                val intent = Intent(this, ServiceDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_broad_cast_demo->{
                val intent = Intent(this, BroadCastReceiverActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_recycler_view_demo_demo->{
                val intent = Intent(this, ItemDecorationDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_ipod->{
                val intent = Intent(this, IpodActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_mediasession->{
                val intent = Intent(this, MediaSessionVideoPlayActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_mediaProvider->{
                val intent = Intent(this, MediaProviderActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_storage->{
                val intent = Intent(this, StorageDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_coroutine->{
                val intent = Intent(this, CoroutineDemoActivity::class.java)
                startActivity(intent)
            }
        }
    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        Log.d("NavigationActivity","onConfigurationChanged")
        super.onConfigurationChanged(newConfig)
    }

    override fun onDestroy() {
        super.onDestroy()
        Log.d("NavigationActivity","onDestroy")
    }
}
