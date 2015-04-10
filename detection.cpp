#include "detection.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;

int findColoredPerson(int *x,int *y,Mat* img) {
//   Brute force
//   http://www.aishack.in/tutorials/tracking-colored-objects-in-opencv/
    
    Mat imgHSV;
    cvtColor(*img, imgHSV, CV_BGR2HSV);

    Mat imgThreshed;
    inRange(imgHSV, Scalar(20, 100, 100), Scalar(30, 255, 255), imgThreshed);

    // Calculate the moments to estimate the position of the ball

    Moments m = moments(imgThreshed, true);

    if(m.m00) {
        *x = m.m10/m.m00;
        *y = m.m01/m.m00;
    }    

    return 0;
}

