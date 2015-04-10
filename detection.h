/* 
 * File:   detection.h
 * Author: aleksey
 *
 * Created on April 10, 2015, 12:41 PM
 */

#ifndef DETECTION_H
#define	DETECTION_H

#include "opencv2/core/core.hpp"

using namespace cv;

int findColoredPerson(int *x,int *y,Mat* img);

#endif	/* DETECTION_H */

