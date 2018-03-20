///////////
////
//// COMP4102 Assignment 3 Q2
//// Created 2018-03-19
//// Ian Smith 100910972
////
//// Usage notes: requires c++14
//// To complile:  g++ -std=c++14 a3q2.cpp -o a3q2 `pkg-config --libs opencv`
////
//// openCV version used:
//// OpenCV version : 3.3.1
//// Major version : 3
//// Minor version : 3
//// Subminor version : 1
////

/*
 The goal of this question is to create a program that take as input two images that are related
by a rotation homography; a left (keble_a_half), middle (keble_b_long) and creates a single
panoramic image (same size as keble_b_long) as output. This is done by warping the left
“into” the middle image. I have made the middle image big enough to hold both the warped
left and the original middle image. I have given you a program called
akaze-match-template.cpp which takes these two images and computes a set of features that
you can use to compute the homography between them. To actually compute the homography
you use the routine findhomography(, , RANSAC) and then you use warperspective routine
with the computed homography to warp the left image into an image of the same size as the
middle image. In other words you warp img1 into img3, and after that you paste (essentially
an OR operation) img3 into img2. You should output two images; warped, which is the
warped version of img1, and merged which is the warped version of img1 (img3) combined
with img2. I have included two images called warped and merged which show you how they
should look like. Notice that the final merged image has some anomalies because of the OR
operation. In real mosaicking programs you do not see these anomalies. Write down a short
(one paragraph) description of how you would get rid of these visible anomalies and include
it in your upload. The answer is simple.
--Response is in ../readme.txt

*/

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
    
    //extract the Point members of the KeyPoint objects
    for(auto& e1: matched1) inliers1.push_back(e1.pt); 
    for(auto& e1: matched2) inliers2.push_back(e1.pt); 
    
    //find the homography between the matched points
    homography = findHomography(inliers1, inliers2, RANSAC);
    
    //apply the homography to the keble-a-half image we want to warp
    warpPerspective(img1, img3, homography, img3.size());
    
    //imshow("Warped",img3);
    imwrite("Warped.jpg", img3);
    //waitKey(0);
        
    //Mat med2 = Mat(img2.rows, img2.cols, CV_8UC1);
    Mat med3 = Mat(img3.rows, img3.cols, CV_8UC1);
    img3.copyTo(med3);

    //corrected image using mean of the two images instead of OR
    for(int i = 0; i < img2.cols; ++i) for(int j = 0; j < img2.rows; ++j){
        if(img3.at<uchar>(j,i) == 0 ) med3.at<uchar>(j,i) = img2.at<uchar>(j,i);
        else if(img2.at<uchar>(j,i) == 0 )  med3.at<uchar>(j,i) = img3.at<uchar>(j,i);
        else med3.at<uchar>(j,i) = (img3.at<uchar>(j,i) + img2.at<uchar>(j,i)) /2; 
    }  

        //Bitwise OR of the two images to merge them
    //pixels are accessed as unsigned char
    for(int i = 0; i < img2.cols; ++i) for(int j = 0; j < img2.rows; ++j){
        img3.at<uchar>(j,i) = img3.at<uchar>(j,i) | img2.at<uchar>(j,i);  
    }
    
    //Display input and output
    imshow("Input1",img1);
    imshow("Input2",img2);
    imshow("Merged using OR",img3);
    imshow("Merged using mean pixel value",med3);
    
    imwrite("Merged.jpg", img3);
    imwrite("Corrected.jpg", med3);    
    
    waitKey(0);

    return 0;
}