#include "miscellaneous.h"
#include <highgui.h>
#include <cmath>

int findColoredPerson(int *x,int *y,Mat* img)
{
// Brute force
//   http://www.aishack.in/tutorials/tracking-colored-objects-in-opencv/

printf("point 1\n");

  CvMat timg = (*img);
  IplImage* imgHSV = cvCreateImage(cvGetSize(&timg), 8, 3);
  cvCvtColor(&timg, imgHSV, CV_BGR2HSV);
  
printf("point 2\n");

  IplImage* imgThreshed = cvCreateImage(cvGetSize(&timg), 8, 1);
  cvInRangeS(imgHSV, cvScalar(20, 100, 100), cvScalar(30, 255, 255), imgThreshed);

printf("point 3\n");

  // Calculate the moments to estimate the position of the ball

  CvMoments moments; // = (CvMoments*)malloc(sizeof(CvMoments));

  cvMoments(imgThreshed, &moments, 1);

        // The actual moment values
  double moment10 = cvGetSpatialMoment(&moments, 1, 0);
  double moment01 = cvGetSpatialMoment(&moments, 0, 1);

  double area = cvGetCentralMoment(&moments, 0, 0);


  if(area)
  {
    *x = moment10/area;
    *y = moment01/area;
  }
  
  cvReleaseImage(&imgHSV);
 
  return 0;
}


void findYellowHelmet(Mat* img) {
    CvMat timg = (*img);
    IplImage* imgHSV = cvCreateImage(cvGetSize(&timg), 8, 3);
    cvCvtColor(&timg, imgHSV, CV_BGR2HSV);

    IplImage* imgThreshed = cvCreateImage(cvGetSize(&timg), 8, 1);
    cvInRangeS(imgHSV, cvScalar(20, 100, 100), cvScalar(30, 255, 255), imgThreshed);    
    Point_<int> center;
    findColoredPerson(&center.x, &center.y, img);
    
    queue<Point_<int> > q;    
    vector<vector<int> > mark(img->cols, vector<int>(img->rows, 0));    
    double radius = 0;
    int min_y = 10000000;
    
    q.push(center);    
    mark[center.x][center.y] = 1;
  
    while(!q.empty()){
        Point_<int> pt = q.front();
        q.pop();        
        radius = max(radius, sqrt(pow(pt.x-center.x,2) + pow(pt.y-center.y,2)));                    
        for (int i=-1; i<2; i++) {
            for (int j=-1; j<2; j++) {
                Point_<int> adj_pt(pt.x + i, pt.y + j);
                uchar* threshed_pixel = (uchar*) (
                        imgThreshed->imageData + adj_pt.y * imgThreshed->widthStep);
                if (0 < adj_pt.x && adj_pt.x < img->cols &&
                    0 < adj_pt.y && adj_pt.y < img->rows &&
                    !mark[adj_pt.x][adj_pt.y] && threshed_pixel[adj_pt.x + 1] == 255) {                    
                    mark[adj_pt.x][adj_pt.y] = 1;
                    threshed_pixel[adj_pt.x + 1] = 100;                   
                    q.push(adj_pt);
                }
            }
        }
    }    
        
    circle(*img, center, radius, cvScalar(80, 255, 20));
}