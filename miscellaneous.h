/* 
 * File:   miscellaneous.h
 * Author: Aleksey
 *
 * Created on April 3, 2015, 1:02 PM
 */

#ifndef MISCELLANEOUS_H
#define	MISCELLANEOUS_H

#include "opencv2/core/core.hpp"

using namespace cv;

void findYellowHelmet(Mat* img);
void makeStereoCube(int, char**);
void drawCubeRight();
void drawCubeLeft();
void processKeyboard(unsigned char key, int x, int y);

#endif	/* MISCELLANEOUS_H */

