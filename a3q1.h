#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include<vector>
#include<iostream>
#include<fstream>

const int NUM_POINTS = 10;
const int A_COLS = 12;
const float RANGE = 100.00;
const int MAX_CAMERAS = 100; 
const int MAX_POINTS = 3000;

void decomposeProjectionMatrix(cv::Mat&, cv::Mat&, cv::Mat&, cv::Mat&);
void computeProjectionMatrix(cv::Mat&, cv::Mat&, cv::Mat&, std::ofstream&);

/*
 The goal of the first questions is to implement some code that performs calibration using the
method described in the book; by first computing a projection matrix, and then decomposing
that matrix to find the extrinsic and intrinsic camera parameters. Use the approach described
in the slides. I have given you a program, written in C++ that uses OpenCV, called
projection-template.cpp. This program takes ten given 3d points and projects them into a 2d
image using the given supplied camera calibration matrix, rotation matrix and translation
vector. Your goal is to write the two routines that are missing, which are
computeprojectionmatrix and decomposeprojectionmatrix. The first routine
computes the projection matrix using the method described in Section 6.3.1 of the book, and
the second uses the method in Section 6.3.2 to decompose the projection matrix into a camera
calibration matrix, rotation matrix and translation vector. It should be the case that the
computed camera matrix, rotation matrix and translation vector are the same (or very similar)
to the original versions that were used to create the projected points. This shows that your
two routines are working properly. I also have a similar routine in Python called
projection_template_python.py. You hand in your program source and the resulting output
file assign3-out created by running this modified program. Or hand in a screen capture of the
output which shows that the computed quantities are the same, or close to the same as the
original ones used to create the projection matrix. 

Some extra hints for Question 1 of Assign3. You first create the A matrix which is a 2n by 12 matrix, where n is the number of given 3d to 2d correspondences. Now you need to find the eigenvectors and eigenvalues of A transpose times A (a 12 by 12 matrix). There are two ways to do this, using cv2.eigen or numpy.linalg.svd (there are actually more ways but these are the ones I will take about). They are shown below, in Python. First I define a 4 by 3 matrix  A (you will have a 2n by 12 matrix), compute A transpose A, and then find the eigenvectors and eigenvalues of that matrix. Note that the resulting eigenvalues from cv2.eigen are returned in order from largest to smallest, and the eigenvectors are returned in the same order. Look at the documentaton in https://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html#eigen

I use cv2.eigen in this python code and then show how to use r, u, s, v = numpy.linalg.svd(A) instead of cv2.eigen. When using the SVD routine instead of eigen the eigenvectors are returned in v in the order of largest to smallest eigenvalue (you need the last entry, the eigenvector associated with the smallest eigenvalue). In this case the original A is input to svd, this is the matrix of size 2*n by 12, where n is the number of 3d points, and not the matrix A transpose times A that was input to eigen (using svd does save you having to perform this matrix multiplication). Both approaches work, and you need the eigenvector associated with the smallest eigenvalue, but you do not actually need the eigenvalue.

Once you have the eigenvector associated with the smallest eigenvalue (a 12 element vector) you create the projection matrix from this eigenvector (one row at a time). You can now apply this computed projection matrix to the original 3d points and verify that the result is the same original set of 2d points. If that is true then you have computed a good projection matrix. Now you use the info in project_deriv_(a,b,c).pdf to compute the K, R and T from the numbers in the computed projection matrix and verify that they are the same as the original K, R and T. 

So to conclude there are two distinct steps to Question 1. First use the given 3d to 2d correspondences to compute the projection matrix. Then decompose this projection matrix to find K, R and T. This should be enough to do Question 1 of assignment 3.  
 */