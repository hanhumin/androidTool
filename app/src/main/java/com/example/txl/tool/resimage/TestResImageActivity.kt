package com.example.txl.tool.resimage

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity
import com.example.txl.tool.R
import kotlinx.android.synthetic.main.activity_test_res_image.*


/**
 * 测试图片在不同的资源文件夹下的内存占用情况;将 test_res_logo_xxxh  分别放置在不同的res文件中查看情况
 *
 * 模拟器为  xhdpi  从高的dpi中加载，图片会被缩小，对应的内存占用也会减少图片会失真，变模糊
 *      从低 dpi 中加载，图片会被放大，内存占用会提高;
 *      res 下的图片内存计算规则是 ：
 *      图片宽 X 图片高  X 每个像素点的大小;
 *      图片的高 = 原图高 X (设备的 dpi / 目录对应的 dpi )
 *      图片的宽 = 原图宽 X (设备的 dpi / 目录对应的 dpi )
 * */
class TestResImageActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_test_res_image)
        loadResImage(image_res_test0,R.drawable.test_res_logo_m)
        loadResImage(image_res_test1,R.drawable.test_res_logo_xh)
        loadResImage(image_res_test2,R.drawable.test_res_logo_xxxh)

        loadResImage(image_res_test0_w,R.drawable.test_res_logo_m)
        loadResImage(image_res_test1_w,R.drawable.test_res_logo_xh)
        loadResImage(image_res_test2_w,R.drawable.test_res_logo_xxxh)
        showW(true)
        Log.d("!!!!!!","density :: ${resources.displayMetrics.density}  dpi :: ${resources.displayMetrics.densityDpi}" +
                "  width :: ${resources.displayMetrics.widthPixels}  height :: ${resources.displayMetrics.heightPixels}")
    }

    /**
     * 是否显示wrap_content的 测试View
     * */
    private fun showW(show:Boolean){
        val vis = if(show){
            View.VISIBLE
        }else{
            View.GONE
        }
        image_res_test0_w.visibility = vis
        image_res_test1_w.visibility = vis
        image_res_test2_w.visibility = vis
    }

    private fun loadResImage(imageView: ImageView,res:Int,inSampleSize:Int = 1) {
        val options: BitmapFactory.Options = BitmapFactory.Options()
        options.inSampleSize = inSampleSize
        val bitmap: Bitmap = BitmapFactory.decodeResource(resources, res, options)
        //Bitmap bitmap = BitmapFactory.decodeFile("mnt/sdcard/weixin.png", options);

        imageView.setImageBitmap(bitmap)
        Log.i("!!!!!!", "${resources.getResourceName(res)}  bitmap：ByteCount = " + (bitmap.byteCount /1024f/1024f) + ":::bitmap：AllocationByteCount = " + bitmap.getAllocationByteCount())
        Log.i("!!!!!!", "${resources.getResourceName(res)}  width:" + bitmap.getWidth().toString() + ":::height:" + bitmap.getHeight())
        Log.i("!!!!!!", "${resources.getResourceName(res)}  inDensity:" + options.inDensity.toString() + ":::inTargetDensity:" + options.inTargetDensity)
        Log.i("!!!!!!", "${resources.getResourceName(res)}  imageview.width:" + imageView.getWidth().toString() + ":::imageview.height:" + imageView.getHeight())
    }
}
