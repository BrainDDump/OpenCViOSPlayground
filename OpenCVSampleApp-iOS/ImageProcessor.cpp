//
//  ImageProcessor.cpp
//  OpenCVSampleApp-iOS
//
//  Created by Кирилл on 4/1/16.
//  Copyright © 2016 BrainDump. All rights reserved.
//

#include "ImageProcessor.hpp"

/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/**
 * Helper function to display text in the center of a contour
 */
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 1;
    int thickness = 3;
    int baseline = 0;
    
    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);
    
    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), 2);
    cv::putText(im, label, pt, fontface, scale, CV_RGB(20,0,20), thickness, 8);
}

void get_contours(cv::Mat& image, std::vector<std::vector<cv::Point> >& contours)
{
    cv::Mat gray;
    cv::cvtColor(image, gray, CV_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(3, 3), 3, 3);
    
    cv::Mat bw;
    cv::Canny(gray, bw, 10, 30, 7);
    
    // Find contours
    cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
}

void draw_contours(cv::Mat& image, const std::vector<std::vector<cv::Point> >& contours)
{
    cv::Scalar color( rand()&255, rand()&255, rand()&255);
    cv::drawContours(image, contours, -1, color);
}


void detect_shapes(cv::Mat& image)
{
    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, CV_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(9, 9), 3, 3);
    
    // Use Canny instead of threshold to catch squares with gradient shading
    cv::Mat bw;
    cv::Canny(gray, bw, 0, 50, 5);
    
    // Find contours
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    
    std::vector<cv::Point> approx;
    for (int i = 0; i < contours.size(); i++)
    {
        // Approximate contour with accuracy proportional
        // to the contour perimeter
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
        
        // Skip small or non-convex objects
        if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
            continue;
        
        if (approx.size() == 3)
        {
            setLabel(image, "TRI", contours[i]);    // Triangles
        }
        else if (approx.size() >= 4 && approx.size() <= 6)
        {
            // Number of vertices of polygonal curve
            size_t vtc = approx.size();
            
            // Get the cosines of all corners
            std::vector<double> cos;
            for (int j = 2; j < vtc + 1; j++)
            {
                cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));
            }
            
            // Sort ascending the cosine values
            std::sort(cos.begin(), cos.end());
            
            // Get the lowest and the highest cosine
            double mincos = cos.front();
            double maxcos = cos.back();
            
            // Use the degrees obtained above and the number of vertices
            // to determine the shape of the contour
            if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
                setLabel(image, "RECT", contours[i]);
            else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
                setLabel(image, "PENTA", contours[i]);
            else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
                setLabel(image, "HEXA", contours[i]);
        }
        else
        {
            // Detect and label circles
            double area = cv::contourArea(contours[i]);
            cv::Rect r = cv::boundingRect(contours[i]);
            int radius = r.width / 2;
            
            if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
                std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
                setLabel(image, "CIR", contours[i]);
        }
    }
}

// My methods
void put_text_on_image(cv::Mat& image) {
    cv::putText(image, "Pidr", cv::Point(100, 300), CV_FONT_HERSHEY_PLAIN, 20.0, cv::Scalar(0, 0, 255));
}

void invert_image(cv::Mat& image) {
    cv::Mat image_copy;
    cv::cvtColor(image, image_copy, cv::COLOR_BGR2GRAY);
    
    cv::bitwise_not(image_copy, image_copy);
    
    cv::Mat bgr;
    cv::cvtColor(image_copy, bgr, cv::COLOR_GRAY2BGR);
    
    cv::cvtColor(bgr, image, cv::COLOR_BGR2BGRA);
}

void detect_circles(cv::Mat& image) {
    cv::Mat gray;
    
    cvtColor(image, gray, CV_BGR2GRAY);
    
    // smooth it, otherwise a lot of false circles may be detected
    cv::GaussianBlur(gray, gray, cv::Size(9, 9), 2, 2);
    
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, gray.rows/4, 200, 100);
    
    for(size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // draw the circle center
        cv::circle(image, center, 3, cv::Scalar(0,255,0), -1, 8, 0);
        // draw the circle outline
        cv::circle(image, center, radius, cv::Scalar(0,0,255), 3, 8, 0);
    }
}

ImageProcessor::ImageProcessor(Mat& img):
sourceImg(img) {
    /*---//---*/
}

void ImageProcessor::detectEdgesWithCanny(int lowThreshold, int flag = 0) {
    if (flag == 1) {
        //cvtColor(imgColorThresholded, imgGRAY);
        blur(imgColorThresholded, imgColorThresholded, Size(3,3));
        
        Canny(imgColorThresholded, detectedEdges, lowThreshold, lowThreshold * 3);
        return;
    }
    
    cvtColor(sourceImg, imgGRAY, CV_BGR2GRAY);
    blur(imgGRAY, imgGRAY, Size(3,3));
    
    Canny(imgGRAY, detectedEdges, lowThreshold, lowThreshold * 3);
}

void ImageProcessor::drawDetectedEdges() {
    Mat dst;
    dst = Scalar::all(0);
    sourceImg.copyTo(dst, detectedEdges);
    
    dst.copyTo(sourceImg);
}

void ImageProcessor::detectContours() {
    findContours(detectedEdges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
}


void ImageProcessor::drawDetectedContours() {
    for( int i = 0; i< contours.size(); i++ ) {
        Scalar color = Scalar(255, 25, 25);
        drawContours( sourceImg, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
}

void ImageProcessor::detectColorThresholdedImage(Scalar colorLow, Scalar colorHigh) {
    Mat imgHSV;
    cvtColor(sourceImg, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
    
    inRange(imgHSV, colorLow, colorHigh, imgColorThresholded); //Threshold the image
    
    //morphological opening (removes small objects from the foreground)
    erode(imgColorThresholded, imgColorThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate(imgColorThresholded, imgColorThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    //morphological closing (removes small holes from the foreground)
    dilate(imgColorThresholded, imgColorThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgColorThresholded, imgColorThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
}









