package com.example.txl.tool.coordinatorlayout;

import android.support.design.widget.AppBarLayout;
import android.support.design.widget.CoordinatorLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.example.txl.tool.R;

import java.util.ArrayList;
import java.util.List;

public class CoordinatorLayoutActivity extends AppCompatActivity {
    private static final String TAG =  CoordinatorLayoutActivity.class.getSimpleName();

    private RecyclerView recyclerView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_coording_layout);
        initView();
    }

    List<String> listItem;
    private void initView() {
        recyclerView = findViewById(R.id.list_view);
        listItem = new ArrayList<>();
        for(int i=0; i<100; i++){
            listItem.add("我是第"+i+"个元素");
        }
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        recyclerView.setAdapter(new RecyclerView.Adapter() {
            @Override
            public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
                TextView tv = (TextView) getLayoutInflater().inflate(R.layout.cordinatory_test_item, null,false);
                return new MyViewHolder(tv);
            }

            @Override
            public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
                MyViewHolder myViewHolder = (MyViewHolder)holder;
                myViewHolder.tv.setText(listItem.get(position));
            }

            @Override
            public int getItemCount() {
                return listItem.size();
            }
        });
    }

    class MyViewHolder extends RecyclerView.ViewHolder{
        TextView tv;
        public MyViewHolder(View itemView) {
            super(itemView);
            tv = (TextView) itemView;

        }
    }

}
