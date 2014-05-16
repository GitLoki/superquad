#include <iostream>
#include <fstream>
#include <cmath>
#include "../../include/Kinect/Kinect.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv) {

    bool die(false);
    string filename("snapshot");
    string suffix(".png");
    int i_snap = 0;

    Mat depthMat(Size(640,480), CV_16UC1);
    Mat depthf  (Size(640,480), CV_8UC1);
    Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
    Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));

    Freenect::Freenect freenect;
    MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);

    namedWindow("rgb",CV_WINDOW_AUTOSIZE);
    namedWindow("depth",CV_WINDOW_AUTOSIZE);
    device.startVideo();
    device.startDepth();
    while (!die) {
	device.getVideo(rgbMat);
	device.getDepth(depthMat);
	cv::imshow("rgb", rgbMat);

	depthMat.convertTo(depthf, CV_8UC1, 1.0/8.03);

	cv::imshow("depth",depthf);

	char k = cvWaitKey(5);

	//key 27 = esc to break out of loop
	if( k == 27 ){
	    die = true;
	}

	// uncomment for debugging:
	/* 
	// find out what key was pressed and print it out
	 if( k != 27 && k != -1 ) {
	   cout << int(k) << " is " << k << endl;
	 }
	*/

	// key 32 == space
	if( k == 32 ){ 
	    std::ostringstream file;
	    file << filename << i_snap << "_rgb" << suffix;
	    cout << "You just saved out: " << file.str();
	    cv::imwrite(file.str(),rgbMat);

	    file.str(""); // clear the string stream
	    file << filename << i_snap << "_dep" << suffix;
	    cout << " and " << file.str() << endl;
	    cv::imwrite(file.str(),depthf);
	    i_snap++;
	}
    }

    device.stopVideo();
    device.stopDepth();
    return 0;
}
