package com.example.txl.tool.activity;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.example.txl.tool.R;

public class NestedScrollingDemoActivity extends AppCompatActivity {

    RecyclerView recyclerView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_nested_scrolling_demo);
        recyclerView = findViewById(R.id.rv_recycler);
        recyclerView.setLayoutManager(new LinearLayoutManager(this,LinearLayoutManager.VERTICAL,false));
        recyclerView.setAdapter(new RecyclerView.Adapter() {
            @Override
            public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
                TextView textView = new TextView(NestedScrollingDemoActivity.this);
                ViewGroup.LayoutParams params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,60);
                textView.setLayoutParams(params);
                return new MyViewHoler(textView);
            }

            @Override
            public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
                MyViewHoler myViewHoler = (MyViewHoler) holder;
                myViewHoler.textView.setText("我是第"+position+"个");
            }

            @Override
            public int getItemCount() {
                return 60;
            }
        });
    }

    class MyViewHoler extends RecyclerView.ViewHolder{
        TextView textView;
        public MyViewHoler(View itemView) {
            super(itemView);
            textView = (TextView) itemView;
        }
    }
}
