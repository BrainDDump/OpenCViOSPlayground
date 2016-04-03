//
//  CTargetDetector.hpp
//  OpenCVSampleApp-iOS
//
//  Created by Кирилл on 4/2/16.
//  Copyright © 2016 BrainDump. All rights reserved.
//

#ifndef CTargetDetector_hpp
#define CTargetDetector_hpp

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>

using namespace cv;

class CTargetDetector {
    const std::string EMPTY  = "EMPTY";
    const std::string SQUARE = "SQUARE";
    const std::string CIRCLE = "CIRCLE";
    
    std::queue<std::string> lastTenTargetsDetected;
    
    Mat sourceImg;
    
    // filtered image by color
    Mat bufferImg;
    
    // detected edges with Canny
    Mat edges;
    
    // detected contours
    std::vector<std::vector<cv::Point> > contours;
    
    // Final image
    Mat resultingImg;
public:
    CTargetDetector();
    
    void processFrame(Mat& srcImg);
    void updateSrcImgWithBufferImg();
    
    std::string getShape();
    
private:
    void applyBlurToBufferImg();
    void filterByColors(std::vector<Scalar> colorsLow, std::vector<Scalar> colorsHigh);
    void detectEdgesWithCanny(int lowThreshold);
    void detectContours();
    void detectAndDrawShapes();
};


#endif /* CTargetDetector_hpp */
