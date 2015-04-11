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

int findColoredObject(int *x,int *y,Mat* img);
void LinesFromMat(Mat *image, vector<TLine*> *lines);
int findRoadLines(Mat* img, int show);
Point2i calcVanishPoint(TLine&, TLine&);
double slope(TLine &);
void extendLine(TLine& line, int from_x, int to_x);
#endif	/* DETECTION_H */

