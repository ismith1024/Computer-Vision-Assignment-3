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

#include "a3q1.h"

int main() {

    cv::Mat objectPoints = (cv::Mat_<double>(10,4) << 0.1251, 56.3585, 19.3304, 1,
                                                        80.8741, 58.5009, 47.9873, 1,
                                                        35.0291, 89.5962, 82.2840, 1,
                                                        74.6605, 17.4108, 85.8943, 1,
                                                        71.0501, 51.3535, 30.3995, 1,
                                                        1.4985, 9.1403, 36.4452, 1,
                                                        14.7313, 16.5899, 98.8525, 1,
                                                        44.5692, 11.9083, 0.4669, 1,
                                                        0.8911, 37.7880, 53.1663, 1,
                                                        57.1184, 60.1764, 60.7166, 1);


    cv::Mat projection = (cv::Mat_<double>(3,4) <<  0.902701, 0.051530, 0.427171, 12.0, 
                                                    0.182987, 0.852568, -0.489535, 16.0, 
                                                    -0.389418, 0.520070, 0.760184, 21.0);

    cv::Mat intrinsic = (cv::Mat_<double>(3,3) << 	-1100.000000, 0.000000, 0.000000, 
                                                    0.000000, -2200.000000, 0.000000, 
                                                    0.000000, 0.000000, 1.000000);


    cv::Mat computedCameraMatrix;
    cv::Mat computedRotationMatrix;	
    cv::Mat computedTranslation;
    cv::Mat imagePoints = (cv::Mat_<double>(10,2) << 0,0,0,0,0,0,0,0,0,0);
    cv::Mat rotationVector;

    cv::Mat computedProjectionMatrix;


    //File for writing to
    std::ofstream fp;
    fp.open("assign3-out");

    ///Set up camera, translation, and rotation matrix
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

    //TODO -- Project the object points             
    cv::Mat transposedObjectPoints = objectPoints.t();
    cv::Mat finalProjection = intrinsic * projection; //cvMatMul(&temp_intrinsic, &temp_projection, final_projection);

    std::cout << std::endl << "Final Projection" << std::endl;
    std::cout << finalProjection << std::endl;
            
    cv::Mat transposedImagePoints = finalProjection * transposedObjectPoints; 
            
    std::cout << std::endl << "Transposed image points" << std::endl;
    std::cout << transposedImagePoints << std::endl;
            
    //std::cout << "Temp transposed image points: " << std::endl << transposedImagePoints.t() << std::endl;
            
    std::cout << std::endl << "Image points:" << std::endl;

    //Compute the image points x = u/w, y = v/w
    for (int i=0; i < NUM_POINTS; ++i) {     
            
        imagePoints.at<double>(i,0) = (transposedImagePoints.at<double>(0,i) / transposedImagePoints.at<double>(2,i));
        imagePoints.at<double>(i,1) = (transposedImagePoints.at<double>(1,i) / transposedImagePoints.at<double>(2,i));
        
        std::cout << "Image point " << i << " : (" << imagePoints.at<double>(i,0) << "," << imagePoints.at<double>(i,1) << ")" << std::endl; 
        fp << "Image point " << i << " : (" << imagePoints.at<double>(i,0) << "," << imagePoints.at<double>(i,1) << ")" << std::endl; 
    }

    //TODO -- write function to compute the projection matrix from image and object point correspondances
    //TODO -- write function to decompose this projection matrix
    //computeProjectionMatrix(imagePoints, objectPoints, computedProjectionMatrix);
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
/// computes a projection matrix given object and image point correspondances
void computeProjectionMatrix(cv::Mat& imagePts, cv::Mat& objectPts, cv::Mat& projMat){
    
    
    
}




/////////////// C STRUCT INITIALIZER FUNCTIONS
/*
	//cvInitMatHeader(&temp_projection, 3, 4, CV_32FC1, projection);
	//cvInitMatHeader(&temp_intrinsic, 3, 3, CV_32FC1, intrinsic);

	//final_projection = cvCreateMat(3, 4, CV_32F);

	//object_points = cvCreateMat(NUM_POINTS, 4, CV_32F);
	//transp_object_points = cvCreateMat(4, NUM_POINTS, CV_32F);

	//image_points = cvCreateMat(NUM_POINTS, 3, CV_32F);
	//transp_image_points = cvCreateMat(3, NUM_POINTS, CV_32F);

	//rot_vector = cvCreateMat(3, 1, CV_32F);
    //camera_matrix = cvCreateMat(3, 3, CV_32F);
    //rotation_matrix = cvCreateMat(3, 3, CV_32F);
    //translation = cvCreateMat(3, 1, CV_32F);

    //computed_camera_matrix = cvCreateMat(3, 3, CV_32F);
    //computed_rotation_matrix = cvCreateMat(3, 3, CV_32F);
    //computed_translation = cvCreateMat(3, 1, CV_32F);
	//computed_projection_matrix = cvCreateMat(3, 4, CV_32F);

*/

/*float projection[3][4] = {
0.902701, 0.051530, 0.427171, 12.0,
0.182987, 0.852568, -0.489535, 16.0,
-0.389418, 0.520070, 0.760184, 21.0,
};

float intrinsic[3][3] = {
-1100.000000, 0.000000, 0.000000, 
0.000000, -2200.000000, 0.000000, 
0.000000, 0.000000,     1.000000,
};

float all_object_points[10][3] = {
0.1251, 56.3585, 19.3304, 
80.8741, 58.5009, 47.9873,
35.0291, 89.5962, 82.2840,
74.6605, 17.4108, 85.8943,
71.0501, 51.3535, 30.3995,
1.4985, 9.1403, 36.4452,
14.7313, 16.5899, 98.8525,
44.5692, 11.9083, 0.4669,
0.8911, 37.7880, 53.1663,
57.1184, 60.1764, 60.7166
};

*/
