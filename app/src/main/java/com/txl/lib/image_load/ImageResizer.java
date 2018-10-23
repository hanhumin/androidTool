package com.txl.lib.image_load;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import java.io.FileDescriptor;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/6
 * description：图片压缩
 */
public class ImageResizer {
    private static final String TAG = "ImageResizer";

    public ImageResizer() {
    }

    public Bitmap decodeSampledBitmapFromResource(Resources res, int resId, int reqWidth, int reqHeight){
        final BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;
        BitmapFactory.decodeResource( res,resId,options );

        //Calculate inSampleSize
        options.inSampleSize = calculateInSampleSize(options,reqWidth,reqHeight);

        //Decode bitmap with inSampleSize set
        options.inJustDecodeBounds = false;
        Bitmap bitmap =BitmapFactory.decodeResource( res, resId, options );
        Log.d( TAG,"decodeSampledBitmapFromResource,w="+bitmap.getWidth()+" h="+bitmap.getHeight() +"原来 req,w="+reqWidth+" h="+reqHeight);
        return bitmap;
    }

    public Bitmap decodeSampledBitmapFromFileDescriptor(FileDescriptor fd, int reqWidth, int reqHeight){
        final BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;
        BitmapFactory.decodeFileDescriptor( fd, null, options );
        //Calculate inSampleSize
        options.inSampleSize = calculateInSampleSize(options,reqWidth,reqHeight);

        //Decode bitmap with inSampleSize set
        options.inJustDecodeBounds = false;
        Bitmap bitmap = BitmapFactory.decodeFileDescriptor( fd, null, options );
        return bitmap;
    }

    public int calculateInSampleSize(BitmapFactory.Options options, int reqWidth, int reqHeight){
        if(reqWidth == 0 && reqHeight == 0){
            return 1;
        }

        //raw height and width of image
        final int height = options.outHeight;
        final int width = options.outWidth;
        Log.d( TAG,"origin,w="+width+" h="+height );
        Log.d( TAG,"原来 req,w="+reqWidth+" h="+reqHeight );
        if(reqWidth == 0){
            reqWidth = (int) (reqHeight*1.0*width/height);
        }
        if(reqHeight == 0){
            reqHeight = (int) (reqWidth*1.0*height/width);
        }
        Log.d( TAG,"new req,w="+reqWidth+" h="+reqHeight );
        int inSampleSize = 1;
        if(height > reqHeight || width>reqWidth){
            final int halfHeight = height/2;
            final int halfWidth = width/2;

            /**
             * Calculate the largest inSampleSize value that is a power of 2 and keeps both
             * height and width larger than the requested height and width
             * */
            while ((halfHeight/inSampleSize) >= reqHeight && (halfWidth/inSampleSize) >= reqWidth){
                inSampleSize *= 2;
            }
        }
        Log.d( TAG,"sampleSize:"+inSampleSize );
        return inSampleSize;
    }
}
