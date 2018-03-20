COMP4102 
Assignment 3
Submitted by Ian Smith #100910972
2018-03-21

OpenCV version : 3.3.1
Major version : 3
Minor version : 3
Subminor version : 1


===== Q1:

I have rewritten most of the template using cv::Mat since I like the C++ class better than the C struct.  I used a singular value decomposition approach instead of eigenvalues.
    
Instructions to run the application:
    - Code is in: a3q1.cpp, a3q1.h 
    - C++ standard used was c++14
    - To compile:
    g++ -std=c++14 a3q1.cpp -o a3q1 `pkg-config --libs opencv`
  

===== Q2:

"Notice that the final merged image has some anomalies because of the OR
operation. In real mosaicking programs you do not see these anomalies. Write down a short
(one paragraph) description of how you would get rid of these visible anomalies and include
it in your upload. The answer is simple."

After inspecting the pixels in both the warped and center image around the region of the anomalies, it is clear that there
are no near-white pixels in either image.  So, I am assuming they result from the mathematical merging of the pixel values.
One possibility is the bitwise OR operation -- for example, 01000 0000 OR 0011 1111 = 0111 1111.

I have included a second merged image that calculates pixel values as follows:

Mean value of the pixel in both the warped and center image, unless exactly one of the images has a black pixel there -- in this case it is the 
value from the other image.
    
The merged image obtained in this way is submitted as "corrected.jpg".  The merged image from OR with the anomalies is "merged.jpg".

To run:
    - Code is in a3q2.cpp 
    - C++ standard is c++14
    - To compile:
    g++ -std=c++14 a3q2.cpp -o a3q2 `pkg-config --libs opencv`

Most of the template code is unchanged.