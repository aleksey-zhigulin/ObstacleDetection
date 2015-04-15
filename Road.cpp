/* 
 * File:   Road.cpp
 * Author: aleksey
 * 
 * Created on April 13, 2015, 12:00 PM
 */

#include "Road.h"
#include "TLine.h"
#include "detection.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

Road::Road() {
}

Road::~Road() {
}

Road::Road(Mat *frame) {        
    img = *frame;
    //calc road borders
    findBorders();
    
    //calc vanish point
    calcVanishPoint();
    
    //extend border lines abscisses from 0 to vanish_point
    left_border.extend(0, vanish_point.x);
    right_border.extend(vanish_point.x, img.cols);
    
    //calc road rect
    calcRoadRect();
    
    //cutting road from img    
    cutRoadRect();
        
    //transform road perspective    
    perspectiveTransform();
}

void Road::findBorders(){    
    
    Mat grey;
    cvtColor(img, grey, CV_BGR2GRAY);

    vector<TLine*> lines; 
    LinesFromMat(&grey,&lines);
      
    vector<TLine*> goodlines;    
    for(int longLen = 40, i=0; i < lines.size(); i++) {                
        if (lines[i]->getLength() > longLen &&
            abs(lines[i]->getAngle()) > 10 && abs(lines[i]->getAngle()) < 80) {
            goodlines.push_back(lines[i]);                            
        }
    }
    
    //Choosing concrete 2 lines

    std::sort(goodlines.begin(), goodlines.end(), TLine::angleCmp);       
    left_border = *goodlines.front();
    right_border = *goodlines.back();
            
}

void Road::calcVanishPoint() {
    int x1=left_border.pt1.x;
    int y1=left_border.pt1.y;

    int x2=left_border.pt2.x;
    int y2=left_border.pt2.y;

    int x3=right_border.pt1.x;
    int y3=right_border.pt1.y;

    int x4=right_border.pt2.x;
    int y4=right_border.pt2.y;
    
    int denom = ((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));    
    vanish_point = Point2i(
        ((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/denom,
        ((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/denom
    );        
}

void Road::calcRoadRect() {    
    int top_left_x = std::max(0, left_border.x(img.rows)); 
    int top_left_y = vanish_point.y + 10;
    int width = std::min(img.cols, right_border.x(img.rows)) - top_left_x;
    int height = img.rows - top_left_y;  
    
    road_rect = Rect(top_left_x, top_left_y, width, height);       
}

void Road::cutRoadRect() {        
    road = img(road_rect).clone();
}

void Road::perspectiveTransform() {  
    //calc road polygon
    Point2f src[4], dst[4];
    //top left
    src[0].y = 0;
    src[0].x = left_border.x(road_rect.y) - road_rect.x;
    //top right
    src[1].y = 0;
    src[1].x = right_border.x(road_rect.y) - road_rect.x;
    //bottom left
    src[2].x = 0;
    src[2].y = road.rows;
    //bottom right
    src[3].x = road.cols;
    src[3].y = road.rows;
    
    dst[0] = Point2f(0, 0);
    dst[1] = Point2f(road.cols, 0);
    dst[2] = src[2];
    dst[3] = src[3];
    
    Mat warp_matrix = getPerspectiveTransform(src, dst);    
    warpPerspective(road, road_transformed, warp_matrix, road.size());           
}

void Road::show() {
    circle(img, vanish_point, 5, CV_RGB(0, 255, 0));
    line(img, left_border.pt1, left_border.pt2, CV_RGB(255,0,0), 3);
    line(img, right_border.pt1, right_border.pt2, CV_RGB(255,0,0), 3);
    namedWindow("road borders and vanish point");
    imshow("road borders and vanish point", img);
    namedWindow("cutted road");
    imshow("cutted road", road);    
    namedWindow("road perspective");
    imshow("road perspective", road_transformed);
    waitKey(0);    
}