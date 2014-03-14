#include <iostream>
#include <fstream>
#include <cmath>
#include "../../include/Kinect/Kinect.hpp"

using namespace std;

const int HEIGHT_READINGS = 5;
const int WIDTH_READINGS = 5;
const int DEPTH_READINGS = 5;
const int READINGS_PER_LOCATION = 250;

double calculateVariance(double readings[], double mean, int number);
double calculateCorrectedVariance(double readings[], double mean, int number);

  struct readingstruct{
    double x[READINGS_PER_LOCATION];
    double y[READINGS_PER_LOCATION];
    double z[READINGS_PER_LOCATION];
  };


void getgrid() {
  double x,y,z,p;
  double sumx=0, sumy=0, sumz=0 ;
  int numReads=0; int goodReads; int nullReads=0;
  double meanx, meany, meanz;
  double minz = 2000; double maxz = 0;
  //double readings [HEIGHT_READINGS][WIDTH_READINGS][DEPTH_READINGS][READINGS_PER_LOCATION];
  double means[HEIGHT_READINGS][WIDTH_READINGS][DEPTH_READINGS];
  double stddevs[HEIGHT_READINGS][WIDTH_READINGS][DEPTH_READINGS];
  double varx, vary, varz;
  double stddevx, stddevy,stddevz;

  readingstruct readings;

  Kinect k;

  char input;
  std::ofstream readingsfile("readings.txt");

  std::cout << "Hit any key to start..." << std::endl;
  std::cin.get(input);

  for(int w = 0; w <= WIDTH_READINGS; w+=1){
    for(int d = 0; d <= DEPTH_READINGS; d+=1){
      for(int h = 0; h <= HEIGHT_READINGS; h+=1){
	
	numReads = 0; nullReads = 0;
	sumx = 0; sumy = 0; sumz = 0;

	while(numReads++ < READINGS_PER_LOCATION) {
	  k.query(x,y,z,p);
	  
	  readings.x[numReads-1] = x;
	  readings.y[numReads-1] = y;
	  readings.z[numReads-1] = z;
	  
	  if(x+y+z == 0){
	    nullReads++;
	  }

	  sumx+=x; sumy+=y; sumz+=z;

	  usleep(29000); // Wait ~thirtieth second
	}
	std::cout << "readings taken for: (" << 100 - (20 * w) << ", " << 100 - (20 * h) << ", " << 50 + (20 * d) << ")" << std::endl;

	meanx = sumx/(numReads-nullReads);
	meany = sumy/(numReads-nullReads);
	meanz = sumz/(numReads-nullReads);
	
	varx = calculateVariance(readings.x, meanx, numReads);
	vary = calculateVariance(readings.y, meany, numReads);
	varz = calculateVariance(readings.z, meanz, numReads);
	stddevx = sqrt(varx); stddevy = sqrt(vary); stddevz = sqrt(varz);

	readingsfile << "Grid Location: (" << 100 - (20 * w) << ", " << 100 - (20 * h) << ", " << 50 + (20 * d) << ")" << std::endl;
	
	if (meanx == 0)
	  readingsfile << "Not in frame" << std::endl << std::endl;
	else {
	  readingsfile << "Mean: (" << meanx << ", " << meany << ", " << meanz << ")" << std::endl;
	  readingsfile << "Std. Dev: (" << stddevx << ", " << stddevy << ", " << stddevz << ")" << std::endl;
	  readingsfile << "Null Readings: " << nullReads << std::endl << std::endl;
	}

	std::cout << "Hit any key to continue..." << std::endl;
	std::cin.get(input);

	/*
	readingsfile << "Mean depth reading: " << mean
		     << "\nVariance: " << var
		     << "\nStandard Deviation: " << stddev
		     << "\nTotal number of readings: " << numReads
		     << "\nTotal number of null readings: " << nullReads
		     << "\nMaximum reading: " << maxz
		     << "\nMinimum reading: " << minz
		     << "\n";
	*/
      }
    }
  }
	
  readingsfile.close();
  
  return ;
}

double calculateVariance(double readings[], double mean, int number){

  double variance = 0;
  double good = 0;

  for(int i=0; i<number; i++){
    if(readings[i]){
      variance += (readings[i]-mean)*(readings[i]-mean);
      good++;
    }
  }
  if(good > 1){
    variance /= (good-1);
  } else {
    variance = 0;
  }

  return variance;

}




double calculateCorrectedVariance(double readings[], double mean, int number){

  double variance = 0;
  int counted = 0;

  for(int i=0;i<number;i++){
    
    if(abs(readings[i]-mean) <= 100){
      counted++;
      variance += (readings[i]-mean)*(readings[i]-mean);
    }
  }

  if(counted > 1){
    variance /= (counted-1);
  } else {
    variance = 0;
  }

  return variance;

}

int main () {
  bool end = false;
  
  getgrid();
  std::cout << "Readings complete." << std::endl; 
}
