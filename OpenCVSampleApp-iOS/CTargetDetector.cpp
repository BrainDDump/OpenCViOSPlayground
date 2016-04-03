//
//  CTargetDetector.cpp
//  OpenCVSampleApp-iOS
//
//  Created by Кирилл on 4/2/16.
//  Copyright © 2016 BrainDump. All rights reserved.
//

#include "CTargetDetector.hpp"

CTargetDetector::CTargetDetector() {
    /*---//---*/
}

void CTargetDetector::processFrame(Mat &srcImg) {
    sourceImg = srcImg;
    
    sourceImg.copyTo(bufferImg);
    
    
    
    updateSrcImgWithBufferImg();
}

void CTargetDetector::updateSrcImgWithBufferImg() {
    bufferImg.copyTo(sourceImg);
}

std::string CTargetDetector::getShape() {
    return EMPTY;
}

// Private methods
void CTargetDetector::applyBlurToBufferImg() {
    blur(bufferImg, bufferImg, Size(3,3));
}

void CTargetDetector::filterByColors(std::vector<Scalar> colorsLow, std::vector<Scalar> colorsHigh) {
    Mat hsvImage;
    cvtColor(bufferImg, hsvImage, COLOR_BGR2HSV);
    
    Mat colorsFilteredImage;
    Mat buffer;
    for (int i = 0; i < colorsLow.size(); ++i) {
        inRange(sourceImg, colorsLow[i], colorsHigh[i], buffer);
        addWeighted(buffer, 1.0, colorsFilteredImage, 1.0, 0.0, colorsFilteredImage);
    }
    
    colorsFilteredImage.copyTo(bufferImg);
}

void CTargetDetector::detectEdgesWithCanny(int lowThreshold) {
    applyBlurToBufferImg();
    
    Canny(bufferImg, edges, lowThreshold, lowThreshold * 3);
}

void CTargetDetector::detectContours() {
    
}

void CTargetDetector::detectAndDrawShapes() {
    
}

















