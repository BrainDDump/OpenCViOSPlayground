//
//  VideoProcessingViewController.m
//  OpenCVSampleApp-iOS
//
//  Created by Кирилл on 3/31/16.
//  Copyright © 2016 BrainDump. All rights reserved.
//

#import "VideoProcessingViewController.h"

#include "ImageProcessor.hpp"
#include "CSimpleTargetDetector.hpp"

CSimpleTargetDetector targetDetector(true);

@implementation VideoProcessingViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _videoCamera = [[CvVideoCamera alloc] initWithParentView: _imageView];
    _videoCamera.delegate = self;
    _videoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionBack;
    _videoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationPortrait;
    _videoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPreset640x480;
    _videoCamera.defaultFPS = 30;
    _videoCamera.grayscaleMode = NO;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#ifdef __cplusplus
- (void) processImage:(cv::Mat&)image {
    cv::Mat subImage = image(cv::Rect(200, 200, 200, 200));
    
    targetDetector.processFrame(subImage);
    targetDetector.detectRedCircles();
    std::string detectedTarget = targetDetector.detectedTarget();
    targetDetector.highlightFrame();
    
    std::cout << "detectedTarget: " << detectedTarget << std::endl;
    dispatch_async(dispatch_get_main_queue(), ^{
        _detectionLabel.text = [[NSString alloc] initWithCString: detectedTarget.c_str()
                                                        encoding: [NSString defaultCStringEncoding]];
    });
}
#endif

- (IBAction)startButtonPressed {
    [_videoCamera start];
}

@end
