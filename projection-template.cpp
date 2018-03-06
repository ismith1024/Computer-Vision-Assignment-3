#include <stdio.h>
#include <string>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include "opencv2/calib3d/calib3d.hpp"


#include <math.h>


#define NUM_POINTS    10
#define RANGE         100.00

#define MAX_CAMERAS   100 
#define MAX_POINTS    3000


float projection[3][4] = {
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

// you write this routine
void decomposeprojectionmatrix(CvMat* projection_matrix, CvMat* rotation_matrix, CvMat* translation, CvMat* camera_matrix)
{
}

// you write this routine
void computeprojectionmatrix(CvMat* image_points, CvMat* object_points, CvMat* projection_matrix)
{
}

int main() {
    CvMat* camera_matrix, *computed_camera_matrix; //
    CvMat* rotation_matrix, *computed_rotation_matrix; //
    CvMat* translation, *computed_translation;
	CvMat* image_points, *transp_image_points;
    CvMat* rot_vector;
    CvMat* object_points, *transp_object_points;
	CvMat* computed_projection_matrix;
	CvMat *final_projection;
	CvMat temp_projection, temp_intrinsic;
	FILE *fp;

	cvInitMatHeader(&temp_projection, 3, 4, CV_32FC1, projection);
	cvInitMatHeader(&temp_intrinsic, 3, 3, CV_32FC1, intrinsic);

	final_projection = cvCreateMat(3, 4, CV_32F);

	object_points = cvCreateMat(NUM_POINTS, 4, CV_32F);
	transp_object_points = cvCreateMat(4, NUM_POINTS, CV_32F);

	image_points = cvCreateMat(NUM_POINTS, 3, CV_32F);
	transp_image_points = cvCreateMat(3, NUM_POINTS, CV_32F);

	rot_vector = cvCreateMat(3, 1, CV_32F);
    camera_matrix = cvCreateMat(3, 3, CV_32F);
    rotation_matrix = cvCreateMat(3, 3, CV_32F);
    translation = cvCreateMat(3, 1, CV_32F);

    computed_camera_matrix = cvCreateMat(3, 3, CV_32F);
    computed_rotation_matrix = cvCreateMat(3, 3, CV_32F);
    computed_translation = cvCreateMat(3, 1, CV_32F);
	computed_projection_matrix = cvCreateMat(3, 4, CV_32F);

	fp = fopen("assign3-out","w");

	fprintf(fp, "Rotation matrix\n");
	for (int i=0; i<3; i++) {
   		for (int j=0; j<3; j++) {
    		cvmSet(camera_matrix,i,j, intrinsic[i][j]);
    		cvmSet(rotation_matrix,i,j, projection[i][j]);
            }
		fprintf(fp, "%f %f %f\n", 
			cvmGet(rotation_matrix,i,0), cvmGet(rotation_matrix,i,1), cvmGet(rotation_matrix,i,2));
	}
        
        for (int i=0; i<3; i++) cvmSet(translation, i, 0, projection[i][3]);

	fprintf(fp, "\nTranslation vector\n");
	fprintf(fp, "%f %f %f\n", 
		cvmGet(translation,0,0), cvmGet(translation,1,0), cvmGet(translation,2,0));

	fprintf(fp, "\nCamera Calibration\n");
	for (int i=0; i<3; i++) {
		fprintf(fp, "%f %f %f\n", 
			cvmGet(camera_matrix,i,0), cvmGet(camera_matrix,i,1), cvmGet(camera_matrix,i,2));
	}

	fprintf(fp,"\n");
	for (int i = 0; i < NUM_POINTS; i++) {
		cvmSet(object_points, i, 0, all_object_points[i][0]);
		cvmSet(object_points, i, 1, all_object_points[i][1]);
		cvmSet(object_points, i, 2, all_object_points[i][2]);
		cvmSet(object_points, i, 3, 1.0);
		fprintf(fp, "Object point %d x %f y %f z %f\n", 
			i, all_object_points[i][0], all_object_points[i][1], all_object_points[i][2]);
	}
	fprintf(fp, "\n");
	cvTranspose(object_points, transp_object_points);        
        

	cvMatMul(&temp_intrinsic, &temp_projection, final_projection);
	cvMatMul(final_projection, transp_object_points, transp_image_points);
	//cvTranspose(transp_image_points, image_points);
    
        //print the final projection
        printf("\nFinal Projection\n");
        for (int i=0; i<3; i++) {
		printf("%f %f %f %f\n", cvmGet(final_projection, i, 0), cvmGet(final_projection, i, 1), cvmGet(final_projection, i, 2), cvmGet(final_projection, i, 3));
	}
	
	/*
         Final projection:
         -992.971130 -56.682999 -469.888092 -13200.000000
        -402.571411 -1875.649536 1076.977051 -35200.000000
        -0.389418 0.520070 0.760184 21.000000
         */
        
        //print the transposed image
        printf("\nTransposed image\n");
        for (int i=0; i<10; i++) {
            for(int j = 0; j < 3; ++j){
		printf("%f ", cvmGet(transp_image_points, j, i));                
            }
            printf("\n");
	}
	
	/*
            -25601.914062 -120140.757812 64.956314 
            -119370.312500 -125803.562500 56.409912 
            -91725.734375 -128734.804688 116.506310 
            -128683.328125 -5406.550781 66.276169 
            -100945.937500 -127384.343750 43.148441 
            -32331.232422 -13696.606445 52.875111 
            -75217.734375 34214.636719 99.037361 
            -58350.316406 -74975.242188 10.192032 
            -41208.984375 -49176.886719 80.721565 
            -101857.906250 -105673.687500 76.208794     
         */


	for (int i=0; i<NUM_POINTS; i++) {
		cvmSet(image_points, i, 0, cvmGet(transp_image_points, 0, i)/cvmGet(transp_image_points, 2, i));
		cvmSet(image_points, i, 1, cvmGet(transp_image_points, 1, i)/cvmGet(transp_image_points, 2, i));
		fprintf(fp, "Image point %d x %f y %f\n", 
			i, cvmGet(image_points, i, 0), cvmGet(image_points, i, 1));
	}
	
	/* Image points:
                Image point 0 x -394.140503 y -1849.562378
                Image point 1 x -2116.123047 y -2230.167725
                Image point 2 x -787.302734 y -1104.959961
                Image point 3 x -1941.622925 y -81.576088
                Image point 4 x -2339.503662 y -2952.235107
                Image point 5 x -611.464111 y -259.036926
                Image point 6 x -759.488464 y 345.472015
                Image point 7 x -5725.091797 y -7356.260742
                Image point 8 x -510.507751 y -609.216248
                Image point 9 x -1336.563721 y -1386.633789  
         
         */
	

	/*computeprojectionmatrix(image_points, object_points, computed_projection_matrix);
	decomposeprojectionmatrix(computed_projection_matrix, computed_rotation_matrix, computed_translation, computed_camera_matrix);

	fprintf(fp, "\nComputed Rotation matrix\n");
	for (int i=0; i<3; i++) {
		fprintf(fp, "%f %f %f\n", 
			cvmGet(computed_rotation_matrix,i,0), cvmGet(computed_rotation_matrix,i,1), cvmGet(computed_rotation_matrix,i,2));
	}

	fprintf(fp, "\nComputed Translation vector\n");
	fprintf(fp, "%f %f %f\n", 
		cvmGet(computed_translation,0,0), cvmGet(computed_translation,1,0), cvmGet(computed_translation,2,0));

	fprintf(fp, "\nComputed Camera Calibration\n");
	for (int i=0; i<3; i++) {
		fprintf(fp, "%f %f %f\n", 
			cvmGet(computed_camera_matrix,i,0), cvmGet(computed_camera_matrix,i,1), cvmGet(computed_camera_matrix,i,2));
	}*/

    fclose(fp);
    return 0;
}
