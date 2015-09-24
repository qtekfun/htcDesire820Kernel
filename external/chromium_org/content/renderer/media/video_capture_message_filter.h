// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_VIDEO_CAPTURE_MESSAGE_FILTER_H_
#define CONTENT_RENDERER_MEDIA_VIDEO_CAPTURE_MESSAGE_FILTER_H_

#include <map>

#include "base/memory/shared_memory.h"
#include "content/common/content_export.h"
#include "content/common/media/video_capture.h"
#include "ipc/ipc_channel_proxy.h"
#include "media/video/capture/video_capture.h"

namespace content {

class CONTENT_EXPORT VideoCaptureMessageFilter
    : public IPC::ChannelProxy::MessageFilter {
 public:
  class CONTENT_EXPORT Delegate {
   public:
    
    virtual void OnBufferCreated(base::SharedMemoryHandle handle,
                                 int length,
                                 int buffer_id) = 0;

    virtual void OnBufferDestroyed(int buffer_id) = 0;

    
    virtual void OnBufferReceived(int buffer_id,
                                  base::Time timestamp,
                                  const media::VideoCaptureFormat& format) = 0;

    
    
    virtual void OnStateChanged(VideoCaptureState state) = 0;

    
    
    virtual void OnDelegateAdded(int32 device_id) = 0;

   protected:
    virtual ~Delegate() {}
  };

  VideoCaptureMessageFilter();

  
  void AddDelegate(Delegate* delegate);

  
  void RemoveDelegate(Delegate* delegate);

  
  virtual bool Send(IPC::Message* message);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnFilterAdded(IPC::Channel* channel) OVERRIDE;
  virtual void OnFilterRemoved() OVERRIDE;
  virtual void OnChannelClosing() OVERRIDE;

 protected:
  virtual ~VideoCaptureMessageFilter();

 private:
  typedef std::map<int32, Delegate*> Delegates;

  
  void OnBufferCreated(int device_id,
                       base::SharedMemoryHandle handle,
                       int length,
                       int buffer_id);

  
  void OnBufferDestroyed(int device_id,
                         int buffer_id);

  
  void OnBufferReceived(int device_id,
                        int buffer_id,
                        base::Time timestamp,
                        const media::VideoCaptureFormat& format);

  
  void OnDeviceStateChanged(int device_id, VideoCaptureState state);

  
  Delegate* find_delegate(int device_id) const;

  
  Delegates delegates_;
  Delegates pending_delegates_;
  int32 last_device_id_;

  IPC::Channel* channel_;

  DISALLOW_COPY_AND_ASSIGN(VideoCaptureMessageFilter);
};

}  

#endif  
