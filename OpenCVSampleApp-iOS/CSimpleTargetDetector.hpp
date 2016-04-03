//
//  CSimpleTargetDetector.hpp
//  OpenCVSampleApp-iOS
//
//  Created by Кирилл on 4/2/16.
//  Copyright © 2016 BrainDump. All rights reserved.
//

#ifndef CSimpleTargetDetector_hpp
#define CSimpleTargetDetector_hpp

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <queue>
#include <list>
#include <math.h>

using namespace cv;

class CSimpleTargetDetector {
    const bool debugOn;
    const cv::Vec3f EMPTY;
    
    std::list<cv::Vec3f> lastTenCirclesDetected;
    
    Mat sourceImg;
    
public:
    CSimpleTargetDetector(bool debug);
    
    void processFrame(Mat& srcImg);
    void detectRedCircles();
    void highlightFrame();
    
    std::string detectedTarget();
};

#endif /* CSimpleTargetDetector_hpp */
