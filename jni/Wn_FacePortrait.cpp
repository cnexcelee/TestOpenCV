#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Wn_FacePortrait.h"
#include "Wn_Log.h"

using namespace cv;
using namespace std;

Mat ExtractFaceSkin(Size size, int nRed, int nGreen, int nBlue) {
	Mat mRed = Mat::ones(size, CV_8U) * nRed;
	Mat mGreen = Mat::ones(size, CV_8U) * nGreen;
	Mat mBlue = Mat::ones(size, CV_8U) * nBlue;

	vector<Mat> channels;
	channels.push_back(mBlue);
	channels.push_back(mGreen);
	channels.push_back(mRed);
	Mat mMerge;
	merge(channels, mMerge);
#ifdef SAVE_IMAGE
	imwrite("faceColor.jpg", mMerge);
#endif
	return mMerge;
}
/// nAlpha:0-255
Mat MergeImages(Mat mFront, Mat mBack, int nAlpha) {
	//addWeighted(src1, alpha, src2, beta, 0.0, dst);
	return mFront * nAlpha / 255 + mBack * (255 - nAlpha) / 255 + Scalar(0, 0, 0);
}

void RemovePepperNoise(Mat &mask)
{
	// For simplicity, ignore the top & bottom row border.
	for (int y = 2; y < mask.rows - 2; y++) {
		// Get access to each of the 5 rows near this pixel.
		uchar *pThis = mask.ptr(y);
		uchar *pUp1 = mask.ptr(y - 1);
		uchar *pUp2 = mask.ptr(y - 2);
		uchar *pDown1 = mask.ptr(y + 1);
		uchar *pDown2 = mask.ptr(y + 2);

		// For simplicity, ignore the left & right row border.
		pThis += 2;
		pUp1 += 2;
		pUp2 += 2;
		pDown1 += 2;
		pDown2 += 2;
		for (int x = 2; x < mask.cols - 2; x++) {
			uchar v = *pThis;   // Get the current pixel value (either 0 or 255).
			// If the current pixel is black, but all the pixels on the 2-pixel-radius-border are white
			// (ie: it is a small island of black pixels, surrounded by white), then delete that island.
			if (v == 0) {
				bool allAbove = *(pUp2 - 2) && *(pUp2 - 1) && *(pUp2) && *(pUp2 + 1) && *(pUp2 + 2);
				bool allLeft = *(pUp1 - 2) && *(pThis - 2) && *(pDown1 - 2);
				bool allBelow = *(pDown2 - 2) && *(pDown2 - 1) && *(pDown2) && *(pDown2 + 1) && *(pDown2 + 2);
				bool allRight = *(pUp1 + 2) && *(pThis + 2) && *(pDown1 + 2);
				bool surroundings = allAbove && allLeft && allBelow && allRight;
				if (surroundings == true) {
					// Fill the whole 5x5 block as white. Since we know the 5x5 borders
					// are already white, just need to fill the 3x3 inner region.
					*(pUp1 - 1) = 255;
					*(pUp1 + 0) = 255;
					*(pUp1 + 1) = 255;
					*(pThis - 1) = 255;
					*(pThis + 0) = 255;
					*(pThis + 1) = 255;
					*(pDown1 - 1) = 255;
					*(pDown1 + 0) = 255;
					*(pDown1 + 1) = 255;
				}
				// Since we just covered the whole 5x5 block with white, we know the next 2 pixels
				// won't be black, so skip the next 2 pixels on the right.
				pThis += 2;
				pUp1 += 2;
				pUp2 += 2;
				pDown1 += 2;
				pDown2 += 2;
			}
			// Move to the next pixel.
			pThis++;
			pUp1++;
			pUp2++;
			pDown1++;
			pDown2++;
		}
	}
}

void Cartoonify(Mat dst, Mat srcColor)
{
	// Convert from BGR color to Grayscale
	Mat srcGray = srcColor;
	if (3 == srcColor.channels()) {
		cvtColor(srcColor, srcGray, CV_BGR2GRAY);
	}

	// Remove the pixel noise with a good Median filter, before we start detecting edges.
	medianBlur(srcGray, srcGray, 7);

	Size size = srcColor.size();
	Mat mask = Mat(size, CV_8U);
	Mat edges = Mat(size, CV_8U);

	// Generate a nice edge mask, similar to a pencil line drawing.
	Laplacian(srcGray, edges, CV_8U, 5);
	threshold(edges, mask, 80, 255, THRESH_BINARY_INV);
	// Mobile cameras usually have lots of noise, so remove small
	// dots of black noise from the black & white edge mask.
	RemovePepperNoise(mask);

	//imshow("edges", edges);
	//imshow("mask", mask);

	cvtColor(mask, dst, CV_GRAY2BGR);
	return;
}


int DecorateFace(Mat & mDst, Mat & mSrc) {
	if (mSrc.empty()) {
		return WN_ERROR_PARAMETER;
	}

	mDst = Mat(mSrc.size(), CV_8UC3);
	Cartoonify(mDst, mSrc.clone());
#ifdef SAVE_IMAGE
	imwrite("dst.jpg", mDst);
#endif

	//face color rgb: 243 224 213
	int nRed = 243;
	int nGreen = 224;
	int nBlue = 213;
	int nAlpha = 30;
	Mat mFaceSkin = ExtractFaceSkin(mSrc.size(), nRed, nGreen, nBlue);
	Mat m = MergeImages(mDst, mFaceSkin, 30);
#ifdef SAVE_IMAGE
	imwrite("merge1.jpg", m);
#endif
	Vec3b v3 = Vec3b(0, 0, 0);
	//Vec3b v3 = Vec3b(243, 224, 213);
	Mat mFaceRgn = imread("../faceRgn.jpg", IMREAD_GRAYSCALE);
//#ifdef SAVE_IMAGE
//	imwrite("faceRgn.png", mFaceRgn);
//#endif
//	if (mFaceRgn.empty()) {
//		return WN_ERROR_FAIL;
//	}
//	vector<Mat> mFaceRgnChannels;
//	split(mFaceRgn, mFaceRgnChannels);
//	mFaceRgn = mFaceRgnChannels[3];
#ifdef SAVE_IMAGE
	imwrite("faceRgn.jpg", mFaceRgn);
#endif
	int nMask = 0;
	for (int nRow = 0; nRow < mFaceRgn.rows; nRow ++) {
		for (int nCol = 0; nCol < mFaceRgn.cols; nCol ++) {
			if (nMask == mFaceRgn.at<unsigned char>(nRow, nCol)) {
				m.at<Vec3b>(nRow, nCol) = Vec3b(0, 0, 0);
			}
		}
	}
#ifdef SAVE_IMAGE
	imwrite("merge2.jpg", m);
#endif
	vector<Mat> mChannels;
	split(m, mChannels);
	vector<Mat> m4Channels;
	m4Channels.push_back(mChannels[0]);
	m4Channels.push_back(mChannels[1]);
	m4Channels.push_back(mChannels[2]);
	m4Channels.push_back(mFaceRgn);
	merge(m4Channels, m);
#ifdef SAVE_IMAGE
	imwrite("faceWithAlpha.png", m);
#endif

	mDst = m;
	return WN_ERROR_NONE;
}
