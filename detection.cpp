#include "detection.h"
#include "TLine.h"

#include <iostream>
#include <cmath>
#include <algorithm>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include "lsd.h"

using namespace cv;
using namespace std;


int findColoredObject(int *x,int *y,Mat* img) {
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


void LinesFromMat(Mat *image, vector<TLine*> *lines) { //image must be in grayscale!
	
    double *output;
    //input for LSD call, that contains all of the pixels of the image
    double *input = (double *) malloc( image->cols * image->rows * sizeof(double) );
    int i=0;

    for(int y = 0; y < image->rows; y++)
        for(int x = 0; x < image->cols; x++) {
            input[i]=(double)image->data[image->step[0]*y + image->step[1]*x];
            i++;
        }

    int lcnt=0;
        
    //LSD call
    output = lsd(&lcnt,input,image->cols,image->rows);    

    int k=0;    
    for(i=0;i<lcnt;i++) {	
          double x1=output[7*i+0],x2=output[7*i+2],y1=output[7*i+1],y2=output[7*i+3];
          if(!((abs(x1-x2)+abs(y1-y2))<((image->rows)*(image->cols))/50000)) {                            
              lines->push_back(new TLine(x1,y1,x2,y2));              
              k++;
          }
    }

    free(input);
    free(output);
	
}



