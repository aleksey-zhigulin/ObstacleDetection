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
    double k, b;
    TLine();        
    TLine(int, int, int, int);
    TLine(Point2i, Point2i);    
    void show(Mat*);
    double getAngle();
    double getLength();
    void extend(int from_x, int to_x);
    int y(int);
    int x(int);
    virtual ~TLine();    
    static bool angleCmp(TLine*, TLine*);
private:
    double slope();
};

#endif	/* TLINE_H */

