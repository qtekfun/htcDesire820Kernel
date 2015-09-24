// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_RENDERER_MEDIA_VIDEO_CAPTURE_IMPL_MANAGER_H_
#define CONTENT_RENDERER_MEDIA_VIDEO_CAPTURE_IMPL_MANAGER_H_

#include <list>
#include <map>

#include "base/message_loop/message_loop_proxy.h"
#include "base/threading/thread.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "media/video/capture/video_capture.h"

namespace content {

class VideoCaptureImpl;
class VideoCaptureMessageFilter;

class CONTENT_EXPORT VideoCaptureImplManager
    : public base::RefCountedThreadSafe<VideoCaptureImplManager> {
 public:
  VideoCaptureImplManager();

  
  
  
  
  virtual media::VideoCapture* AddDevice(
      media::VideoCaptureSessionId id,
      media::VideoCapture::EventHandler* handler);

  
  
  virtual void RemoveDevice(media::VideoCaptureSessionId id,
                            media::VideoCapture::EventHandler* handler);

  
  
  virtual void SuspendDevices(bool suspend);

  VideoCaptureMessageFilter* video_capture_message_filter() const {
    return filter_.get();
  }

 protected:
  virtual ~VideoCaptureImplManager();

 private:
  friend class base::RefCountedThreadSafe<VideoCaptureImplManager>;

  struct Device {
    Device(VideoCaptureImpl* device,
           media::VideoCapture::EventHandler* handler);
    ~Device();

    VideoCaptureImpl* vc;
    std::list<media::VideoCapture::EventHandler*> clients;
  };

  void FreeDevice(VideoCaptureImpl* vc);

  typedef std::map<media::VideoCaptureSessionId, Device*> Devices;
  Devices devices_;
  base::Lock lock_;
  scoped_refptr<VideoCaptureMessageFilter> filter_;
  base::Thread thread_;
  scoped_refptr<base::MessageLoopProxy> message_loop_proxy_;

  DISALLOW_COPY_AND_ASSIGN(VideoCaptureImplManager);
};

}  

#endif  
