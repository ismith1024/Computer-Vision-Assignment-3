#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include<vector>
#include<iostream>
#include<fstream>

const int NUM_POINTS = 10;
const float RANGE = 100.00;
const int MAX_CAMERAS = 100; 
const int MAX_POINTS = 3000;

void decomposeProjectionMatrix(cv::Mat&, cv::Mat&, cv::Mat&, cv::Mat&);
void computeProjectionMatrix(cv::Mat&, cv::Mat&, cv::Mat&);