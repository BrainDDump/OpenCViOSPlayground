//
//  CSimpleTargetDetector.cpp
//  OpenCVSampleApp-iOS
//
//  Created by Кирилл on 4/2/16.
//  Copyright © 2016 BrainDump. All rights reserved.
//

#include "CSimpleTargetDetector.hpp"

#define MY_DEBUG(on, debugCommand) if (on) debugCommand

CSimpleTargetDetector::CSimpleTargetDetector(bool debug):
EMPTY(Vec3f(-1, -1, -1)), debugOn(debug) {
    for (int i = 0; i < 10; ++i) {
        lastTenCirclesDetected.push_back(EMPTY);
    }
}

void CSimpleTargetDetector::processFrame(cv::Mat& srcImg) {
    sourceImg = srcImg;
}

void CSimpleTargetDetector::detectRedCircles() {
    
    Mat bgr_image;
    sourceImg.copyTo(bgr_image);
    medianBlur(bgr_image, bgr_image, 3);
    
    //cv::medianBlur(bgr_image, bgr_image, 3);
    // Convert input image to HSV
    Mat hsv_image;
    cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);
    
    // Threshold the HSV image, keep only the red pixels
    Mat lower_red_hue_range;
    Mat upper_red_hue_range;
    inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
    inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);
    
    // Combine the above two images
    Mat red_hue_image;
    addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

    GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);
    
    // Use the Hough transform to detect circles in the combined threshold image
    std::vector<cv::Vec3f> circles;
    HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows/8, 100, 20, 0, 0);
    
    // Loop over all detected circles and outline them on the original image
    if (circles.size() > 1) {
        MY_DEBUG(debugOn, std::cout << "*** too many circles detected, skipping" << std::endl;)
        
        lastTenCirclesDetected.push_back(EMPTY);
    } else if (circles.size() == 1)  {
        MY_DEBUG(debugOn, std::cout << "*** circle detected & pushed" << std::endl;)
        
        circle(sourceImg, Point(circles.front()[0], circles.front()[1]), circles.front()[2], Scalar(50, 100, 100));
        
        lastTenCirclesDetected.push_back(circles.front());
    } else {
        MY_DEBUG(debugOn, std::cout << "*** no circles detected" << std::endl;)
        
        lastTenCirclesDetected.push_back(EMPTY);
    }
    
    if (lastTenCirclesDetected.size() > 10) {
        lastTenCirclesDetected.pop_front();
    }
}

std::string CSimpleTargetDetector::detectedTarget() {
    const int imgCenterX = sourceImg.size().width / 2;
    const int imgCenterY = sourceImg.size().height / 2;
    const cv::Point imgCenter = cv::Point(imgCenterX, imgCenterY);
    
    int emptyCount = 0;
    
    int minArea = 1000000;
    int maxArea = 0;
    
    int centerXCoordinateSum = 0;
    int centerYCoordinateSum = 0;
    for (auto it = lastTenCirclesDetected.begin(); it != lastTenCirclesDetected.end(); ++it) {
        if (*it == EMPTY) {
            emptyCount += 1;
            continue;
        }
        
        centerXCoordinateSum += (*it)[0];
        centerYCoordinateSum += (*it)[1];
        
        int radius = (*it)[2];
        int boundingRectArea = M_1_PI * radius * radius;
        
        if (boundingRectArea < minArea) {
            minArea = boundingRectArea;
        }
        if (boundingRectArea > maxArea) {
            maxArea = boundingRectArea;
        }
    }
    
    const int nonEmptyCount = 10 - emptyCount;
    const cv::Point point = cv::Point(centerXCoordinateSum / nonEmptyCount, centerYCoordinateSum / nonEmptyCount);
    const int avarageDist = cv::norm(point - imgCenter);
    
    MY_DEBUG(debugOn, std::cout
             << "emptyCount: " << emptyCount << std::endl
             << "minArea: "    << minArea << std::endl
             << "maxArea: "    << maxArea << std::endl;)
    
    if ((0 < maxArea - minArea) &&
        (maxArea - minArea < 200) &&
        (maxArea > 10) &&
        (emptyCount <= 4) &&
        (avarageDist <= 30)) {
        return "CIRCLE";
    }
    
    return "EMPTY";
}

void CSimpleTargetDetector::highlightFrame() {
    int height = sourceImg.size().height;
    int width  = sourceImg.size().width;
    
    rectangle(sourceImg, cv::Point(0, 0), cv::Point(height, width), Scalar(255,0,0), 2);
}

#undef MY_DEBUG










