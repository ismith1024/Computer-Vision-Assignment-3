#include "opencv\cv.h"
#include "opencv\cxcore.h"
#include "opencv\highgui.h"
#include "opencv2/calib3d/calib3d.hpp"
#include <math.h>

const int NUM_POINTS = 10;
const float RANGE = 100.00
const int MAX_CAMERAS = 100; 
const int MAX_POINTS = 3000;

const float projection[3][4] = {
0.902701, 0.051530, 0.427171, 12.0,
0.182987, 0.852568, -0.489535, 16.0,
-0.389418, 0.520070, 0.760184, 21.0,
};

const float intrinsic[3][3] = {
-1100.000000, 0.000000, 0.000000, 
0.000000, -2200.000000, 0.000000, 
0.000000, 0.000000,     1.000000,
};

const float all_object_points[10][3] = {
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

void decomposeprojectionmatrix(CvMat*, CvMat*, CvMat*, CvMat*);

void computeprojectionmatrix(CvMat*, CvMat*, CvMat*);