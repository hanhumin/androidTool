package com.example.txl.tool.twmediaplayer

import android.content.Context
import android.content.res.AssetFileDescriptor
import android.media.AudioFocusRequest
import android.media.AudioManager
import android.media.MediaMetadata
import android.media.MediaPlayer
import android.media.session.MediaSession
import android.media.session.PlaybackState
import android.os.Build
import android.os.Bundle
import android.os.SystemClock
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.example.txl.tool.R
import com.example.txl.tool.utils.AppExecutors
import kotlinx.android.synthetic.main.activity_tw_media_player_test.*


class TwMediaPlayerTestActivity : AppCompatActivity() {
val tag = TwMediaPlayerTestActivity::class.java.simpleName
    val mediaPlayer = MediaPlayer()
//    var mediaSession:MediaSession? = null
//    var mAudioManager: AudioManager? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_tw_media_player_test)
//         mAudioManager = getSystemService(Context.AUDIO_SERVICE) as AudioManager
//        mAudioManager?.requestAudioFocus(AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN)
//                .setOnAudioFocusChangeListener { focusChange -> Log.d(tag, "onAudioFocusChange $focusChange") }
//                .build())
        val afd: AssetFileDescriptor = resources.assets.openFd("test_video.mp4")
//        val afd: AssetFileDescriptor = resources.assets.openFd("test_bluetooth.mp3")
        mediaPlayer.setOnErrorListener(object :MediaPlayer.OnErrorListener{
            override fun onError(mp: MediaPlayer?, what: Int, extra: Int): Boolean {
                Log.e(tag,"onerror what :: $what  extra :: $extra")
                return true
            }
        })
        mediaPlayer.setOnPreparedListener(object :MediaPlayer.OnPreparedListener{
            override fun onPrepared(mp: MediaPlayer?) {
                mediaPlayer.start()
//                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
//                    mediaSession = MediaSession(this@TwMediaPlayerTestActivity, "test")
//                    mediaSession?.setActive(true)
//                    mediaSession?.setMetadata(MediaMetadata.Builder()
//                            .putString(MediaMetadata.METADATA_KEY_TITLE, "?????????????????????")
//                            .putString(MediaMetadata.METADATA_KEY_TITLE, "?????????????????????lalalla")
//                            .build())
//                    AppExecutors().networkIO().execute {
//                        var i =0;
//                        while (true){
//                            Log.d(tag,"update PlaybackState  "+mediaPlayer.currentPosition)
//                            mediaSession?.setPlaybackState(PlaybackState.Builder()
//                                    .setState(PlaybackState.STATE_PLAYING, mediaPlayer.currentPosition.toLong(), 1.0f)
//                                    .build())
//                            mediaSession?.setMetadata(MediaMetadata.Builder()
//                                    .putString(MediaMetadata.METADATA_KEY_TITLE, "?????????????????????"+i)
//                                    .putString(MediaMetadata.METADATA_KEY_AUTHOR, "???????????????")
//                                    .putLong(MediaMetadata.METADATA_KEY_DURATION, mediaPlayer.duration.toLong())
//                                    .build())
//                            SystemClock.sleep(500)
//                            i++;
//                            if(i>500){
//                                break
//                            }
//                        }
//                    }
//                }
            }
        })
        mediaPlayer.setOnInfoListener(object :MediaPlayer.OnInfoListener{
            override fun onInfo(mp: MediaPlayer?, what: Int, extra: Int): Boolean {
                when (what) {
                    MediaPlayer.MEDIA_INFO_BUFFERING_START -> {
                        Log.e(tag,"????????????????????????")
                    }
                    MediaPlayer.MEDIA_INFO_BUFFERING_END -> {
                        Log.e(tag,"????????????????????????")
                    }
                }
                return false
            }
        })
        surface_view.postDelayed({
            mediaPlayer.setDisplay(surface_view.holder)
            mediaPlayer.setDataSource(afd.fileDescriptor,afd.startOffset,afd.declaredLength)
//            mediaPlayer.setDataSource(afd.fileDescriptor)
            Log.e(tag,"afd :: ${afd.fileDescriptor}")
//            mediaPlayer.isLooping = true
//            mediaPlayer.setDataSource("http://116.77.76.10:5000/nn_vod/nn_x64/aWQ9NmVlYWYyNTU0M2M0NDkyNTc3NTVmZmY5NmFkOGEwNmQmdXJsX2MxPTczNzA1ZjczNzM3YTcxMmYzMjMwMzIzMDJmMzAzNDJmMzIzMjJmNjYzMTM5MzgzMTMyNjU2MjYxNjI2MTY2MzQzOTMwMzg2MjM1MzczNzM3NjIzNTMyMzUzMzMzMzkzMDMyMzQ2MzJmNjYz/MTM5MzgzMTMyNjU2MjYxNjI2MTY2MzQzOTMwMzg2MjM1MzczNzM3NjIzNTMyMzUzMzMzMzkzMDMyMzQ2MzJlNmQzMzc1MzgwMCZubl9haz0wMTViYjIyODY0MjU0NWI4ZDhhN2ZkM2YxNzE2ZDQyNWY1Jm5waXBzPTE3Mi4yNy4xNy43NTo1MTAwJm5jbXNpZD0xMDAw/MDEwMyZuZ3M9NWViZDM5NDIwMDAyNjE1MmIwMGUwMzQ1NDhmN2JiMTkmbm5kPXR3bmV0Lmd1YW5nZG9uZy5zaGVuJm5zZD16Z2R4LnNoZW56aGVuJm5mdD1tM3U4Jm5uX3VzZXJfaWQ9NTdiZWI3Mzk3YWU5OGFhMjBlNWNmYWFmYWJmNjUwMGQmbmR0PXN0YiZuZGk9/ODA3NTU4ODAwOTg4NzEyMyZuZHY9MS40LjE2LlNUQi5GVFRILlNURC5EVkJfU0tXMDEuUmVsZWFzZSZuc3Q9aXB0diZuY2E9JTI2bmFpJTNkZjE5ODEyZWJhYmFmNDkwOGI1Nzc3YjUyNTMzOTAyNGMlMjZubl9jcCUzZHNwX3NzenEmbmFsPTAxNDIzOWJkNWUwNjA3/NGYwNDBmY2QyZTM0MDU0MTlhZTk3MTEwNzUzYTI4NzM,/6eeaf25543c449257755fff96ad8a06d.m3u8")
            mediaPlayer.prepareAsync()
        },500)



    }

    override fun onStop() {
        super.onStop()
        mediaPlayer.release()
//        mediaSession?.release()
    }
}
