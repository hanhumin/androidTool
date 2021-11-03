// IMediaSessionToken.aidl
package com.example.txl.tool;
import android.media.session.MediaSession.Token;
import android.view.Surface;
// Declare any non-default types here with import statements
interface IMediaSessionToken {
    /**
     * Demonstrates some basic types that you can use as parameters
     * and return values in AIDL.
     */
    Token getToken();
    void setSurface(in Surface surface);
}