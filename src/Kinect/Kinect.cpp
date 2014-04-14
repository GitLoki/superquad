#include "../../include/Kinect/Kinect.hpp"

Kinect::Kinect():
    depthMat(new cv::Mat(cv::Size(640,480),CV_16UC1)),
    depthf(  new cv::Mat(cv::Size(640,480),CV_8UC1)),
    rgbMat(  new cv::Mat(cv::Size(640,480),CV_8UC3,cv::Scalar(0))){
}

Kinect::~Kinect(){
    delete depthMat;
    delete depthf;
    delete rgbMat;
    cv::destroyAllWindows();
}

void Kinect::save_frame(std::string filename){

    cvStartWindowThread(); // fixes the 'force quit' problem on close

    cv::namedWindow("rgb",CV_WINDOW_AUTOSIZE);
    cv::namedWindow("depth",CV_WINDOW_AUTOSIZE);

    std::string suffix(".png");
    int i_snap = 0;

    while(true){
	update(); //get new RGB and depth

	imshow("rgb", *rgbMat);
	depthMat->convertTo(*depthf, CV_8UC1, 1.0/8.03);
	imshow("depth",*depthf);

	char k = cvWaitKey(10);
	// taking a screenshot by "space"
	if( k == 32 ) { 

	    std::ostringstream file;
	    file << filename << i_snap << "_rgb" << suffix;
	    std::cout << "You just saved out: " << file.str();
	    imwrite(file.str(),*rgbMat);
    
	    file.str(""); // clear the string stream
	    file << filename << i_snap << "_dep" << suffix;
	    std::cout << " and " << file.str() << std::endl;
	    imwrite(file.str(),*depthf);
	    i_snap++;
	}

	if( k == 27 ) { 
	    // esc key was pressed
	    cvDestroyWindow("rgb");
	    cvDestroyWindow("depth");
	    return;
	}
    }
}

/* expect this function to be called inside a loop continuously */
bool Kinect::query(double& realX, double& realY, double& avgDepth) {
    if( !camera.getDepth(*depthMat) ) return false;

    depthMat->convertTo(*depthf, CV_8UC1, 1.0/8.03);

    int mmDepth;
    int sumX = 0;
    int sumY = 0;
    double sumDepth = 0;
    int count = 0;
  
    for(int y = 0 ; y < 480 ; y++) {
	for(int x = 0 ; x < 640 ; x++) {
      
	    mmDepth = rawDepthToMilimeters(depthMat->at<unsigned short>(y,x));
      
	    if(mmDepth < THRESHOLD && mmDepth != 0){	
		sumX += x;
		sumY += y;
		sumDepth += mmDepth;
		count++;
	    }
	}
    }

    if(count) {
	avgDepth = sumDepth/count;
	float avgX = (float) sumX / count;
	float avgY = (float) sumY / count;

	realX = getrealwidth(avgX, avgDepth);
	realY = getrealheight(avgY, avgDepth);
	return true;
    }
    else {
	avgDepth = 0;
	realX = 0;
	realY = 0;
	return false;
    }
}

