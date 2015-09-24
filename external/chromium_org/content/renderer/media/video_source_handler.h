// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_VIDEO_SOURCE_HANDLER_H_
#define CONTENT_RENDERER_MEDIA_VIDEO_SOURCE_HANDLER_H_

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "third_party/libjingle/source/talk/app/webrtc/videosourceinterface.h"

namespace cricket {
class VideoFrame;
}

namespace content {

class MediaStreamDependencyFactory;
class MediaStreamRegistryInterface;

class CONTENT_EXPORT FrameReaderInterface {
 public:
  
  
  
  virtual bool GotFrame(cricket::VideoFrame* frame) = 0;

 protected:
  virtual ~FrameReaderInterface() {}
};

class CONTENT_EXPORT VideoSourceHandler {
 public:
  
  
  explicit VideoSourceHandler(MediaStreamRegistryInterface* registry);
  virtual ~VideoSourceHandler();
  
  
  
  bool Open(const std::string& url, FrameReaderInterface* reader);
  
  
  
  
  bool Close(const std::string& url, FrameReaderInterface* reader);

  
  
  cricket::VideoRenderer* GetReceiver(FrameReaderInterface* reader);

 private:
  scoped_refptr<webrtc::VideoSourceInterface> GetFirstVideoSource(
      const std::string& url);

  MediaStreamRegistryInterface* registry_;
  std::map<FrameReaderInterface*, cricket::VideoRenderer*> reader_to_receiver_;

  DISALLOW_COPY_AND_ASSIGN(VideoSourceHandler);
};

}  

#endif  

