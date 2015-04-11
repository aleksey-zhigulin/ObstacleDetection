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

//bool cmp(TLine* a, TLine* b) {
//    return a->get_angle() < b->get_angle();
//}

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

int findRoadLines(Mat* img, int show){    
    
    Mat grey;
    cvtColor(*img, grey, CV_BGR2GRAY);

    vector<TLine*> lines; 
    LinesFromMat(&grey,&lines);
  
    
    vector<TLine*> goodlines;    
    for(int longLen = 40, i=0; i < lines.size(); i++) {                
        if (lines[i]->get_length() > longLen &&
            abs(lines[i]->get_angle()) > 10 && abs(lines[i]->get_angle()) < 80) {
            goodlines.push_back(lines[i]);                            
        }
    }
    
    //Choosing concrete 2 lines

    std::sort(goodlines.begin(), goodlines.end(), TLine::angle_cmp);       
    TLine left=*goodlines.front(), right=*goodlines.back();
    
    //Calc Vanish Point
    Point vanish = calcVanishPoint(left, right);
    
    //Extends the lines just for illustration    
    extendLine(left, 0, vanish.x);
    extendLine(right, vanish.x, img->cols);   
     
    if (show) {
        left.show(img);
        right.show(img);
        circle(*img, vanish, 5, CV_RGB(0,255,0));
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

Point2i calcVanishPoint(TLine &left, TLine &right) {
    int x1=left.pt1.x;
    int y1=left.pt1.y;

    int x2=left.pt2.x;
    int y2=left.pt2.y;

    int x3=right.pt1.x;
    int y3=right.pt1.y;

    int x4=right.pt2.x;
    int y4=right.pt2.y;
    
    int denom = ((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));    
        
    return Point2i(
        ((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/denom,
        ((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/denom
    );    
}

double slope(TLine &line) {    
    return (double)(line.pt1.y - line.pt2.y)/(line.pt1.x - line.pt2.x);
}

void extendLine(TLine& line, int from_x, int to_x) {
    double k = slope(line);
    double b = line.pt1.y - k * line.pt1.x;
    line.pt1.x = from_x;
    line.pt2.x = to_x;
    line.pt1.y = line.pt1.x * k + b;
    line.pt2.y = line.pt2.x * k + b;
}