int Kinect::rawDepthToMilimeters(int depthValue) {
    if (depthValue < 2047) {
	return (1000 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
    }
    return 0;
}

void Kinect::save_video(std::string filename, int frames){


    cv::VideoWriter writer;

    cv::namedWindow("rgb",CV_WINDOW_AUTOSIZE);
    cv::namedWindow("depth",CV_WINDOW_AUTOSIZE);

    bool truth = true;

    cv::Mat doubleImg= cv::Mat::zeros(960,640,CV_8UC3);
    cv::Mat *depth3 = new cv::Mat(cv::Size(640,480),CV_8UC3);
    cv::Mat in[] = {*depthf,*depthf,*depthf};
    cv::merge(in,3,*depth3);

    cv::Size frameSize(640,960);

    int count = 0;

    writer.open(filename, CV_FOURCC('P','I','M','1'), 20, frameSize, true);

    while(truth && count < frames){

	cv::Mat in[] = {*depthf,*depthf,*depthf};
	cv::merge(in,3,*depth3);

	update();

	imshow("rgb", *rgbMat);
	depthMat->convertTo(*depthf, CV_8UC1, 1.0/8.03);
	imshow("depth",*depthf);

	rgbMat->copyTo(doubleImg(cv::Range(0,480),cv::Range(0,640)));
	depth3->copyTo(doubleImg(cv::Range(480,960),cv::Range(0,640)));

	writer.write(doubleImg);

	if (cv::waitKey(10) == 27)
	    {
		std::cout << "esc key is pressed by user" << std::endl;
		truth = false; 
	    }
	count++;
    }

    cvDestroyWindow("rgb");
    cvDestroyWindow("depth");
    delete depth3;
}

void  Kinect::update(){
    // loop both video and depth until they have updated
    while( !camera.getDepth(*depthMat)) ;
    while( !camera.getVideo(*rgbMat)  ) ;
}

float Kinect::getrealwidth(float avgX, float depth) {
    float focal_distance =  WIDTH/(2*tan((57.0/2.0)*(PI/180.0)));
    return depth * (avgX - WIDTH / 2) / focal_distance;
}

float Kinect::getrealheight(float avgY, float depth) {
    float focal_distance =  HEIGHT/(2*tan((43.0/2.0)*(PI/180.0)));
    return -1 * depth * (avgY - HEIGHT / 2) / focal_distance;
}


//Filtered Query - hasn't been tested

bool Kinect::filtered_query(double& realX, double& realY, double& avgDepth) {
    if( !camera.getDepth(*depthMat) ) return false;

    cv::Mat *realDepthMat = new cv::Mat(cv::Size(640,480),CV_16UC1);

    depthMat->convertTo(*depthf, CV_8UC1, 1.0/8.03);

    double mmDepth;
    int sumX = 0;
    int sumY = 0;
    double sumDepth = 0;
    int count = 0;
  
    for(int y = 0 ; y < 480 ; y++) {
	for(int x = 0 ; x < 640 ; x++) {
      
	    mmDepth = rawDepthToMilimeters(depthMat->at<double>(y,x));
      
	    realDepthMat->at<double>(y,x);

		if(mmDepth < THRESHOLD && mmDepth != 0){	
		    sumDepth += mmDepth;
		    count++;
		}
	}
    }

    if(count) {

	//filtering
  
	cv::Mat closed, binary, contours;
	int morph_operator = cv::MORPH_CLOSE; //max 4

	cv::Mat kernel = cv::Mat::ones(15,15,CV_8UC1);

	//close missing areas
	cv::morphologyEx(*realDepthMat, closed, morph_operator, kernel);

	//create a binary array
	cv::threshold(closed, binary, 5, 1, cv::THRESH_BINARY);
    
	//find contours
	cv::findContours(binary, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	//use contour to calculate new centroid using moments
	cv::Moments m = moments(contours,true);
    
	//MISSING: get avgX and avgY from m
	float avgX = m.m10/m.m00;
	float avgY = m.m01/m.m00;

	avgDepth = sumDepth/count;

	realX = getrealwidth(avgX, avgDepth);
	realY = getrealheight(avgY, avgDepth);
	return true;
    }
    else {
	avgDepth = 0;
	realX = 0;
	realY = 0;
	return false;
    }
}

double Kinect::rawDepthToMilimetersDouble(int depthValue) {
    if (depthValue < 2047) {
	return (1000 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
    }
    return 0;
}


/* Code below taken from Alex's Python file and 
   transformed into C++ */
/*
  __author__ = 'az'
  import cv2
  import numpy as np
  import sys
*/
/*

  void Kinect::show(Mat* image, const string str_text, int wait_time){
  //text_params = ((30,30), cv::FONT_HERSHEY_TRIPLEX, 1, (0,0,255), 2);
  int fontFace = cv::FONT_HERSHEY_TRIPLEX;
  double fontScale = 1;
  int thickness = 3;  
  cv::Point textOrg(30, 30);
 

  Mat texty_image = image.clone();
  //cv::putText(texty_image, str_text, *text_params);
  cv::putText(texty_image, str_text, textOrg, fontFace, fontScale,
  Scalar::all(255), thickness,2);
  cv::imshow("preview", texty_image);
  if (cv::waitKey(wait_time) == 27)
  //sys.exit();
  exit (EXIT_FAILURE);
  // filename = "prepend_"+str_text+".png";
  // cv2.imwrite(filename, texty_image);
  }

  void Kinect::closed_sequence(){
  cv::namedWindow("preview");
  Mat original = cv::imread("quadcopter_images/snapshot1_dep.png");

  Mat raw = original;
  show(raw, "Raw", WAIT);

  // a value on the quadcopter (at this depth) is ~73
  // print "value", raw[275, 315] #= [0,0,255]
  // near clipping plane is 0 (quadcopter is closest point to kinect)
  // ret,threshold = cv2.threshold(original,0,255,cv2.THRESH_TOZERO)
  // far clipping plane is about 100 (ceiling shows up around 110)

  // Our Z-accuracy sucks a**. We only have about 30 'units' over our range of interest!!!
  // At the moment 30 units covers about 1.5 metres, so we have ~10cm accuracy
  //ret,threshold = cv::threshold(original,100,255,cv2.THRESH_TOZERO_INV);
  double threshold_sequence = cv::threshold(original,100,255,cv2.THRESH_TOZERO_INV);
  show(threshold_sequence, "Threshold", WAIT);

  // dilated = cv2.dilate(original,kernel,iterations = 1)
  // cv2.putText(dilated, "Dilated", *text_params)
  // cv2.imshow("preview", dilated)
  // if cv2.waitKey(WAIT) == 27: return

  // time to fiddle around with the blur kernel... a 15px blur seems good
  kernel = np.ones((15,15),np.uint8);

  // use 'closing' to fill in some of the missing regions...
  // this does a dilation followed by an erosion step
  closed = cv::morphologyEx(threshold_sequence, cv2.MORPH_CLOSE, kernel);
  show(closed, "Closed", WAIT);

  // okay, create a binary version for the contour processing
  //_, binary = cv::threshold(closed,50 ,255,cv2.THRESH_BINARY);
  double binary = cv::threshold(closed,50 ,255,cv2.THRESH_BINARY);
  show(binary, "Binary", WAIT);

  pre_contour = cv::cvtColor(binary,cv2.COLOR_BGR2GRAY);
  //contours, _ = cv::findContours(pre_contour, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE);
  contours = cv::findContours(pre_contour, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE);
  cv::drawContours(closed, contours, -1, (0, 0, 255));
  show(closed, "Contour(s)", WAIT);

  // so, now we've got a contour
  // the biggest (only?) contour should be our quadcopter
  cnt = contours[0];

  // look at some of the moments...
  M = cv::moments(cnt);
  cx = int(M['m10']/M['m00']);
  cy = int(M['m01']/M['m00']);

  // draw a crosshair on the centre
  annotated = np.copy(closed);
  cv::line(annotated, (cx-5, cy), (cx+5, cy), (0,0,255));
  cv::line(annotated, (cx, cy-5), (cx, cy+5), (0,0,255));
  show(annotated, "Centroid", WAIT);

  tracked_area = cv::contourArea(cnt);

  sum = np.sum(closed[:,:,0]);
  pixels = np.count_nonzero(closed[:,:,0]);
  mean_z = sum / pixels;
  mean_z = round(mean_z, 2);
  // CARE! 'contour' is bigger than 'pixels';
  // 583.5 to 498, in my test case...;

  cv::putText(annotated, "Centroid at "+str(cx)+", "+str(cy), *text_params);
  cv::putText(annotated, "Tracked Area "+str(pixels), (30,60), cv2.FONT_HERSHEY_TRIPLEX, 1, (0,0,255), 2);
  cv::putText(annotated, "Mean depth "+str(mean_z), (30,90), cv2.FONT_HERSHEY_TRIPLEX, 1, (0,0,255), 2);
  show(annotated, "", LONG_WAIT);
  }

  void Kinect::non_closed_sequence(){
  // no comments here, look at the closed_sequence for that...
  cv::namedWindow("preview");
  Mat original = cv::imread("quadcopter_images/snapshot1_dep.png");

  Mat raw = original;
  show(raw, "Raw", WAIT);

  // ret,threshold = cv2.threshold(original,100,255,cv2.THRESH_TOZERO_INV);
  double threshold_sequence = cv::threshold(original,100,255,cv2.THRESH_TOZERO_INV);
  show(threshold_sequence, "Threshold", WAIT);

  //_, binary = cv2.threshold(threshold,50 ,255,cv2.THRESH_BINARY);
  double binary = cv::threshold(threshold_sequence,50 ,255,cv2.THRESH_BINARY);
  show(binary, "Binary", WAIT);

  pre_contour = cv::cvtColor(binary,cv2.COLOR_BGR2GRAY);
  //contours, _ = cv2.findContours(pre_contour, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE);
  contours = cv::findContours(pre_contour, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE);
  cv::drawContours(threshold_sequence, contours, -1, (0, 0, 255));
  show(threshold, "Contours", WAIT);
  
  Mat M = cv::moments(contours[0]);
  int cx = int(M['m10']/M['m00']);
  int cy = int(M['m01']/M['m00']);
  

  Mat annotated;
  annotated = threshold.clone();
  cv::line(annotated, (cx-5, cy), (cx+5, cy), (0,0,255));
  cv::line(annotated, (cx, cy-5), (cx, cy+5), (0,0,255));
  show(annotated, "Centroid", WAIT);

  // we have to fix this part
  sum = np.sum(threshold_sequence[:,:,0]);
  pixels = np.count_nonzero(threshold[:,:,0]);
  mean_z = round(sum / pixels, 2);

  cv::putText(annotated, "Centroid at "+str(cx)+", "+str(cy), *text_params);
  cv::putText(annotated, "Tracked Area "+str(pixels), (30,60), cv2.FONT_HERSHEY_TRIPLEX, 1, (0,0,255), 2);
  cv::putText(annotated, "Mean depth "+str(mean_z), (30,90), cv2.FONT_HERSHEY_TRIPLEX, 1, (0,0,255), 2);
  show(annotated, "", LONG_WAIT);
  
  }
  //closed_sequence()
  // non_closed_sequence()
  */
