///////////
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


#include "a3q1.h"

int main() {

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


    cv::Mat computedCameraMatrix;
    cv::Mat computedRotationMatrix;
    cv::Mat computedTranslation;
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

    cv::Mat rotationMatrix = (cv::Mat_<double>(3,3) << 	projection.at<double>(0,0), 0, 0,
                                                            0, projection.at<double>(1,1), 0,
                                                            0 , 0, projection.at<double>(2,2));

    cv::Mat translation = (cv::Mat_<double>(3,1) << projection.at<double>(0,3), projection.at<double>(1,3), projection.at<double>(2,3));
            
    std::cout << "Rotation matrix" << std::endl;
    std::cout << rotationMatrix << std::endl;
    fp << "Rotation matrix" << std::endl;
    fp << rotationMatrix << std::endl;
    std::cout << std::endl << "Translation vector" << std::endl;
    fp << std::endl << "Translation vector" << std::endl;
    std::cout << translation << std::endl;
    fp << translation << std::endl;	
    std::cout << std::endl << "Camera Calibration" << std::endl;
    std::cout << cameraMatrix << std::endl;
    fp << std::endl << "Camera Calibration" << std::endl;
    fp << cameraMatrix << std::endl;

    std::cout << std::endl << "Intrinsic" << std::endl;
    std::cout << intrinsic << std::endl;

    std::cout << std::endl << "Projection" << std::endl;
    std::cout << projection << std::endl;

    std::cout << std::endl << "Object Points" << std::endl;
    std::cout << objectPoints << std::endl;
    
    fp << std::endl << "Intrinsic" << std::endl;
    fp << intrinsic << std::endl;

    fp << std::endl << "Projection" << std::endl;
    fp << projection << std::endl;

    fp << std::endl << "Object Points" << std::endl;
    fp << objectPoints << std::endl;

    cv::Mat transposedObjectPoints = objectPoints.t();
    cv::Mat finalProjection = intrinsic * projection; 

    std::cout << std::endl << "Projection" << std::endl;
    std::cout << finalProjection << std::endl;
    fp << std::endl << "Projection" << std::endl;
    fp << finalProjection << std::endl;
            
    cv::Mat transposedImagePoints = finalProjection * transposedObjectPoints; 
            
    std::cout << std::endl << "Transposed image points" << std::endl;
    std::cout << transposedImagePoints << std::endl;
    
    fp << std::endl << "Transposed image points" << std::endl;
    fp << transposedImagePoints << std::endl;
            
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
    //decomposeProjectionMatrix(computedProjectionMatrix, computedRotationMatrix, computedTranslation, computedCameraMatrix);
     
    ///Print the results to console and file
    std::cout << std::endl << "Computed Rotation matrix" << std::endl;
    //std::cout << computed_rotation_matrix <<std::endl;

    std::cout << std::endl << "Computed Translation vector" << std::endl;
    //std::cout << computed_translation << std::endl;

    std::cout << std::endl << "Computed Camera Calibration" << std::endl;
    //std::cout << computed_camera_matrix << std::endl;

    fp << std::endl << "Computed Rotation matrix" << std::endl;
    //fp << computed_rotation_matrix <<std::endl;

    fp << std::endl << "Computed Translation vector" << std::endl;
    //fp << computed_translation << std::endl;

    fp << std::endl << "Computed Camera Calibration" << std::endl;
    //fp << computed_camera_matrix << std::endl;

    fp.close();
    return 0;
}

////////////
/// decomposeProjectionMatrix
/// Decomposes a projection matrix into rotation, translation, and camera matrices
void decomposeProjectionMatrix(cv::Mat& projMat, cv::Mat& rotMat, cv::Mat& transMat, cv::Mat& camMat){
    
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
    
    //for debugging
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



 

///////// This is the unused Eigenvalue solution -- I used SVD instead
    /*cv::Mat at_a = aTrans * a;
    
    std::cout << std::endl << " --------------------- A^T * A ----------" << std::endl << at_a << std::endl;
    
    cv::Mat E, V;
    cv::eigen(at_a,E,V);
    
    std::cout << "Eigenmatrix: " << E << std::endl;
    std::cout << "EigenVals: " << V << std::endl;
    
    cv::Mat smallestEig = (cv::Mat_<double>(A_COLS,1) << 
                                V.at<double>(A_COLS-1, 0),
                                V.at<double>(A_COLS-1, 1),
                                V.at<double>(A_COLS-1, 2),
                                V.at<double>(A_COLS-1, 3),
                                V.at<double>(A_COLS-1, 4),
                                V.at<double>(A_COLS-1, 5),
                                V.at<double>(A_COLS-1, 6),
                                V.at<double>(A_COLS-1, 7),
                                V.at<double>(A_COLS-1, 8),
                                V.at<double>(A_COLS-1, 9),
                                V.at<double>(A_COLS-1, 10),
                                V.at<double>(A_COLS-1, 11));
    std::cout << "Smallest Eigenvector: " << smallestEig << std::endl;
    
    ////Good to here
    
    cv::Mat compM = (cv::Mat_<double>(3,4) << 
                                V.at<double>(A_COLS-1, 0),
                                V.at<double>(A_COLS-1, 1),
                                V.at<double>(A_COLS-1, 2),
                                V.at<double>(A_COLS-1, 3),
                                V.at<double>(A_COLS-1, 4),
                                V.at<double>(A_COLS-1, 5),
                                V.at<double>(A_COLS-1, 6),
                                V.at<double>(A_COLS-1, 7),
                                V.at<double>(A_COLS-1, 8),
                                V.at<double>(A_COLS-1, 9),
                                V.at<double>(A_COLS-1, 10),
                                V.at<double>(A_COLS-1, 11));;
    
    
    std::cout << "Comp M: " << compM << std::endl;
    
    cv::Mat exampleObjPt = (cv::Mat_<double>(1,4) << /*80.8741, 58.5009, 47.9873, 1);* / 0.1251, 56.3585, 19.3304, 1);
    
    cv::Mat exampleImgPt = exampleObjPt * compM.t();
    
    std::cout << "Sample projection Eigen" << std::endl << exampleObjPt << std::endl << "Point: " << (exampleImgPt.at<double>(0,0) / exampleImgPt.at<double>(0,2)) << "," << (exampleImgPt.at<double>(0,1) / exampleImgPt.at<double>(0,2)) << ")" << std::endl;*/
