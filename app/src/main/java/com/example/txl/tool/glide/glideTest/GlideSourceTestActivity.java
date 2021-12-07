package com.example.txl.tool.glide.glideTest;

import android.app.Activity;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentTransaction;

import com.bumptech.glide.Glide;
import com.bumptech.glide.load.DataSource;
import com.bumptech.glide.load.engine.DiskCacheStrategy;
import com.bumptech.glide.load.engine.GlideException;
import com.bumptech.glide.request.RequestListener;
import com.bumptech.glide.request.target.Target;
import com.example.txl.tool.R;
import com.example.txl.tool.glide.AudioAssetUriLoader;


//测试Glide是否会发生内存泄漏
/**
 * 1. activity级别泄漏  在生命周期结束后  对应的request  target如何处理相关的引用关系？  会不会有activity view泄漏呢？ view泄漏应该会对应着activity泄漏
 * 2. fragment级别泄漏  如果错用activity作为with对象的参数  此时fragment应该不会泄漏，但是View会不会泄漏呢？
 * 3. Glide 是否会存在图片错乱？ 不会  ImageViewTarget
 * */
public class GlideSourceTestActivity extends AppCompatActivity {
    private static final String  TAG = GlideSourceTestActivity.class.getSimpleName();
    private TextView tvLoadImg, tvActivityLeak, tvFragmentLeak;
    private ImageView showImage;
    static Activity mA = null;
    static View mV = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_glide_leak_test);
        AudioAssetUriLoader.init(this);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        initView();
        //强制内存内泄漏
//        mA = this;
    }

    private void initView() {
        showImage = findViewById(R.id.image);
//        mV = showImage;
        tvLoadImg = findViewById(R.id.tvLoadImage);
        tvLoadImg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //这个会在加载额度时候进行休眠因此速度更慢
                Glide.with(GlideSourceTestActivity.this).load(Uri.parse("file:///android_asset/DuiMianDeNvHaiKanGuoLai--RenXianQi.mp3")).diskCacheStrategy(DiskCacheStrategy.NONE).into(showImage);
                Glide.with(GlideSourceTestActivity.this).load(Uri.parse("file:///android_asset/LiRongHao-MaQue.mp3")).diskCacheStrategy(DiskCacheStrategy.NONE).into(showImage);
            }
        });
        tvActivityLeak = findViewById(R.id.tvActivityLeak);
        //activity级别内存泄漏
        //注意需要先杀进程再重新独立测试
        tvActivityLeak.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Glide.with(GlideSourceTestActivity.this).load(Uri.parse("file:///android_asset/DuiMianDeNvHaiKanGuoLai--RenXianQi.mp3")).diskCacheStrategy(DiskCacheStrategy.NONE).into(showImage);
                tvActivityLeak.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        finish();
                    }
                },30);

            }
        });

        tvFragmentLeak = findViewById(R.id.tvFragmentLeak);
        tvFragmentLeak.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                FragmentTransaction fragmentTransaction = getSupportFragmentManager().beginTransaction();
                MyTestFragment myTestFragment = new MyTestFragment();
                fragmentTransaction.add(R.id.frame,myTestFragment).commit();
            }
        });
    }

    public static class MyTestFragment extends Fragment {
        ImageView imageView;
        @Nullable
        @Override
        public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
            View root = inflater.inflate(R.layout.fragment_glide_source_test,container,false);
            imageView = root.findViewById(R.id.imageView);
            //强制保留引用关系
            root.setTag(this);
            Log.d(TAG,"onCreateView finish");
            return root;
        }

        @Override
        public void onCreate(@Nullable Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
        }

        @Override
        public void onActivityCreated(@Nullable Bundle savedInstanceState) {
            super.onActivityCreated(savedInstanceState);
            Glide.with(getActivity())
                    .load(Uri.parse("file:///android_asset/DuiMianDeNvHaiKanGuoLai--RenXianQi.mp3"))
                    .diskCacheStrategy(DiskCacheStrategy.NONE).
                    addListener(new RequestListener<Drawable>() {
                @Override
                public boolean onLoadFailed(@Nullable GlideException e, Object model, Target<Drawable> target, boolean isFirstResource) {
                    Log.d(TAG,"MyTestFragment onLoadFailed");
                    return false;
                }

                @Override
                public boolean onResourceReady(Drawable resource, Object model, Target<Drawable> target, DataSource dataSource, boolean isFirstResource) {
                    Log.d(TAG,"MyTestFragment onResourceReady");
                    return false;
                }
            }).into(imageView);
            Log.d(TAG,"onActivityCreated load ");
            imageView.postDelayed(new Runnable() {
                @Override
                public void run() {
                    Log.d(TAG,"onActivityCreated remove ");
                    getActivity().getSupportFragmentManager().beginTransaction().remove(MyTestFragment.this).commit();
                }
            },300);

        }

        @Override
        public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
            super.onViewCreated(view, savedInstanceState);
        }
    }
}