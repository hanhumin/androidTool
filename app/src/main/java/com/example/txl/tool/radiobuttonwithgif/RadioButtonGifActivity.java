package com.example.txl.tool.radiobuttonwithgif;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import android.graphics.drawable.Drawable;
import android.graphics.drawable.StateListDrawable;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.RadioButton;

import com.bumptech.glide.Glide;
import com.bumptech.glide.load.DataSource;
import com.bumptech.glide.load.engine.GlideException;
import com.bumptech.glide.load.resource.gif.GifDrawable;
import com.bumptech.glide.request.RequestListener;
import com.bumptech.glide.request.target.CustomTarget;
import com.bumptech.glide.request.target.Target;
import com.bumptech.glide.request.transition.Transition;
import com.example.txl.tool.R;

public class RadioButtonGifActivity extends AppCompatActivity {

    private StateListDrawable stateListDrawable;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_radio_button_gif );
        stateListDrawable = new StateListDrawable();
        final int[] a = {android.R.attr.state_checked};
        final int[] b = {};
        final ImageView imageView = findViewById( R.id.image_test_gif );
        Glide.with( this ).asGif().load( R.drawable.mygif ).listener( new RequestListener<GifDrawable>() {
            @Override
            public boolean onLoadFailed(@Nullable GlideException e, Object model, Target<GifDrawable> target, boolean isFirstResource) {
                return false;
            }

            @Override
            public boolean onResourceReady(GifDrawable resource, Object model, Target<GifDrawable> target, DataSource dataSource, boolean isFirstResource) {
                stateListDrawable.addState( a,resource );
                stateListDrawable.addState( b, ContextCompat.getDrawable( RadioButtonGifActivity.this,R.drawable.ic_launcher_background ) );
                RadioButton radioButton = findViewById( R.id.radio_button1 );
                radioButton.setBackground( stateListDrawable );
                resource.start();

                return false;
            }
        } ).override( 300,300 ).into( new CustomTarget<GifDrawable>() {
            @Override
            public void onResourceReady(@NonNull GifDrawable resource, @Nullable Transition<? super GifDrawable> transition) {
//                imageView.setImageDrawable( resource );
//                resource.start();
            }

            @Override
            public void onLoadCleared(@Nullable Drawable placeholder) {

            }
        } );
//        Glide.with( this ).asGif().load( R.drawable.mygif ).into( imageView );
    }
}
