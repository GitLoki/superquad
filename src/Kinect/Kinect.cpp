#include "../../include/Kinect/Kinect.hpp"

/* constructor */
Kinect::Kinect() {
  cv::Mat *depthMat = new cv::Mat(cv::Size(640,480), CV_16UC1);
  cv::Mat *depthf = new cv::Mat(cv::Size(640,480), CV_8UC1);
  cv::Mat *rgbMat = new cv::Mat(cv::Size(640,480),CV_8UC3,cv::Scalar(0));
  cv::Mat *ownMat = new cv::Mat(cv::Size(640,480),CV_8UC3,cv::Scalar(0));

  //The next two lines must be changed as Freenect::Freenect isn't a 
  //template but the method createDevice:
  //Freenect::Freenect<MyFreenectDevice> freenect;
  //MyFreenectDevice& device = freenect.createDevice(0);
  //by these two lines:

  cv::namedWindow("rgb",CV_WINDOW_AUTOSIZE);
  cv::namedWindow("depth",CV_WINDOW_AUTOSIZE);
  device.startVideo();
  device.startDepth();

  /*From colourtracking*/
  //some boolean variables for different functionality within this
  //program
  bool trackObjects = true;
  bool useMorphOps = true;
  //Matrix to store each frame of the webcam feed
  cv::Mat cameraFeed;
  //matrix storage for HSV image
  cv::Mat HSV;
  //matrix storage for binary threshold image
  cv::Mat threshold;

  //create slider bars for HSV filtering
  createTrackbars();
  //video capture object to acquire webcam feed
  cv::VideoCapture capture;
  //open capture object at location zero (default location for webcam)
  capture.open(0);
  //set height and width of capture frame
  capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
  capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
  //start an infinite loop where webcam feed is copied to cameraFeed matrix
  //all of our operations will be performed within this loop
  /*End_of_colourtracking*/
}

/* destructor */
Kinect::~Kinect() {
  device.stopVideo();
  device.stopDepth();
}

/* expect this function to be called inside a loop continuously */
int Kinect::query(double& x, double& y, double& z, double& p) {
  std::string filename("snapshot");
  std::string suffix(".png");
  int i_snap = 0;
  //  int iter = 0;

  //x and y values for the location of the object
  int colour_x=0.0, colour_y=0.0;

  device.getVideo(*rgbMat);
  device.getDepth(*depthMat);
  imshow("rgb", *rgbMat);
  //depthMat.convertTo(depthf, CV_16UC1, 255.0/2048.0);
  // http://stackoverflow.com/questions/6909464/convert-16-bit-depth-cvmat-to-8-bit-depth
  depthMat->convertTo(*depthf, CV_8UC1, 1.0/8.03);
  imshow("depth",*depthf);

  //imshow("HSV",HSV);
  
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
  
  for(int i = 0 ; i < 100 ; i++) {
    std::cout << std::endl;
  }
  int mmDepth;
  int sumX = 0;
  int sumY = 0;
  double sumDepth = 0;
  int count = 0;
  
  std::cout << "starting x,y,z loop" << std::endl;
  
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
    
    float avgX = ((float)sumX)/count;
    float avgY = ((float)sumY)/count;
    double avgDepth = sumDepth/count;
    float realX = getrealwidth(avgX, avgDepth);
    float realY = getrealheight(avgY, avgDepth);
    
    std::cout << "Object located at pixels: ("
	      << avgX << "," << avgY << "," << avgDepth << ")"
	      << std::endl;
    std::cout << "Real world location: ("
	      << realX << "," << realY << "," << avgDepth << ")"
	      << std::endl;
    std::cout << "number of points read: " << count << std::endl;
    
    // graphical tracker from colour tracking code
    colour_x = avgX;
    colour_y = avgY;
    std::cout << "before draw object" << std::endl;
    drawObject(colour_x,colour_y,*rgbMat);
    //trackFilteredObject(colour_x,colour_y, threshold, rgbMat);
    std::cout << "after draw object" << std::endl;
  }
  else {
    std::cout << "No values read" << std::endl;
  }
}

int Kinect::rawDepthToMilimeters(int depthValue) {
  if (depthValue < 2047) {
    return (1000 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
  }
  return 0;
}

void Kinect::on_trackbar( int, void*) {
  //This function gets called whenever a
  // trackbar position is changed
}

std::string Kinect::intToString(int number) {
  std::stringstream ss;
  ss << number;
  return ss.str();
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

float Kinect::getrealwidth(float avgX, float depth) {
  float focal_distance =  FRAME_WIDTH/(2*tan((57.0/2.0)*(PI/180.0)));
  return depth * avgX / focal_distance;
}

float Kinect::getrealheight(float avgY, float depth) {
  float focal_distance =  FRAME_HEIGHT/(2*tan((43.0/2.0)*(PI/180.0)));
  return depth * avgY / focal_distance;
}
