///////////
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

#include "a3q2.h"

int main(void) {
    std::cout << "Opening" << std::endl;
    cv::Mat img1 = cv::imread("keble_a_half.bmp", IMREAD_GRAYSCALE);
    cv::Mat img2 = cv::imread("keble_b_long.bmp", IMREAD_GRAYSCALE);
    cv::Mat img3 = cv::Mat(img2.rows, img2.cols, CV_8UC1);
    //img2.copyTo(img3);

    cv::Mat homography;

    std::vector<KeyPoint> kpts1, kpts2;
    cv::Mat desc1, desc2;
    
    std::cout << "Have opened" << std::endl;

    cv::Ptr<cv::AKAZE> akaze = cv::AKAZE::create();
    akaze->detectAndCompute(img1, noArray(), kpts1, desc1);
    akaze->detectAndCompute(img2, noArray(), kpts2, desc2);

    std::cout << "Have computed akaze" << std::endl;

    cv::BFMatcher matcher(NORM_HAMMING);
    std::vector< std::vector<cv::DMatch> > nn_matches;
    matcher.knnMatch(desc1, desc2, nn_matches, 2);

    std::cout << "Have done match" << std::endl;

    std::vector<cv::KeyPoint> matched1, matched2;
    std::vector<cv::Point2f> inliers1, inliers2;
 
    for(size_t i = 0; i < nn_matches.size(); i++) {
        cv::DMatch first = nn_matches[i][0];
        float dist1 = nn_matches[i][0].distance;
        float dist2 = nn_matches[i][1].distance;

        if(dist1 < nn_match_ratio * dist2) {
            matched1.push_back(kpts1[first.queryIdx]);
            matched2.push_back(kpts2[first.trainIdx]);
        }
    }
	
    std::cout << "Matches :" <<  matched1.size() << " " << matched2.size() << std::endl;

    //Display input and output
    cv::imshow("Input1",img1);
    cv::imshow("Input2",img2);
    cv::waitKey(0);

    return 0;
}