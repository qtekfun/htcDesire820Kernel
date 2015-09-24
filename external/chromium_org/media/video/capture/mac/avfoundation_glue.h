// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_VIDEO_CAPTURE_MAC_AVFOUNDATION_GLUE_H_
#define MEDIA_VIDEO_CAPTURE_MAC_AVFOUNDATION_GLUE_H_

#import <Foundation/Foundation.h>

#include "base/basictypes.h"
#include "media/base/media_export.h"
#import "media/video/capture/mac/coremedia_glue.h"

class MEDIA_EXPORT AVFoundationGlue {
 public:
  
  
  static bool IsAVFoundationSupported();

  static NSBundle const* AVFoundationBundle();

  static void* AVFoundationLibraryHandle();

  
  static NSString* AVCaptureDeviceWasConnectedNotification();
  static NSString* AVCaptureDeviceWasDisconnectedNotification();

  
  static NSString* AVMediaTypeVideo();
  static NSString* AVMediaTypeAudio();
  static NSString* AVMediaTypeMuxed();

  
  static NSString* AVCaptureSessionRuntimeErrorNotification();
  static NSString* AVCaptureSessionDidStopRunningNotification();
  static NSString* AVCaptureSessionErrorKey();
  static NSString* AVCaptureSessionPreset320x240();
  static NSString* AVCaptureSessionPreset640x480();
  static NSString* AVCaptureSessionPreset1280x720();

  
  static NSString* AVVideoScalingModeKey();
  static NSString* AVVideoScalingModeResizeAspect();

  static Class AVCaptureSessionClass();
  static Class AVCaptureVideoDataOutputClass();

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(AVFoundationGlue);
};

MEDIA_EXPORT
@interface CrAVCaptureDevice : NSObject

- (BOOL)hasMediaType:(NSString*)mediaType;
- (NSString*)uniqueID;
- (NSString*)localizedName;
- (BOOL)supportsAVCaptureSessionPreset:(NSString*)preset;

@end

MEDIA_EXPORT
@interface CrAVCaptureInput  
@end

MEDIA_EXPORT
@interface CrAVCaptureOutput  
@end

MEDIA_EXPORT
@interface CrAVCaptureSession : NSObject

- (void)release;
- (BOOL)canSetSessionPreset:(NSString*)preset;
- (void)setSessionPreset:(NSString*)preset;
- (NSString*)sessionPreset;
- (void)addInput:(CrAVCaptureInput*)input;
- (void)removeInput:(CrAVCaptureInput*)input;
- (void)addOutput:(CrAVCaptureOutput*)output;
- (void)removeOutput:(CrAVCaptureOutput*)output;
- (BOOL)isRunning;
- (void)startRunning;
- (void)stopRunning;

@end

MEDIA_EXPORT
@interface CrAVCaptureConnection : NSObject

- (BOOL)isVideoMinFrameDurationSupported;
- (void)setVideoMinFrameDuration:(CoreMediaGlue::CMTime)minFrameRate;
- (BOOL)isVideoMaxFrameDurationSupported;
- (void)setVideoMaxFrameDuration:(CoreMediaGlue::CMTime)maxFrameRate;

@end

MEDIA_EXPORT
@interface CrAVCaptureDeviceInput : CrAVCaptureInput

@end

@protocol CrAVCaptureVideoDataOutputSampleBufferDelegate <NSObject>

@optional

- (void)captureOutput:(CrAVCaptureOutput*)captureOutput
didOutputSampleBuffer:(CoreMediaGlue::CMSampleBufferRef)sampleBuffer
       fromConnection:(CrAVCaptureConnection*)connection;

@end

MEDIA_EXPORT
@interface CrAVCaptureVideoDataOutput : CrAVCaptureOutput

- (oneway void)release;
- (void)setSampleBufferDelegate:(id)sampleBufferDelegate
                          queue:(dispatch_queue_t)sampleBufferCallbackQueue;

- (void)setVideoSettings:(NSDictionary*)videoSettings;
- (NSDictionary*)videoSettings;
- (CrAVCaptureConnection*)connectionWithMediaType:(NSString*)mediaType;

@end

MEDIA_EXPORT
@interface AVCaptureDeviceGlue : NSObject

+ (NSArray*)devices;

+ (CrAVCaptureDevice*)deviceWithUniqueID:(NSString*)deviceUniqueID;

@end

MEDIA_EXPORT
@interface AVCaptureDeviceInputGlue : NSObject

+ (CrAVCaptureDeviceInput*)deviceInputWithDevice:(CrAVCaptureDevice*)device
                                           error:(NSError**)outError;

@end

#endif  
