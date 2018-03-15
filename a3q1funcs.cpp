/*
	
For assignment 3 Question 1 the relevant section in the Trucco and Verri book is section 6.3. Also, I have the files project_deriv_(a,b,c).pdf uploaded on CuLearn and they have the same information as in the book, but in more detail. 

Some extra hints for Question 1 of Assign3. You first create the A matrix which is a 2n by 12 matrix, where n is the number of given 3d to 2d correspondences. Now you need to find the eigenvectors and eigenvalues of A transpose times A (a 12 by 12 matrix). There are two ways to do this, using cv2.eigen or numpy.linalg.svd (there are actually more ways but these are the ones I will take about). They are shown below, in Python. First I define a 4 by 3 matrix  A (you will have a 2n by 12 matrix), compute A transpose A, and then find the eigenvectors and eigenvalues of that matrix. Note that the resulting eigenvalues from cv2.eigen are returned in order from largest to smallest, and the eigenvectors are returned in the same order. Look at the documentaton in https://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html#eigen

I use cv2.eigen in this python code and then show how to use r, u, s, v = numpy.linalg.svd(A) instead of cv2.eigen. When using the SVD routine instead of eigen the eigenvectors are returned in v in the order of largest to smallest eigenvalue (you need the last entry, the eigenvector associated with the smallest eigenvalue). In this case the original A is input to svd, this is the matrix of size 2*n by 12, where n is the number of 3d points, and not the matrix A transpose times A that was input to eigen (using svd does save you having to perform this matrix multiplication). Both approaches work, and you need the eigenvector associated with the smallest eigenvalue, but you do not actually need the eigenvalue.

Once you have the eigenvector associated with the smallest eigenvalue (a 12 element vector) you create the projection matrix from this eigenvector (one row at a time). You can now apply this computed projection matrix to the original 3d points and verify that the result is the same original set of 2d points. If that is true then you have computed a good projection matrix. Now you use the info in project_deriv_(a,b,c).pdf to compute the K, R and T from the numbers in the computed projection matrix and verify that they are the same as the original K, R and T. 

So to conclude there are two distinct steps to Question 1. First use the given 3d to 2d correspondences to compute the projection matrix. Then decompose this projection matrix to find K, R and T. This should be enough to do Question 1 of assignment 3. 


*/

////////////
/// computeProjectionMatrix
/// computes a projection matrix given object and image point correspondances
void computeProjectionMatrix(cv::Mat& imagePts, cv::Mat& objectPts, cv::Mat& projMat){
    //You first create the A matrix which is a 2n by 12 matrix,
    //      [X1 Y1  Z1  1   0   0   0   0   -x1X1   -x1Y1   -x1Z1   -x1]
    //      [0  0   0   0   X1  Y1  Z1  1   -y1X1   -y1Y1   -y1Z1   -y1]
    // A =  [ ...
    //      [Xn Yn  Zn  1   0   0   0   0   -xnXn   -xnYn   -xnZn   -xn]
    //      [0  0   0   0   Xn  Yn  Zn  1   -ynXn   -ynYn   -ynZn   -yn]
    
    
    //Create and populate A
    cv::Mat a((NUM_POINTS *2) , A_COLS, cv::DataType<double>::type);
    
    for(int i=0; i < (NUM_POINTS * 2); i+=2){
        float Xn = objectPts.at<double>(i,0);
        float Yn = objectPts.at<double>(i,1);
        float Zn = objectPts.at<double>(i,2);
        float x_n = imagePts.at<double>(i,0);
        float y_n = imagePts.at<double>(i,1);
        
        //row i
        a.at<double>(i,0) = Xn; //Xn
        a.at<double>(i,1) = Yn; //Yn
        a.at<double>(i,2) = Zn; //Zn
        a.at<double>(i,3) = 1; //1
        a.at<double>(i,4) = 0; //0
        a.at<double>(i,5) = 0; //0
        a.at<double>(i,6) = 0; //0
        a.at<double>(i,7) = 0; //0
        a.at<double>(i,8) = -1 * x_n * Xn; //-xnXn
        a.at<double>(i,9) = -1 * x_n * Yn; //-xnYn
        a.at<double>(i,10) = -1 * x_n * Zn; //-xnZn
        a.at<double>(i,11) = -1 * x_n; //-xn
        
        //row i+1
        a.at<double>(i+1,0) = 0; //0
        a.at<double>(i+1,1) = 0; //0
        a.at<double>(i+1,2) = 0; //0
        a.at<double>(i+1,3) = 0; //0
        a.at<double>(i+1,4) = Xn; //Xn
        a.at<double>(i+1,5) = Yn; //Yn
        a.at<double>(i+1,6) = Zn; //Zn
        a.at<double>(i+1,7) = 1; //1
        a.at<double>(i+1,8) = -1 * y_n * Xn; //-ynXn
        a.at<double>(i+1,9) = -1 * y_n * Yn; //-ynYn
        a.at<double>(i+1,10) = -1 * y_n * Zn; //-ynZn
        a.at<double>(i+1,11) = -1 * y_n; //-yn
        
    }
    
    cv::SVD svd_a(a);
    
    std::cout << "SVD of A:  " << svd_a << std::endl;
    
}


////////////
/// decomposeProjectionMatrix
/// Decomposes a projection matrix into rotation, translation, and camera matrices
void decomposeProjectionMatrix(cv::Mat& projMat, cv::Mat& rotMat, cv::Mat& transMat, cv::Mat& camMat){
    
}

