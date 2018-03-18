#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

const float inlier_threshold = 2.5f; // Distance threshold to identify inliers
const float nn_match_ratio = 0.8f;   // Nearest neighbor matching ratio

int main(void)
{
    puts("opening");
    Mat img1 = imread("keble_a_half.bmp", IMREAD_GRAYSCALE);
    Mat img2 = imread("keble_b_long.bmp", IMREAD_GRAYSCALE);
    Mat img3 = Mat(img2.rows, img2.cols, CV_8UC1);
//img2.copyTo(img3);

    Mat homography;

    vector<KeyPoint> kpts1, kpts2;
    Mat desc1, desc2;
    
    puts("Have opened");

    Ptr<AKAZE> akaze = AKAZE::create();
    akaze->detectAndCompute(img1, noArray(), kpts1, desc1);
    akaze->detectAndCompute(img2, noArray(), kpts2, desc2);

    puts("have commputed akaze");

    BFMatcher matcher(NORM_HAMMING);
    vector< vector<DMatch> > nn_matches;
    matcher.knnMatch(desc1, desc2, nn_matches, 2);
    
    puts("Have done match");

    vector<KeyPoint> matched1, matched2;
    vector<Point2f> inliers1, inliers2;
 
    for(size_t i = 0; i < nn_matches.size(); i++) {
        DMatch first = nn_matches[i][0];
        float dist1 = nn_matches[i][0].distance;
        float dist2 = nn_matches[i][1].distance;

        if(dist1 < nn_match_ratio * dist2) {
            matched1.push_back(kpts1[first.queryIdx]);
            matched2.push_back(kpts2[first.trainIdx]);
        }
    }
    
    printf("Matches %d %d\n", matched1.size(), matched2.size());
    
    for(auto& e1: matched1) inliers1.push_back(e1.pt); 
    for(auto& e1: matched2) inliers2.push_back(e1.pt); 
    
    //find the homography between the matched points
    homography = findHomography(inliers1, inliers2, RANSAC);
    
    //apply the homography to the keble-a-half image we want to warp
    warpPerspective(img1, img3, homography, img3.size());
    
    imshow("Warped",img3);
    waitKey(0);
        
    Mat blur2 = Mat(img2.rows, img2.cols, CV_8UC1);
    Mat blur3 = Mat(img3.rows, img3.cols, CV_8UC1);
    
    /*cv::blur(img2, blur2, cv::Size(3, 3)); 
    cv::blur(img3, blur3, cv::Size(3, 3));*/
    
    //corrected for grainy images
    /*for(int i = 0; i < blur2.cols; ++i) for(int j = 0; j < blur2.rows; ++j){
        blur3.at<uchar>(j,i) = blur3.at<uchar>(j,i) | blur2.at<uchar>(j,i);  
    } */   
    
    //Boolean OR of the two images to merge them
    for(int i = 0; i < img2.cols; ++i) for(int j = 0; j < img2.rows; ++j){
        img3.at<uchar>(j,i) = img3.at<uchar>(j,i) | img2.at<uchar>(j,i);  
    }
    
    //Display input and output
    imshow("Input1",img1);
    imshow("Input2",img2);
    imshow("Merged",img3);
    //imshow("Corrected",blur3);
    waitKey(0);

    return 0;
}