#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include<vector>
#include<iostream>
#include<fstream>

//#define NUM_POINTS    10
#define RANGE         100.00
#define MAX_CAMERAS   100 
#define MAX_POINTS    3000

void decomposeProjectionMatrix(cv::Mat&, cv::Mat&, cv::Mat&, cv::Mat&);
void computeProjectionMatrix(cv::Mat&, cv::Mat&, cv::Mat&);