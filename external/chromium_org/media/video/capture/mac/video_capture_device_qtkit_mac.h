// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_VIDEO_CAPTURE_MAC_VIDEO_CAPTURE_DEVICE_QTKIT_MAC_H_
#define MEDIA_VIDEO_CAPTURE_MAC_VIDEO_CAPTURE_DEVICE_QTKIT_MAC_H_

#import <Foundation/Foundation.h>

#include <vector>

#import "media/video/capture/mac/platform_video_capturing_mac.h"

namespace media {
class VideoCaptureDeviceMac;
}

@class QTCaptureDeviceInput;
@class QTCaptureSession;

@interface VideoCaptureDeviceQTKit : NSObject<PlatformVideoCapturingMac> {
 @private
  
  int frameRate_;

  NSLock *lock_;
  media::VideoCaptureDeviceMac *frameReceiver_;

  
  QTCaptureSession *captureSession_;
  QTCaptureDeviceInput *captureDeviceInput_;

  
  
  
  std::vector<UInt8> adjustedFrame_;
}

+ (NSDictionary*)deviceNames;

- (id)initWithFrameReceiver:(media::VideoCaptureDeviceMac*)frameReceiver;

- (void)setFrameReceiver:(media::VideoCaptureDeviceMac*)frameReceiver;

- (BOOL)setCaptureDevice:(NSString*)deviceId;

- (BOOL)setCaptureHeight:(int)height width:(int)width frameRate:(int)frameRate;

- (BOOL)startCapture;

- (void)stopCapture;

- (void)handleNotification:(NSNotification*)errorNotification;

@end

#endif  
