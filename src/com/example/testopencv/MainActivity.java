package com.example.testopencv;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.InstallCallbackInterface;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends Activity {

	Button mButton = null;
	ImageView mImageView = null;
	static{
        if(!OpenCVLoader.initDebug()){
        	Log.i("TAG", "initDebug");
            //handle initialization error
        }else{
        	System.loadLibrary("image_process");
        	System.loadLibrary("opencv_java");
        	Log.i("TAG", "loadLibrary");
//        　System.loadLibrary("my_jni_lib2");
        }
}  
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		initUI();
	}

	public void initUI() {

		mButton = (Button) findViewById(R.id.my_btn);
		mImageView = (ImageView) findViewById(R.id.my_imv);

		mButton.setOnClickListener(new MyOnclickListener());
	}

	public void proc2Gray() {

		Mat rgbMat = new Mat();
		Mat grayMat = new Mat();

		Bitmap rgbBitmap = BitmapFactory.decodeResource(getResources(),
				R.drawable.testface);
		Bitmap grayBitmap = Bitmap.createBitmap(rgbBitmap.getWidth(),
				rgbBitmap.getHeight(), Config.RGB_565);

//		Utils.bitmapToMat(rgbBitmap, rgbMat);
//		Imgproc.cvtColor(rgbMat, grayMat, Imgproc.COLOR_RGB2GRAY);
//		Utils.matToBitmap(grayMat, grayBitmap);
		
		Mat newMat = new Mat();
		ImageProc ip = new ImageProc();
		int back = ip.DecorateFace(newMat, rgbMat);
		Utils.matToBitmap(newMat, grayBitmap);
		mImageView.setImageBitmap(grayBitmap);
		Log.i("TAG", "backbackback: "+back);
	}

	class MyOnclickListener implements OnClickListener {
		@Override
		public void onClick(View v) {
			proc2Gray();
		}

	}

	BaseLoaderCallback mCallBack = new BaseLoaderCallback(this) {

		@Override
		public void onManagerConnected(int status) {
			switch (status) {
			case BaseLoaderCallback.SUCCESS:
				Log.i("TAG", "baseloadercallback : success");
				break;
			default:
				super.onManagerConnected(status);
				Log.i("TAG", "baseloadercallback : faild");
				break;
			}

		}

	};

	@Override
	protected void onResume() {
		super.onResume();
//		 OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_9,
//		 this,mCallBack);
//		 OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_9, MainActivity.this, mCallBack);
		mCallBack.onManagerConnected(LoaderCallbackInterface.SUCCESS);
	}

}
