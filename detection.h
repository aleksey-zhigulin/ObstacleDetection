/* 
 * File:   detection.h
 * Author: aleksey
 *
 * Created on April 10, 2015, 12:41 PM
 */

#ifndef DETECTION_H
#define	DETECTION_H

#include "opencv2/core/core.hpp"
#include "TLine.h"

using namespace cv;

void LinesFromMat(Mat *image, vector<TLine*> *lines);
int findColoredObject(int *x,int *y,Mat* img);

#endif	/* DETECTION_H */

