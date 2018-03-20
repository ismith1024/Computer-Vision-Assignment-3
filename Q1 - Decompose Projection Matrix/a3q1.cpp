///////////
////
//// COMP4102 Assignment 3 Q1
//// Created 2018-03-17
//// Ian Smith 100910972
////
//// Usage notes: requires c++14
//// To complile:  g++ -std=c++14 a3q1.cpp -o a3q1 `pkg-config --libs opencv`
////
//// openCV version used:
//// OpenCV version : 3.3.1
//// Major version : 3
//// Minor version : 3
//// Subminor version : 1
////

/* OBJECTIVE:
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
 */


#include "a3q1.h"

int main(){

    ///object point matrix: [X_n, Y_n, Z_n, 1]
    cv::Mat objectPoints = (cv::Mat_<double>(10,4) << 0.1251, 56.3585, 19.3304, 1.0,
                                                        80.8741, 58.5009, 47.9873, 1.0,
                                                        35.0291, 89.5962, 82.2840, 1.0,
                                                        74.6605, 17.4108, 85.8943, 1.0,
                                                        71.0501, 51.3535, 30.3995, 1.0,
                                                        1.4985, 9.1403, 36.4452, 1.0,
                                                        14.7313, 16.5899, 98.8525, 1.0,
                                                        44.5692, 11.9083, 0.4669, 1.0,
                                                        0.8911, 37.7880, 53.1663, 1.0,
                                                        57.1184, 60.1764, 60.7166, 1.0);

    ///Given this projection matrix
    cv::Mat projection = (cv::Mat_<double>(3,4) <<  0.902701, 0.051530, 0.427171, 12.0, 
                                                    0.182987, 0.852568, -0.489535, 16.0, 
                                                    -0.389418, 0.520070, 0.760184, 21.0);

    ///Given this intrinsic matrix
    cv::Mat intrinsic = (cv::Mat_<double>(3,3) << -1100.000000, 0.000000, 0.000000, 
                                                    0.000000, -2200.000000, 0.000000, 
                                                    0.000000, 0.000000, 1.000000);


    cv::Mat computedCameraMatrix = (cv::Mat_<double>(3,3) << 0,0,0,0,0,0,0,0,0);
    cv::Mat computedRotationMatrix = (cv::Mat_<double>(3,3) << 0,0,0,0,0,0,0,0,0);
    cv::Mat computedTranslation = (cv::Mat_<double>(1,3) << 0,0,0);
    cv::Mat imagePoints = (cv::Mat_<double>(10,2) << 0,0,0,0,0,0,0,0,0,0);
    cv::Mat newImagePoints = (cv::Mat_<double>(10,2) << 0,0,0,0,0,0,0,0,0,0);
    cv::Mat rotationVector;
    cv::Mat computedProjectionMatrix;


    //Log file for writing results to
    std::ofstream fp;
    fp.open("assign3-out");

    ///Set up camera, translation, and rotation matrix and print to log file
    cv::Mat cameraMatrix = (cv::Mat_<double>(3,3) << 	intrinsic.at<double>(0,0), 0, 0,
                                                            0, intrinsic.at<double>(1,1), 0,
                                                            0 , 0, intrinsic.at<double>(2,2));

    cv::Mat rotationMatrix = (cv::Mat_<double>(3,3) << 	projection.at<double>(0,0), projection.at<double>(0,1), projection.at<double>(0,2),
                                                        projection.at<double>(1,0), projection.at<double>(1,1), projection.at<double>(1,2),
                                                        projection.at<double>(2,0) , projection.at<double>(2,1), projection.at<double>(2,2));

    cv::Mat translation = (cv::Mat_<double>(3,1) << projection.at<double>(0,3), projection.at<double>(1,3), projection.at<double>(2,3));
            
    std::cout << "Rotation matrix" << std::endl << rotationMatrix << std::endl;
    fp << "Rotation matrix" << std::endl << rotationMatrix << std::endl;
    std::cout << std::endl << "Translation vector" << std::endl; 
    fp << std::endl << "Translation vector" << std::endl;
    std::cout << translation << std::endl;
    fp << translation << std::endl;	
    std::cout << std::endl << "Camera Calibration" << std::endl << cameraMatrix << std::endl;
    fp << std::endl << "Camera Calibration" << std::endl << cameraMatrix << std::endl;

    std::cout << std::endl << "Intrinsic" << std::endl << intrinsic << std::endl;

    std::cout << std::endl << "Projection" << std::endl << projection << std::endl;

    std::cout << std::endl << "Object Points" << std::endl << objectPoints << std::endl;
    
    fp << std::endl << "Intrinsic" << std::endl << intrinsic << std::endl;

    fp << std::endl << "Projection" << std::endl << projection << std::endl;

    fp << std::endl << "Object Points" << std::endl << objectPoints << std::endl;

    cv::Mat transposedObjectPoints = objectPoints.t();
    cv::Mat finalProjection = intrinsic * projection; 

    std::cout << std::endl << "Projection" << std::endl << finalProjection << std::endl;
    fp << std::endl << "Projection" << std::endl << finalProjection << std::endl;
            
    cv::Mat transposedImagePoints = finalProjection * transposedObjectPoints; 
            
    std::cout << std::endl << "Transposed image points" << std::endl << transposedImagePoints << std::endl;    
    fp << std::endl << "Transposed image points" << std::endl << transposedImagePoints << std::endl;
            
    std::cout << std::endl << "Image points:" << std::endl;
    fp << std::endl << "Image points:" << std::endl;
    
    //Compute the image points x = u/w, y = v/w
    for (int i=0; i < NUM_POINTS; ++i) {     
            
        imagePoints.at<double>(i,0) = (transposedImagePoints.at<double>(0,i) / transposedImagePoints.at<double>(2,i));
        imagePoints.at<double>(i,1) = (transposedImagePoints.at<double>(1,i) / transposedImagePoints.at<double>(2,i));
        
        std::cout << "Image point " << i << " : (" << imagePoints.at<double>(i,0) << "," << imagePoints.at<double>(i,1) << ")" << std::endl; 
        fp << "Image point " << i << " : (" << imagePoints.at<double>(i,0) << "," << imagePoints.at<double>(i,1) << ")" << std::endl; 
    }

    ///Compute the projection matrix
    computeProjectionMatrix(imagePoints, objectPoints, computedProjectionMatrix, fp);
    
    std::cout << std::endl << std::endl << "==== Verify computed projection matrix ==== " << std::endl;
    fp << std::endl << std::endl << "==== Verify computed projection matrix ==== " << std::endl;
    cv::Mat compImagePts = computedProjectionMatrix * transposedObjectPoints;
    
    std::cout << std::endl << "Projection of orignal image points using computed M: " << std::endl;
    fp << std::endl << "Projection of orignal image points using computed M: " << std::endl;

    for (int i=0; i < NUM_POINTS; ++i) {     
            
        newImagePoints.at<double>(i,0) = (compImagePts.at<double>(0,i) / compImagePts.at<double>(2,i));
        newImagePoints.at<double>(i,1) = (compImagePts.at<double>(1,i) / compImagePts.at<double>(2,i));
        
        std::cout << "Image point " << i << " : (" << newImagePoints.at<double>(i,0) << "," << newImagePoints.at<double>(i,1) << ")" << std::endl; 
        fp << "Image point " << i << " : (" << newImagePoints.at<double>(i,0) << "," << newImagePoints.at<double>(i,1) << ")" << std::endl; 
    }
    
    cv::Mat deltas = imagePoints - newImagePoints;
    std::cout << std::endl << "[Image points from original M] - [Image points from computed M]: " << std::endl << deltas << std::endl;
    fp << std::endl << "[Image points from original M] - [Image points from computed M]: " << std::endl << deltas << std::endl;
    
    //Decompose the projection matrix
    decomposeProjectionMatrix(computedProjectionMatrix, computedRotationMatrix, computedTranslation, computedCameraMatrix, fp);
    

    fp.close();
    return 0;
}

