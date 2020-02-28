package com.example.txl.tool.custom

import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.example.txl.tool.R
import kotlinx.android.synthetic.main.activity_test_custom_radio_button_draw.*

class TestCustomRadioButtonDrawActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_test_custom_radio_button_draw)
    }

    var change = false

    fun changeColor(view: View) {
        if(change){
            test.setCircleTitleBgColor(Color.WHITE)
        }else{
            test.setCircleTitleBgColor(Color.RED)
        }
        change = !change
    }
}
