// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_VIDEO_DESTINATION_HANDLER_H_
#define CONTENT_RENDERER_MEDIA_VIDEO_DESTINATION_HANDLER_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "third_party/libjingle/source/talk/media/base/videocapturer.h"

namespace content {

class MediaStreamDependencyFactory;
class MediaStreamRegistryInterface;
class PPB_ImageData_Impl;

class CONTENT_EXPORT FrameWriterInterface {
 public:
  
  
  virtual void PutFrame(PPB_ImageData_Impl* image_data,
                        int64 time_stamp_ns) = 0;
  virtual ~FrameWriterInterface() {}
};

class CONTENT_EXPORT PpFrameWriter
    : public NON_EXPORTED_BASE(cricket::VideoCapturer),
      public FrameWriterInterface {
 public:
  PpFrameWriter();
  virtual ~PpFrameWriter();

  
  
  virtual cricket::CaptureState Start(
      const cricket::VideoFormat& capture_format) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual bool IsRunning() OVERRIDE;
  virtual bool GetPreferredFourccs(std::vector<uint32>* fourccs) OVERRIDE;
  virtual bool GetBestCaptureFormat(const cricket::VideoFormat& desired,
                                    cricket::VideoFormat* best_format) OVERRIDE;
  virtual bool IsScreencast() const OVERRIDE;

  
  
  virtual void PutFrame(PPB_ImageData_Impl* image_data,
                        int64 time_stamp_ns) OVERRIDE;

 private:
  bool started_;
  
  
  base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(PpFrameWriter);
};

class CONTENT_EXPORT VideoDestinationHandler {
 public:
  
  
  
  
  
  
  
  
  
  static bool Open(MediaStreamDependencyFactory* factory,
                   MediaStreamRegistryInterface* registry,
                   const std::string& url,
                   FrameWriterInterface** frame_writer);

 private:
  DISALLOW_COPY_AND_ASSIGN(VideoDestinationHandler);
};

}  

#endif  