////////////
/// computeProjectionMatrix
/// computes a projection matrix given object and image point correspondances using singular value decomposition
void computeProjectionMatrix(cv::Mat& imagePts, cv::Mat& objectPts, cv::Mat& projMat, std::ofstream& fp){
    //Create the A matrix which is a 2n by 12 matrix,
    //      [X1 Y1  Z1  1   0   0   0   0   -x1X1   -x1Y1   -x1Z1   -x1]
    //      [0  0   0   0   X1  Y1  Z1  1   -y1X1   -y1Y1   -y1Z1   -y1]
    // A =  [ ...
    //      [Xn Yn  Zn  1   0   0   0   0   -xnXn   -xnYn   -xnZn   -xn]
    //      [0  0   0   0   Xn  Yn  Zn  1   -ynXn   -ynYn   -ynZn   -yn]
    
    
    //Create and populate A
    cv::Mat a((NUM_POINTS *2) , A_COLS, cv::DataType<double>::type);
    
    for(int i=0; i < (NUM_POINTS * 2); i+=2){
        int n = i/2;
        
        double Xn = objectPts.at<double>(n,0);
        double Yn = objectPts.at<double>(n,1);
        double Zn = objectPts.at<double>(n,2);
        double x_n = imagePts.at<double>(n,0);
        double y_n = imagePts.at<double>(n,1);
        
        //row i
        a.at<double>(i,0) = Xn; //Xn
        a.at<double>(i,1) = Yn; //Yn
        a.at<double>(i,2) = Zn; //Zn
        a.at<double>(i,3) = 1.0; //1
        a.at<double>(i,4) = 0; //0
        a.at<double>(i,5) = 0; //0
        a.at<double>(i,6) = 0; //0
        a.at<double>(i,7) = 0; //0
        a.at<double>(i,8) = -(x_n * Xn); //-xnXn
        a.at<double>(i,9) = -(x_n * Yn); //-xnYn
        a.at<double>(i,10) = -(x_n * Zn); //-xnZn
        a.at<double>(i,11) = -(x_n); //-xn
        
        //row i+1
        a.at<double>(i+1,0) = 0; //0
        a.at<double>(i+1,1) = 0; //0
        a.at<double>(i+1,2) = 0; //0
        a.at<double>(i+1,3) = 0; //0
        a.at<double>(i+1,4) = Xn; //Xn
        a.at<double>(i+1,5) = Yn; //Yn
        a.at<double>(i+1,6) = Zn; //Zn
        a.at<double>(i+1,7) = 1.0; //1
        a.at<double>(i+1,8) = -1.0 * y_n * Xn; //-ynXn
        a.at<double>(i+1,9) = -1.0 * y_n * Yn; //-ynYn
        a.at<double>(i+1,10) = -1.0 * y_n * Zn; //-ynZn
        a.at<double>(i+1,11) = -1.0 * y_n; //-yn
        
    }
    
    std::cout << std::endl << std::endl << " ================== ComputeProjectionMatrix Function ========= " << std::endl;        
    fp << std::endl << std::endl << " ================== ComputeProjectionMatrix Function ========= " << std::endl;
    
    std::cout << std::endl << "A Matrix:" << std::endl << a << std::endl;
    fp << std::endl << "A Matrix:" << std::endl << a << std::endl;
        
    cv::SVD svd_a(a);
    
    //The right-singular vector corresponding to a zero singular value is a solution to the system of equations
    //Print the singular vector found in this way
    cv::Mat smallestSingVect = (cv::Mat_<double>(A_COLS,1) << 
                                svd_a.vt.at<double>(A_COLS-1, 0),
                                svd_a.vt.at<double>(A_COLS-1, 1),
                                svd_a.vt.at<double>(A_COLS-1, 2),
                                svd_a.vt.at<double>(A_COLS-1, 3),
                                svd_a.vt.at<double>(A_COLS-1, 4),
                                svd_a.vt.at<double>(A_COLS-1, 5),
                                svd_a.vt.at<double>(A_COLS-1, 6),
                                svd_a.vt.at<double>(A_COLS-1, 7),
                                svd_a.vt.at<double>(A_COLS-1, 8),
                                svd_a.vt.at<double>(A_COLS-1, 9),
                                svd_a.vt.at<double>(A_COLS-1, 10),
                                svd_a.vt.at<double>(A_COLS-1, 11));
    std::cout << std::endl << "Smallest Singular Vector: " << std::endl << smallestSingVect << std::endl;
    fp << std::endl << "Smallest Singular Vector: " << std::endl << smallestSingVect << std::endl;
    
    //Assign these values into the computed M matrix
    projMat = (cv::Mat_<double>(3,4) << 
                                svd_a.vt.at<double>(A_COLS-1, 0),
                                svd_a.vt.at<double>(A_COLS-1, 1),
                                svd_a.vt.at<double>(A_COLS-1, 2),
                                svd_a.vt.at<double>(A_COLS-1, 3),
                                svd_a.vt.at<double>(A_COLS-1, 4),
                                svd_a.vt.at<double>(A_COLS-1, 5),
                                svd_a.vt.at<double>(A_COLS-1, 6),
                                svd_a.vt.at<double>(A_COLS-1, 7),
                                svd_a.vt.at<double>(A_COLS-1, 8),
                                svd_a.vt.at<double>(A_COLS-1, 9),
                                svd_a.vt.at<double>(A_COLS-1, 10),
                                svd_a.vt.at<double>(A_COLS-1, 11));    
    
    std::cout << std::endl<< "M Matrix computed from singular vector: " << std::endl << projMat << std::endl;
    fp << std::endl << "M Matrix computed from singular vector: " << std::endl << projMat << std::endl;
   
}

