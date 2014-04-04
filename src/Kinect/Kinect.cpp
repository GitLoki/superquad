#include "../../include/Kinect/Kinect.hpp"

Kinect::Kinect():
  depthMat(new cv::Mat(cv::Size(640,480), CV_16UC1)),
  depthf(  new cv::Mat(cv::Size(640,480),CV_8UC1)),
  rgbMat(  new cv::Mat(cv::Size(640,480),CV_8UC3,cv::Scalar(0))),
  ownMat(  new cv::Mat(cv::Size(640,480),CV_8UC3,cv::Scalar(0))),
  trackObjects(true),
  useMorphOps( true)
{

}

Kinect::~Kinect(){
  delete depthMat;
  delete depthf;
  delete rgbMat;
  delete ownMat;
 }

bool Kinect::update(){
  bool videoSuccess = camera.getVideo(*rgbMat);
  bool depthSuccess = camera.getDepth(*depthMat);

  if (videoSuccess && depthSuccess){
    // std::cout<< "Success!" << std::endl;
    return true;
  }
  
  // since tracking currently ONLY depends on depth...
  if (depthSuccess){
    // std::cout<< "(Partial)Success!" << std::endl;
    return true;
  }

  //std::cout << "Failure of ";
  //if (!videoSuccess) std::cout << "video";
  //if (!depthSuccess) std::cout << "depth";
  //std::cout << std::endl;

  return false;
}

void Kinect::save_frame(std::string filename){

  cv::namedWindow("rgb",CV_WINDOW_AUTOSIZE);
  cv::namedWindow("depth",CV_WINDOW_AUTOSIZE);

  std::string suffix(".png");
  int i_snap = 0;

  while(true){
    update(); //get new RGB and depth

    imshow("rgb", *rgbMat);
    depthMat->convertTo(*depthf, CV_8UC1, 1.0/8.03);
    imshow("depth",*depthf);

    char k = cvWaitKey(5);
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
  if( !update() ){
    avgDepth = 0;
    realX = 0;
    realY = 0;
    return false;
  }

  // depthMat.convertTo(depthf, CV_16UC1, 255.0/2048.0);
  // http://stackoverflow.com/questions/6909464/convert-16-bit-depth-cvmat-to-8-bit-depth
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

  // ##########  COLOR TRACKING ##########
  /*
  //store image to matrix
  capture.read(cameraFeed);
  //convert frame from BGR to HSV colorspace
  cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
  //filter HSV image between values and store filtered image to
  //threshold matrix
  inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
  
  if(useMorphOps)
  morphOps(threshold);
  //pass in thresholded frame to our object tracking function
  //this function will return the x and y coordinates of the
  //filtered object
  if(trackObjects){
  trackFilteredObject(colour_x,colour_y,threshold,cameraFeed);
  cout << "X: " << colour_x << "    Y: " << colour_y << std::endl;
  }
  
  //show frames 
  imshow(windowName2,threshold);
  imshow(windowName,cameraFeed);
  imshow(windowName1,HSV);
  
  */
  // ##########  END COLOR TRACKING ##########

  if(count) {
    avgDepth = sumDepth/count;
    realX = (double) sumX / count;
    realY = (double) sumY / count;
    return true;
    // need to getrealwidth(avgX, avgDepth);
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

std::string Kinect::intToString(int number) {
  std::stringstream ss;
  ss << number;
  return ss.str();
}

float Kinect::getrealwidth(float avgX, float depth) {
  float focal_distance =  FRAME_WIDTH/(2*tan((57.0/2.0)*(PI/180.0)));
  return depth * avgX / focal_distance;
}

float Kinect::getrealheight(float avgY, float depth) {
  float focal_distance =  FRAME_HEIGHT/(2*tan((43.0/2.0)*(PI/180.0)));
  return depth * avgY / focal_distance;
}



void Kinect::drawObject(int x, int y, cv::Mat &frame) {
  //use some of the openCV drawing functions to draw crosshairs
  //on your tracked image!

  //UPDATE:JUNE 18TH, 2013
  //added 'if' and 'else' statements to prevent
  //memory errors from writing off the screen (ie. (-25,-25) is not within 
  //the window!)
  circle(frame,cv::Point(x,y),20,cv::Scalar(0,255,0),2);
  if(y-25>0)
    line(frame,cv::Point(x,y),cv::Point(x,y-25),cv::Scalar(0,255,0),2);
  else line(frame,cv::Point(x,y),cv::Point(x,0),cv::Scalar(0,255,0),2);
  if(y+25<FRAME_HEIGHT)
    line(frame,cv::Point(x,y),cv::Point(x,y+25),cv::Scalar(0,255,0),2);
  else line(frame,cv::Point(x,y),cv::Point(x,FRAME_HEIGHT),
	    cv::Scalar(0,255,0),2);
  if(x-25>0)
    line(frame,cv::Point(x,y),cv::Point(x-25,y),cv::Scalar(0,255,0),2);
  else line(frame,cv::Point(x,y),cv::Point(0,y),cv::Scalar(0,255,0),2);
  if(x+25<FRAME_WIDTH)
    line(frame,cv::Point(x,y),cv::Point(x+25,y),cv::Scalar(0,255,0),2);
  else line(frame,cv::Point(x,y),cv::Point(FRAME_WIDTH,y),
	    cv::Scalar(0,255,0),2);

  putText(frame,intToString(x)+","+intToString(y),cv::Point(x,y+30),1,1,
	  cv::Scalar(0,255,0),2);
}

//Colourtracking functions below

void Kinect::trackFilteredObject(int &x, int &y, cv::Mat threshold, cv::Mat &cameraFeed) {
  cv::Mat temp;
  threshold.copyTo(temp);
  //these two vectors needed for output of findContours
  std::vector< std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;
  //find contours of filtered image using openCV findContours function
  findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
  //use moments method to find our filtered object
  double refArea = 0;
  bool objectFound = false;
  if (hierarchy.size() > 0) {
    int numObjects = hierarchy.size();
    //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
    if(numObjects<MAX_NUM_OBJECTS){
      for (int index = 0; index >= 0; index = hierarchy[index][0]) {

	cv::Moments moment = cv::moments((cv::Mat)contours[index]);
	double area = moment.m00;

	//if the area is less than 20 px by 20px then it is probably just noise
	//if the area is the same as the 3/2 of the image size, probably just 
	//a bad filter
	//we only want the object with the largest area so we safe a reference 
	//area each
	//iteration and compare it to the area in the next iteration.
	if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
	  x = moment.m10/area;
	  y = moment.m01/area;
	  objectFound = true;
	  refArea = area;
	}else objectFound = false;

      }
      //let user know you found an object
      if(objectFound ==true){
	putText(cameraFeed,"Tracking Object",cv::Point(0,50),2,1,cv::Scalar(0,255,0),2);
	//draw object location on screen
	drawObject(x,y,cameraFeed);
      }

    } else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",cv::Point(0,50),1,2,cv::
Scalar(0,0,255),2);
  }
}


void Kinect::morphOps(cv::Mat &thresh) {

  //create structuring element that will be used to "dilate" and "erode" image.
  //the element chosen here is a 3px by 3px rectangle

  // original value (3,3)
  cv::Mat erodeElement = getStructuringElement(cv::MORPH_RECT,cv::Size(1,1));
  //dilate with larger element so make sure object is nicely visible
  // original value (8,8)
  cv::Mat dilateElement = getStructuringElement(cv::MORPH_RECT,cv::Size(8,8));

  erode(thresh,thresh,erodeElement);
  erode(thresh,thresh,erodeElement);

  dilate(thresh,thresh,dilateElement);
  dilate(thresh,thresh,dilateElement);
}


void Kinect::createTrackbars() {
  // colour tracking constants
  // Tereza: this should match orange colour, but needs to be tested
  // again with the orange quadcopter
  int H_MIN = 177;
  int H_MAX = 185;
  int S_MIN = 115;
  int S_MAX = 256;
  int V_MIN = 0;
  int V_MAX = 256;
 

  //create window for trackbars
  cv::namedWindow(trackbarWindowName,0);

  //create memory to store trackbar name on window
  char TrackbarName[50];
  /* MILO: CHANGED THESE LINES BELOW - REMOVED LAST ARGUMENT BECAUSE 
     COMPILER COMPLAINED - MILO */

  sprintf( TrackbarName, "H_MIN");
  sprintf( TrackbarName, "H_MAX");
  sprintf( TrackbarName, "S_MIN");
  sprintf( TrackbarName, "S_MAX");
  sprintf( TrackbarName, "V_MIN");
  sprintf( TrackbarName, "V_MAX");

 //create trackbars and insert them into window
  //3 parameters are: the address of the variable that is changing when 
  //the trackbar is moved(eg.H_LOW),
  //the max value the trackbar can move (eg. H_HIGH), 
  //and the function that is called whenever the trackbar is moved(eg. 
  //on_trackbar)     
  /*
    ORIGINAL createTrackbar - changed by TEREZA
    it might be better hardcode the values for orange and don't run
    the trackbar window

  cv::createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
  cv::createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
  cv::createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
  cv::createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
  cv::createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
  cv::createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);
  */

  cv::createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX);
  cv::createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX);
  cv::createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX);
  cv::createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX);
  cv::createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX);
  cv::createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX);

}


