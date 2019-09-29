package com.example.txl.tool.fragment

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.example.txl.tool.R

class FragmentDemoActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_fragment_demo)
        val fragmentTransaction = supportFragmentManager.beginTransaction()
        fragmentTransaction.commit()
    }
}