////////////
/// decomposeProjectionMatrix
/// Decomposes a projection matrix into rotation, translation, and camera matrices
/// Based on Trucco & Verri, sec 6.3.2
void decomposeProjectionMatrix(cv::Mat& projMat, cv::Mat& rotMat, cv::Mat& transMat, cv::Mat& camMat, std::ofstream& fp){        
    std::cout << std::endl << std::endl << " ================== DecomposeProjectionMatrix Function ========= " << std::endl;        
    fp << std::endl << std::endl << " ================== DecomposeProjectionMatrix Function ========= " << std::endl;
    
    //compute gamma
    double gamma = sqrt( pow(projMat.at<double>(2,0), 2) + pow(projMat.at<double>(2,1), 2) + pow(projMat.at<double>(2,2), 2));
    
    std::cout << "Gamma: " << gamma << std::endl << std::endl;
    fp << "Gamma: " << gamma << std::endl << std::endl;
    
    //compute sigma
    double sigma = 1.0;
    if(projMat.at<double>(2, 3) < 0) sigma = -1.0;
    
    //Factor out the scalar gamma from computed projection matrix
    cv::Mat nProjMat = projMat * (1/gamma);
    
    std::cout << "Normalized Projection Matrix: " << std::endl << nProjMat << std::endl << std::endl; 
    fp << "Normalized Projection Matrix: " << std::endl << nProjMat << std::endl << std::endl; 

    //compute row 3 of the rotation matrix
    for(int i = 0; i < 3; ++i){
        rotMat.at<double>(2,i) = sigma * nProjMat.at<double>(2,i);
    }
    
    //extract tz:
    double tz = sigma * nProjMat.at<double>(2, 3);
    
    //Print the recovered Tz, r31, r32, r33
    std::cout << "Tz: " << tz << " r31: " << rotMat.at<double>(2,0) << " r32: " <<  rotMat.at<double>(2,1) << " r33: " << rotMat.at<double>(2,2) << std::endl << std::endl;
    fp << "Tz: " << tz << " r31: " << rotMat.at<double>(2,0) << " r32: " <<  rotMat.at<double>(2,1) << " r33: " << rotMat.at<double>(2,2) << std::endl << std::endl;   

    //Compute o_x = [q1].[q3]
    double o_x = (nProjMat.at<double>(0, 0) * nProjMat.at<double>(2, 0) ) + (nProjMat.at<double>(0, 1) * nProjMat.at<double>(2, 1)) + (nProjMat.at<double>(0, 2) * nProjMat.at<double>(2, 2));
    //o_y = [q2].[q3]
    double o_y = (nProjMat.at<double>(1, 0) * nProjMat.at<double>(2, 0)) + (nProjMat.at<double>(1, 1) * nProjMat.at<double>(2, 1)) + (nProjMat.at<double>(1, 2) * nProjMat.at<double>(2, 2));
    
    //Print the recovered ox, oy
    std::cout << "o_x: " << o_x << " o_y: " << o_y << std::endl << std::endl;
    fp << "o_x: " << o_x << " o_y: " << o_y << std::endl << std::endl;
    
    //Fx = sqrt(q1.q1 - o_x^2)    
    //Fy = sqrt(q2.q2 - o_y^2)
    double fx = sqrt(pow(nProjMat.at<double>(0, 0),2) + pow(nProjMat.at<double>(0, 1),2) + pow(nProjMat.at<double>(0, 2),2) - pow(o_x, 2));
    double fy = sqrt( pow(nProjMat.at<double>(1, 0),2) + pow(nProjMat.at<double>(1, 1),2) + pow(nProjMat.at<double>(1, 2),2) - pow(o_y, 2));
    
    std::cout << "fx: " << fx << " fy: " << fy << std::endl;
    
    //set the remaining rows 0, 1 of the rotation matrix
    for(int i = 0; i < 3; ++i){
        rotMat.at<double>(0,i) = sigma * (o_x * nProjMat.at<double>(2,i) - nProjMat.at<double>(0,i))/fx;
        rotMat.at<double>(1,i) = sigma * (o_y * nProjMat.at<double>(2,i) - nProjMat.at<double>(1,i))/fy;
    }
    
    //tx, ty
    double tx = sigma * 1/fx * (o_x * tz - nProjMat.at<double>(0, 3));    
    double ty = sigma * 1/fy * (o_y * tz - nProjMat.at<double>(1, 3));
    
    //set the translation matrix to the values that were recovered
    transMat.at<double>(0,0) = tx;
    transMat.at<double>(0,1) = ty;
    transMat.at<double>(0,2) = tz;
    
    //set the camera matrix values
    camMat.at<double>(0,0) = fx;
    camMat.at<double>(1,1) = fy;
    camMat.at<double>(2,2) = 1.0;
    camMat.at<double>(0,2) = o_x;
    camMat.at<double>(1,2) = o_y;
    
    //print the results to console and file    
    std::cout << std::endl << "----- Final results:" << std::endl;
    std::cout << "Rotation matrix " << std::endl << rotMat << std::endl << std::endl;
    std::cout << "Translation matrix " << std::endl << transMat << std::endl << std::endl;
    std::cout << "Camera matrix " << std::endl << camMat << std::endl << std::endl;  
    
    fp << std::endl << "----- Final results:" << std::endl;
    fp << "Rotation matrix " << std::endl << rotMat << std::endl << std::endl;
    fp << "Translation matrix " << std::endl << transMat << std::endl << std::endl;
    fp << "Camera matrix " << std::endl << camMat << std::endl << std::endl; 
    
}

