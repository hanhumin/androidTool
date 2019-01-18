package com.example.txl.tool.activity;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.example.txl.tool.R;
import com.txl.lib.wiget.BadgeView;

public class HecoderActivty extends AppCompatActivity {
    BadgeView badgeView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hecoder_activty);
        badgeView = findViewById(R.id.badge_view);
        badgeView.setImageRes(R.drawable.ic_messages_like_selected);
        badgeView.setImagePadding(10);

//        badgeView.postDelayed(new Runnable() {
//            @Override
//            public void run() {
//                badgeView.setImageRes(R.drawable.content);
//            }
//        },5000);
    }
}
