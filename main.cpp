/* 
 * File:   main.cpp
 * Author: Aleksey
 *
 * Created on March 26, 2015, 2:31 PM
 */

#include <cstdlib>
#include <cstring>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "miscellaneous.h"

using namespace cv;
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    for(int i=1; i<argc; ++i) {
        Mat img = imread(argv[i]); 
        findYellowHelmet(&img);
        imwrite("result.jpg", img);   
    }
    
    return 0;
}