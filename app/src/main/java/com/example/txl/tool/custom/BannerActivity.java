package com.example.txl.tool.custom;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.example.txl.tool.R;
import com.example.txl.tool.banner.CycleBannerStrategy;
import com.example.txl.tool.banner.FlexibleBannerView;

public class BannerActivity extends AppCompatActivity {

    FlexibleBannerView bannerView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_banner );
        initView();
    }

    private void initView() {
        bannerView = findViewById(R.id.FlexibleBannerView);
        bannerView.setBannerStyle( CycleBannerStrategy.NAME);
        bannerView.setAutoPlay( true );
        bannerView.setViewCreator(new FlexibleBannerView.IViewCreator() {
            @Override
            public int getCount() {
                return 3;
            }

            @Override
            public View createView(int position) {
                TextView textView = new TextView(BannerActivity.this);
                textView.setText("我是第"+position+"个元素");
                return textView;
            }
        });
        bannerView.start( 1 );
    }

    public void changeBannerStyle(View view) {
        bannerView.setBannerStyle( CycleBannerStrategy.NAME);
    }
}
