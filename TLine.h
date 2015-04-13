/* 
 * File:   TLine.h
 * Author: aleksey
 *
 * Created on April 10, 2015, 7:29 PM
 */

#include "opencv2/core/core.hpp"

#ifndef TLINE_H
#define	TLINE_H

using namespace cv;

class TLine {
public:
    Point2i pt1, pt2;
    TLine();        
    TLine(int, int, int, int);
    TLine(Point2i, Point2i);    
    void show(Mat*);
    double get_angle();
    double get_length();
    double slope();
    void extend(int from_x, int to_x);
    virtual ~TLine();    
    static bool angle_cmp(TLine*, TLine*);
private:
};

#endif	/* TLINE_H */

