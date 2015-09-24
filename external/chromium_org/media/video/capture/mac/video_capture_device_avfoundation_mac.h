// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_VIDEO_CAPTURE_MAC_VIDEO_CAPTURE_DEVICE_AVFOUNDATION_MAC_H_
#define MEDIA_VIDEO_CAPTURE_MAC_VIDEO_CAPTURE_DEVICE_AVFOUNDATION_MAC_H_

#import <Foundation/Foundation.h>

#import "base/mac/scoped_nsobject.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#import "media/video/capture/mac/avfoundation_glue.h"
#import "media/video/capture/mac/platform_video_capturing_mac.h"

namespace media {
class VideoCaptureDeviceMac;
}

@class CrAVCaptureDevice;
@class CrAVCaptureSession;
@class CrAVCaptureVideoDataOutput;

@interface VideoCaptureDeviceAVFoundation
    : NSObject<CrAVCaptureVideoDataOutputSampleBufferDelegate,
               PlatformVideoCapturingMac> {
 @private
  
  int frameWidth_;
  int frameHeight_;
  int frameRate_;

  base::Lock lock_;  
  media::VideoCaptureDeviceMac* frameReceiver_;  

  base::scoped_nsobject<CrAVCaptureSession> captureSession_;

  
  
  CrAVCaptureDevice* captureDevice_;
  
  CrAVCaptureDeviceInput* captureDeviceInput_;
  base::scoped_nsobject<CrAVCaptureVideoDataOutput> captureVideoDataOutput_;

  base::ThreadChecker thread_checker_;
}

+ (NSDictionary*)deviceNames;

- (id)initWithFrameReceiver:(media::VideoCaptureDeviceMac*)frameReceiver;

- (void)setFrameReceiver:(media::VideoCaptureDeviceMac*)frameReceiver;

- (BOOL)setCaptureDevice:(NSString*)deviceId;

- (BOOL)setCaptureHeight:(int)height width:(int)width frameRate:(int)frameRate;

- (BOOL)startCapture;

- (void)stopCapture;

@end

#endif  
