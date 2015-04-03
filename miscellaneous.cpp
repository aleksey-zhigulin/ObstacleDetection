#include "miscellaneous.h"
#include <highgui.h>
#include <cmath>

void findYellowHelmet(Mat* img) {
    CvMat timg = (*img);
    IplImage* imgHSV = cvCreateImage(cvGetSize(&timg), 8, 3);
    cvCvtColor(&timg, imgHSV, CV_BGR2HSV);

    IplImage* imgThreshed = cvCreateImage(cvGetSize(&timg), 8, 1);
    cvInRangeS(imgHSV, cvScalar(20, 100, 100), cvScalar(30, 255, 255), imgThreshed);    
    Mat ma(imgThreshed, true);
    imwrite("/vagrant/ObstacleDetection/threshed.jpg", ma);   
    
    Point_<int> center;
    ObjectDetectionStub::findColoredPerson(&center.x, &center.y, img);
    
    queue<Point_<int> > q;    
    vector<vector<int> > mark(img->cols, vector<int>(img->rows, 0));    
    double radius = 0;
    int min_y = 10000000;
    
    q.push(center);    
    mark[center.x][center.y] = 1;
    
    while(!q.empty()){
        Point_<int> pt = q.front();
        q.pop();
        radius = max(radius, sqrt((pt.x-center.x)^2 + (pt.y-center.y)^2));                    
        for (int i=-1; i<2; i++) {
            for (int j=-1; j<2; j++) {
                Point_<int> adj_pt(pt.x + i, pt.y + j);
                uchar* threshed_pixel = (uchar*) (
                        imgThreshed->imageData + adj_pt.x * imgThreshed->widthStep);
                if (0 < adj_pt.x && adj_pt.x < img->cols &&
                    0 < adj_pt.y && adj_pt.y < img->rows &&
                    !mark[adj_pt.x][adj_pt.y] && threshed_pixel[adj_pt.y + 1] == 0) {                    
                    mark[adj_pt.x][adj_pt.y] = 1;
                    min_y = min(min_y, adj_pt.y);                    
                    q.push(adj_pt);
                }
            }
        }
    }            
    cout << radius << ' ' << min_y << endl;
    circle(*img, center, radius, cvScalar(80, 255, 20));
}