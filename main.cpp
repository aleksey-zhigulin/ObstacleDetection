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
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/program_options.hpp>

#include "miscellaneous.h"
#include "detection.h"

using namespace cv;
using namespace std;
using namespace boost::program_options;

/*
 * 
 */
int main(int argc, char** argv) {
   
    options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("find-yellow-helmet", value<string>(), "finds out a yellow helmet")        
        ("find-road", value<string>(), "finds out a road")        
    ;
    positional_options_description p;
//    p.add("input-file", -1);

    variables_map vm;
    store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    notify(vm);
    
    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
    if (vm.count("find-yellow-helmet")) {        
        Mat img = imread(vm["find-yellow-helmet"].as<string>()); 
        findYellowHelmet(&img);
        namedWindow("output");
        imshow("output", img);
        waitKey(0);
    }
    if (vm.count("find-road")) {        
        Mat img = imread(vm["find-road"].as<string>());        
        findRoadLines(&img, 1);
        namedWindow("output");
        imshow("output", img);
        waitKey(0);
    }
    
    return 0;
}