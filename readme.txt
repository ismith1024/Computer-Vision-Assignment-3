COMP4102 
Assignment 2
Submitted by Ian Smith #100910972
2018-02-28

OpenCV version : 3.3.1
Major version : 3
Minor version : 3
Subminor version : 1


===== Q1:

Assumptions made:
    - The image classifier relies on looking exclusively at the 6 largest polygons to find signs.  This is to avoid false positive stop signs when arbitrary octagons are found in the background noise, and can be interpreted as "I assume the sign is close to the camera".
    - The Canny threshold is adaptive and is computed using the median pixel value in the image, so this assumes the grayscale image of the sign has high contrast
    - The blur size assmes that the images are of comparable pixel dimensions to the image files used in the assignment -- cv::Size(4,4) worked best for me
    - The minimum matchtemplate() threshold for the 40/80 signs is set slightly higher than the matchtemplate() value for the 100 km/h sign.  I am assuming that the 100 sign in the assignment is "typical" when setting the minimum threshold for 40/80.  In a real application, I would look at the statistical values for many 100 km/h signs to set the 40/80 threshold.
    
Instructions to run the application:
    - Code is in: a2q1.cpp, imageclassifier.h, imageclassifier.cpp 
    - C++ standard used was c++14
    - To compile:
    g++ -std=c++14 a2q1.cpp imageclassifier.cpp -o a2q1 `pkg-config --libs opencv`
    - PLEASE NOTE: The image filename is passed to the program as a command line parameter (as opposed to a hardcoded string like the template code):
            ./a2q1 speedsign5.jpg

===== Q2:

I was able to obtain 8 lines by setting edgeThresh to 100.  The rest of the code was based on the examples, so no real assumptions.

To run:
    - Code is in a2q2.cpp 
    - C++ standard used was c++14
    - To compile:
    g++ -std=c++14 a2q2.cpp -o a2q2 `pkg-config --libs opencv`
    - The filename is a parameter as in Q1:
            ./a2q2 track.jpg
