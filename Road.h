/* 
 * File:   Road.h
 * Author: aleksey
 *
 * Created on April 13, 2015, 12:00 PM
 */

#ifndef ROAD_H
#define	ROAD_H

#include "opencv2/core/core.hpp"
#include "TLine.h"

using namespace cv;

class Road {
public:
    Road();    
    Road(Mat*);        
    void show();
    virtual ~Road();
private:
    Mat img, road, road_transformed;    
    TLine left_border, right_border;
    Point2i vanish_point;
    Rect road_rect;
    void findBorders();
    void perspectiveTransform();
    void calcVanishPoint();
    void cutRoadRect();
    void calcRoadRect();
};

#endif	/* ROAD_H */

