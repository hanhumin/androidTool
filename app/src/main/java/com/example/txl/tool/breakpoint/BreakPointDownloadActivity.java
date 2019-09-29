package com.example.txl.tool.breakpoint;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.ProgressBar;

import com.example.txl.tool.R;

public class BreakPointDownloadActivity extends AppCompatActivity {

    private ProgressBar progressBar;
    private Button startButton, stopButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_break_point_download );
        progressBar = findViewById( R.id.progressBar );
        startButton = findViewById( R.id.button_start );
        stopButton = findViewById( R.id.button_stop );
    }
}
