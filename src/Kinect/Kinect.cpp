#include "../../include/Kinect/Kinect.hpp"

//Constructor
Kinect::Kinect():
    depthMat(new cv::Mat(cv::Size(640,480),CV_16UC1)),
    depthf  (new cv::Mat(cv::Size(640,480),CV_8UC1)),
    rgbMat  (new cv::Mat(cv::Size(640,480),CV_8UC3,cv::Scalar(0)))
    {}

//destructor
Kinect::~Kinect() {
    delete depthMat;
    delete depthf;
    delete rgbMat;
    cv::destroyAllWindows();
}

// query the kinect for the location of an object
// expect this function to be called inside a loop continuously
bool Kinect::query(double& realX, double& realY, double& avgDepth, cv::Mat* inMat) {

    //inMat can be used to provide a depth matrix, instead of obtaining it from the Kinect.
    if(inMat && (inMat->rows == WIDTH) && (inMat->cols == HEIGHT)){
	//if a valid depth matrix has been supplied
	depthMat = inMat;
    } else {
	//read in depth matrix
	if(!camera.getDepth(*depthMat) ) return false;
    }
    
    depthMat->convertTo(*depthf, CV_8UC1, 1.0/8.03);

    int mmDepth;
    int sumX = 0;
    int sumY = 0;
    double sumDepth = 0;
    int count = 0;

    //loop over every row and column, and check if a depth value was read there.
    for(int y = 0 ; y < 480 ; y++) {
	for(int x = 0 ; x < 640 ; x++) {
      
	    //convert the raw depth reading into a mm value
	    mmDepth = rawDepthToMillimeters(depthMat->at<unsigned short>(y,x));
      
	    //if an object was recorded here, mark it as such.
	    if(mmDepth < THRESHOLD && mmDepth != 0){	
		sumX += x;
		sumY += y;
		sumDepth += mmDepth;
		count++;
	    }
	}
    }

    //if something was spotted, calculate the average x, y and z (in mm)
    if(count) {
	avgDepth = sumDepth/count;
	float avgX = (float) sumX / count;
	float avgY = (float) sumY / count;

	realX = getRealWidth(avgX, avgDepth);
	realY = getRealHeight(avgY, avgDepth);
	return true;
    }
    //if nothing was recorded, set all readings to 0.
    else {
	avgDepth = 0;
	realX = 0;
	realY = 0;
	return false;
    }
}

//query function that returns a Location object
Location Kinect::query(){

    Location reading;
    query(reading.X,reading.Y,reading.Z);
    return reading;

}

//save a video of both rgb and depth
void Kinect::saveVideo(std::string filename, int frames){

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

//display rgb and depth images, and save both if spacebar pressed
void Kinect::saveFrame(std::string filename){

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

void Kinect::update(){
    // loop both video and depth until they have updated
    while( !camera.getDepth(*depthMat)) ;
    while( !camera.getVideo(*rgbMat)  ) ;
}

//convert 11-bit depth reading into a distance in mm
int Kinect::rawDepthToMillimeters(int depthValue) {
    if (depthValue < 2047) {
	return (1000 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
    }

    //a value of 2047 means no reading
    return 0;
}

//convert x pixels on screen to mm distance from kinect origin
float Kinect::getRealWidth(float avgX, float depth) {
    float focal_distance =  WIDTH/(2*tan((57.0/2.0)*(PI/180.0)));
    return -1 * depth * (avgX - WIDTH / 2) / focal_distance;
}

//convert y pixels on screen to mm distance from kinect origin
float Kinect::getRealHeight(float avgY, float depth) {
    float focal_distance =  HEIGHT/(2*tan((43.0/2.0)*(PI/180.0)));
    return -1 * depth * (avgY - HEIGHT / 2) / focal_distance;
}
