//
//  ImageProcessor.hpp
//  OpenCVSampleApp-iOS
//
//  Created by Кирилл on 4/1/16.
//  Copyright © 2016 BrainDump. All rights reserved.
//

#ifndef ImageProcessor_hpp
#define ImageProcessor_hpp

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>

using namespace cv;

class ImageProcessor {
    Mat& sourceImg;
    
    // edge detection
    Mat imgGRAY;
    Mat detectedEdges;
    
    // contour detection
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    
    // color detection
    Mat imgColorThresholded;
public:
    ImageProcessor(Mat& img);
    
    void detectEdgesWithCanny(int lowThreshold, int flag);
    void drawDetectedEdges();
    
    void detectContours();
    void drawDetectedContours();
    
    void detectColorThresholdedImage(Scalar colorLow, Scalar colorHigh);
    void detectColorThresholdedContours();
};

#endif /* ImageProcessor_hpp */
