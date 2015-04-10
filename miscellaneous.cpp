#include "miscellaneous.h"
#include "detection.h"

#include <queue>
#include <cmath>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace std;


void findYellowHelmet(Mat* img) {    
    Mat imgHSV;
    cvtColor(*img, imgHSV, CV_BGR2HSV);

    Mat imgThreshed;
    inRange(imgHSV, Scalar(20, 100, 100), Scalar(30, 255, 255), imgThreshed);    
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
                if (0 < adj_pt.x && adj_pt.x < img->cols &&
                    0 < adj_pt.y && adj_pt.y < img->rows &&
                    !mark[adj_pt.x][adj_pt.y] && imgThreshed.at<uchar>(adj_pt) == 255) {                    
                    mark[adj_pt.x][adj_pt.y] = 1;                                      
                    q.push(adj_pt);
                }
            }
        }
    }    
        
    circle(*img, center, radius, cvScalar(80, 255, 20));
}
