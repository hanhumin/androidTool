package com.example.txl.tool.coordinatorlayout;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.google.android.material.tabs.TabLayout;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentPagerAdapter;
import androidx.viewpager.widget.ViewPager;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import com.example.txl.tool.R;

public class CoordinatorLayoutActivity extends AppCompatActivity {
    private static final String TAG =  CoordinatorLayoutActivity.class.getSimpleName();

    private TabLayout tableLayout;
    private ViewPager viewPager;
    private String[] mTitles = new String[] { "简介", "评价", "相关" };
    private TableFragment[] mFragments = new TableFragment[mTitles.length];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_coording_layout);
        initView();
    }


    private void initView() {
        for (int i = 0; i < mTitles.length; i++)
        {
            mFragments[i] =  TableFragment.Companion.newInstance((mTitles[i]));
        }
        tableLayout = findViewById(R.id.table_layout);
        viewPager = findViewById(R.id.view_pager);
        viewPager.setAdapter(new FragmentPagerAdapter(getSupportFragmentManager()) {
            @Override
            public Fragment getItem(int position) {
                return mFragments[position];
            }

            @Override
            public int getCount() {
                return mFragments.length;
            }

            @Nullable
            @Override
            public CharSequence getPageTitle(int position) {
                return mTitles[position];
            }
        });
        tableLayout.setupWithViewPager(viewPager);
    }
}
