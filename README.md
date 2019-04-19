# Computer Vision COMP4102
### Assignment 3 - Decompose Projection Matrix; Image Stitching
This project contains two parts:
 - Using the opencv C++ library, decompose a projection matrix to find camera parameters
 - Stich two images, related by homography, using the RANSAC feature detection algorithm  

### Table of Contents

1. [Libraries and Installation](#installation)
2. [Project Description](#motivation)
3. [Files](#files)
4. [Findings](#results)

## Libraries and Installation <a name="installation"></a>

Libraries used:
The project uses the openCV library for linear algebra operations.  The library is licensed using the 3-clause BSD license, linked here:
https://opencv.org/license/

OpenCV version : 3.3.1
Major version : 3
Minor version : 3
Subminor version : 1

Both parts of the project are written un C++ using the C++14 standard.

## Project Description<a name="motivation"></a>

### Part 1:
The purpose of the exercise is to show understanding of the matrix operations used in projection and camera calibration:
- Given a set of 3D points and their 2D image in a camera, a camera calibration matrix, and rotation and transaltion vectors
- Compute a projection matrix
- Then decompose this projection matrix into decompose the projection matrix into a camera calibration matrix, rotation matrix and translation vector
- Solution is implemented using singular value decomposition

### Part 2:
The purpose of this exercise is to stictch two related images using a homography relationship to create a panorama.

Here are the base images:
![Base Image](https://github.com/ismith1024/Computer-Vision-Assignment-3/blob/master/Q2%20-%20Image%20Stitching/keble_b_long.bmp "Image 1")

![Base Image](https://github.com/ismith1024/Computer-Vision-Assignment-3/blob/master/Q2%20-%20Image%20Stitching/keble_a_half.bmp "Image 2")

The second image, after matching features using RANSAC, and then applying the homography transformation:
![Base Image](https://github.com/ismith1024/Computer-Vision-Assignment-3/blob/master/Q2%20-%20Image%20Stitching/Warped.jpg "Image 2, warped")

Merging the two images using bitwise OR.  This operation was not successful - consider two adjacent grayscale values of 0100 0000 and 0011 1111.  This gives 0100 0000 | 0011 1111 = 0111 1111, which is much brighter than either.
![Base Image](https://github.com/ismith1024/Computer-Vision-Assignment-3/blob/master/Q2%20-%20Image%20Stitching/Merged.jpg "Stitched (using bitwise OR")

The improved merge operation: if one pixel is black (0000 0000), then use the pixel value from the other image, else, use the mean value value of the corresponding pixels in both images.
![Base Image](https://github.com/ismith1024/Computer-Vision-Assignment-3/blob/master/Q2%20-%20Image%20Stitching/Corrected.jpg "Stitched (using bitwise OR")


## Files <a name="files"></a>

### Part 1:
 - Code is in: a3q1.cpp, a3q1.h 
 - C++ standard used was c++14
 - To compile:
    ``g++ -std=c++14 a3q1.cpp -o a3q1 `pkg-config --libs opencv` ``

### Part 2:
 - Code is in a3q2.cpp 
 - C++ standard is c++14
 - To compile:
    ``g++ -std=c++14 a3q2.cpp -o a3q2 `pkg-config --libs opencv` ``

## Findings<a name="results"></a>

### Part 1:

Inspecting the log, the reconstructed image and projection points are within reasonable rounding errors of the originals.

### Part 2:

After inspecting the pixels in both the warped and center image around the region of the anomalies, it is clear that there
are no near-white pixels in either image.  So, I am assuming they result from the mathematical merging of the pixel values.

I have included a second merged image that calculates pixel values as follows:

Mean value of the pixel in both the warped and center image, unless exactly one of the images has a black pixel there -- in this case it is the 
value from the other image.
    
The merged image obtained in this way is submitted as "corrected.jpg".  The merged image from OR with the anomalies is "merged.jpg"








