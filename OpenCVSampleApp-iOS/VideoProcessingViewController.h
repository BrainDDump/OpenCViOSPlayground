//
//  VideoProcessingViewController.h
//  OpenCVSampleApp-iOS
//
//  Created by Кирилл on 3/31/16.
//  Copyright © 2016 BrainDump. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import <Accelerate/Accelerate.h>
#import <CoreGraphics/CoreGraphics.h>
#import <CoreImage/CoreImage.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreVideo/CoreVideo.h>
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>

#import <opencv2/opencv.hpp>
#import <opencv2/videoio/cap_ios.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>

@interface VideoProcessingViewController: UIViewController<CvVideoCameraDelegate>

@property (weak, nonatomic) IBOutlet UISlider *cannyLowerThreshholdSlider;
@property (weak, nonatomic) IBOutlet UIButton *startButton;

@property (weak, nonatomic) IBOutlet UIImageView *imageView;

@property (weak, nonatomic) IBOutlet UILabel *detectionLabel;

@property (nonatomic, retain) CvVideoCamera* videoCamera;

- (IBAction)startButtonPressed;

@end