void Kinect::on_trackbar( int, void*) {
  //This function gets called whenever a
  // trackbar position is changed
}

void Kinect::save_video(std::string filename, int frames){
  
  bool truth = true;

  cv::Mat doubleImg= cv::Mat::zeros(960,640,CV_8UC3);
  cv::Mat *depth3 = new cv::Mat(cv::Size(640,480),CV_8UC3);
  cv::Mat in[] = {*depthf,*depthf,*depthf};
  cv::merge(in,3,*depth3);

  cv::Size frameSize(640,960);

  int count = 0;

  writer.open(filename, CV_FOURCC('P','I','M','1'), 20, frameSize, true);

  double x,y,z;

  while(truth && count < frames){

    cv::Mat in[] = {*depthf,*depthf,*depthf};
    cv::merge(in,3,*depth3);

    query(x,y,z);

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

  delete depth3;

}
/*

// Open the input video files or camera stream.
CvCapture* capture1 = cvCaptureFromAVI( argv[1] );
CvCapture* capture2 = cvCaptureFromAVI( argv[2] );
// Create a new video file for output.
CvSize combinedSize = [Big enough to fit both video frames next to each other, plus a border].
CvVideoWriter* videoWriter = cvCreateVideoWriter(outputFilename, CV_FOURCC('D','I','V','3'), FPS, combinedSize, TRUE);
IplImage *frameOut = cvCreateImage(combinedSize, 8, 3);		// Create an empty RGB image for storing the combined frame.
IplImage *frame1, *frame2;

// Process both video streams while atleast one is still running (AVI or MPG file or camera stream).
frame1 = (IplImage*)1;	// Enter the loop.
frame2 = (IplImage*)1;	// Enter the loop.
while (frame1 || frame2) {
	// Get the next video frames.
	frame1 = cvQueryFrame( capture1 );
	frame2 = cvQueryFrame( capture2 );
	if (frame1 || frame2) {
		// Combine the 2 image frames into 1 big frame.	
		frameOut = combineImages(frame1, frame2);
		// Store the combined video frame into the new video file.
		cvWriteFrame(videoWriter, frameOut);
	}
}
*/
