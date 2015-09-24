// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_VIDEO_CAPTURE_MAC_PLATFORM_VIDEO_CAPTURING_MAC_H_
#define MEDIA_VIDEO_CAPTURE_MAC_PLATFORM_VIDEO_CAPTURING_MAC_H_

#import <Foundation/Foundation.h>

namespace media {
class VideoCaptureDeviceMac;
}

@protocol PlatformVideoCapturingMac <NSObject>

- (id)initWithFrameReceiver:(media::VideoCaptureDeviceMac*)frameReceiver;

- (void)setFrameReceiver:(media::VideoCaptureDeviceMac*)frameReceiver;

- (BOOL)setCaptureDevice:(NSString*)deviceId;

- (BOOL)setCaptureHeight:(int)height width:(int)width frameRate:(int)frameRate;

- (BOOL)startCapture;

- (void)stopCapture;

@end

#endif  
