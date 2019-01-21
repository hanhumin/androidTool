package com.example.txl.tool.activity;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.example.txl.tool.R;
import com.txl.lib.wiget.BadgeView;

public class HecoderActivty extends AppCompatActivity {
    BadgeView badgeView;
    int num = 1;
    boolean state = true;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hecoder_activty);
        badgeView = findViewById(R.id.badge_view);
        badgeView.setImageRes(R.drawable.ic_messages_like_selected);
        badgeView.setImagePadding(15);
        badgeView.setBadgeImageSize(66,66);
        badgeView.setBadgeText(num+"");
        badgeView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                num = num * 10;
                badgeView.setBadgeText(num+"");
                int res = state ? R.drawable.ic_messages_like_selected:R.drawable.ic_messages_like_unselected;
                badgeView.setImageRes(res);
                state = !state;
            }
        });
//        badgeView.postDelayed(new Runnable() {
//            @Override
//            public void run() {
//                badgeView.setImageRes(R.drawable.content);
//            }
//        },5000);
    }
}
