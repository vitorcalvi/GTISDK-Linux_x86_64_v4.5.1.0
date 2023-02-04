/***********************************************************************
*
* Copyright (c) 2017-2019 Gyrfalcon Technology Inc. All rights reserved.
* See LICENSE file in the project root for full license information.
*
************************************************************************/

package gti.gtidemo;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.ImageView;
import android.view.View;
import android.view.View.OnClickListener;
import android.util.Log;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.content.pm.PackageManager;

import android.Manifest;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;
import java.lang.String;

import android.os.Environment;

public class MainActivity extends Activity {

    Button button_next;
    Button button_previous;
    ImageView image;
    private Activity _activity;
    String imageFolder = Environment.getExternalStorageDirectory() + "/gti/Image_bmp/";

    final File folder = new File(imageFolder);
    ArrayList<File> imageList = new ArrayList<File> ();
    int gIndex = 0;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if( ! folder.exists() ) {
            Log.d("gtiDemoActivity", "imageFolder not found!");
        }

        if(ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(MainActivity.this, new String[]{
                    Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    Manifest.permission.READ_EXTERNAL_STORAGE
            }, 1);
        }

        Log.d("gtiDemoActivity", "Loading gti_classify library.");
        System.loadLibrary("gti_classify");
        Log.d("gtiDemoActivity", "Loaded gti_classify library.");

        initEngine(1, 0); // (gNet type, demo type)
        // gNet type: 0 = gNet1, 1 = gNet3,
        // demo type: 0 = Image, 1 = Video, 2 = Camera

        Log.d("gtiDemoActivity", "initEngine done.");

        addListenerOnButton();

        Log.d("gtiDemoActivity", "addListenerOnButton done.");

        for (final File fileEntry : folder.listFiles()) {
            imageList.add(fileEntry);
            Log.d("gtiDemoActivity", imageFolder + fileEntry.getName());
        }

        gIndex = 0;
        if(imageList.size() > 0)
            decodeAndClassifyImage(imageFolder + imageList.get(gIndex).getName());

        Log.d("gtiDemoActivity", "onCreate done.");

    }

    public void decodeAndClassifyImage(String imagename)
    {
        String[] predictions;
        Bitmap bitmap;

        final int[] imgData = new int[224 * 224];
        final byte[] rgbData = new byte[224 * 224 * 3];

        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inPreferredConfig = Bitmap.Config.ARGB_8888;
        options.outWidth = 224;
        options.outHeight = 224;

        bitmap = BitmapFactory.decodeFile(imagename, options);

        Bitmap scaledBitmap = Bitmap.createScaledBitmap(bitmap, 224, 224, true);

        image.setImageBitmap(scaledBitmap);

        scaledBitmap.getPixels(imgData, 0, 224, 0, 0, 224, 224);

        int offset1;
        int offset2;
        int offset3;

        //Convert pxiels to BGR plane format
        for(int i = 0; i < 224; i++) {
            offset1 = 224 * i;
            offset2 = 50176 + offset1;
            offset3 = 100352 + offset1;
            for (int j = 0; j < 224; j++)  //BGR
            {
                /* BGR */
                rgbData[offset1 + j] = (byte)((imgData[offset1 + j]) & 0xFF);
                rgbData[offset2 + j] = (byte)((imgData[offset1 + j] >> 8) & 0xFF);
                rgbData[offset3 + j] = (byte)((imgData[offset1 + j] >> 16) & 0xFF);

                /* RGB -- Newer Models uses RGB instead of BGR
                rgbData[offset3 + j] = (byte)((imgData[offset1 + j]) & 0xFF);
                rgbData[offset2 + j] = (byte)((imgData[offset1 + j] >> 8) & 0xFF);
                rgbData[offset1 + j] = (byte)((imgData[offset1 + j] >> 16) & 0xFF);
                */
            }
        }

        predictions = oneFramePredict(rgbData);

        TextView txtView1 = (TextView) findViewById(R.id.textView1);
        TextView txtView2 = (TextView) findViewById(R.id.textView2);

        char[] dispText1 = predictions[0].toCharArray();
        char[] dispText2 = predictions[1].toCharArray();
        int alphaStart = 0;

        if(predictions[0].length() > 10 && predictions[1].length() > 10) {

            for (alphaStart = 20; alphaStart < predictions[0].length(); alphaStart++) {
                if ((dispText1[alphaStart] >= 'a' && dispText1[alphaStart] <= 'z') ||(dispText1[alphaStart] >= 'A' && dispText1[alphaStart] <= 'Z'))
                    break;
            }
            txtView1.setText(predictions[0].subSequence(0, 6) + " -- " + predictions[0].subSequence(alphaStart, predictions[0].length()));

            Log.d("gtiDemoActivity", predictions[0]);
            Log.d("gtiDemoActivity", predictions[1]);
            //  Log.d(TAG, predictions[0].subSequence(0, 6) + " -- " + predictions[0].subSequence(alphaStart, predictions[0].length()));

            for (alphaStart = 20; alphaStart < predictions[1].length(); alphaStart++) {
                if ((dispText2[alphaStart] >= 'a' && dispText2[alphaStart] <= 'z') ||(dispText2[alphaStart] >= 'A' && dispText2[alphaStart] <= 'Z'))
                    break;
            }
            txtView2.setText(predictions[1].subSequence(0, 6) + " -- " + predictions[1].subSequence(alphaStart, predictions[1].length()));
            // Log.d(TAG, predictions[1].subSequence(0, 6) + " -- " + predictions[1].subSequence(alphaStart, predictions[1].length()));
        }
    }


    public void addListenerOnButton() {

        image = (ImageView) findViewById(R.id.imageView1);
        button_next = (Button) findViewById(R.id.btnNext);
        button_previous = (Button) findViewById(R.id.btnPrevious);

        button_next.setOnClickListener(new OnClickListener() {
            private LayoutInflater inflater;

            @Override

            public void onClick(View arg0) {
                gIndex++;
                if(gIndex == imageList.size())
                    gIndex = 0;
                decodeAndClassifyImage(imageFolder + imageList.get(gIndex).getName());
            }
        });

        button_previous.setOnClickListener(new OnClickListener() {
            private LayoutInflater inflater;

            @Override

            public void onClick(View arg0) {
                gIndex--;
                if(gIndex < 0)
                    gIndex = imageList.size() - 1;
                decodeAndClassifyImage(imageFolder + imageList.get(gIndex).getName());
            }
        });

    }
    public native void initEngine(int network, int type);
    public native String[] oneFramePredict(byte[] imgData);
}
