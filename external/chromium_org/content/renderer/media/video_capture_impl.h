// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.




#ifndef CONTENT_RENDERER_MEDIA_VIDEO_CAPTURE_IMPL_H_
#define CONTENT_RENDERER_MEDIA_VIDEO_CAPTURE_IMPL_H_

#include <list>
#include <map>

#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "content/common/media/video_capture.h"
#include "content/renderer/media/video_capture_message_filter.h"
#include "media/video/capture/video_capture.h"
#include "media/video/capture/video_capture_types.h"

namespace base {
class MessageLoopProxy;
}

namespace content {

class CONTENT_EXPORT VideoCaptureImpl
    : public media::VideoCapture, public VideoCaptureMessageFilter::Delegate {
 public:
  
  virtual void StartCapture(
      media::VideoCapture::EventHandler* handler,
      const media::VideoCaptureParams& params) OVERRIDE;
  virtual void StopCapture(media::VideoCapture::EventHandler* handler) OVERRIDE;
  virtual bool CaptureStarted() OVERRIDE;
  virtual int CaptureFrameRate() OVERRIDE;

  
  virtual void OnBufferCreated(base::SharedMemoryHandle handle,
                               int length,
                               int buffer_id) OVERRIDE;
  virtual void OnBufferDestroyed(int buffer_id) OVERRIDE;
  virtual void OnBufferReceived(
      int buffer_id,
      base::Time timestamp,
      const media::VideoCaptureFormat& format) OVERRIDE;
  virtual void OnStateChanged(VideoCaptureState state) OVERRIDE;
  virtual void OnDelegateAdded(int32 device_id) OVERRIDE;

  
  virtual void SuspendCapture(bool suspend);

 private:
  friend class VideoCaptureImplManager;
  friend class VideoCaptureImplTest;
  friend class MockVideoCaptureImpl;

  class ClientBuffer;
  typedef std::map<media::VideoCapture::EventHandler*,
                   media::VideoCaptureParams> ClientInfo;

  VideoCaptureImpl(media::VideoCaptureSessionId session_id,
                   base::MessageLoopProxy* capture_message_loop_proxy,
                   VideoCaptureMessageFilter* filter);
  virtual ~VideoCaptureImpl();

  void DoStartCaptureOnCaptureThread(
      media::VideoCapture::EventHandler* handler,
      const media::VideoCaptureParams& params);
  void DoStopCaptureOnCaptureThread(media::VideoCapture::EventHandler* handler);
  void DoBufferCreatedOnCaptureThread(base::SharedMemoryHandle handle,
                                      int length,
                                      int buffer_id);
  void DoBufferDestroyedOnCaptureThread(int buffer_id);
  void DoBufferReceivedOnCaptureThread(
      int buffer_id,
      base::Time timestamp,
      const media::VideoCaptureFormat& format);
  void DoClientBufferFinishedOnCaptureThread(
      int buffer_id,
      const scoped_refptr<ClientBuffer>& buffer);
  void DoStateChangedOnCaptureThread(VideoCaptureState state);
  void DoDelegateAddedOnCaptureThread(int32 device_id);

  void DoSuspendCaptureOnCaptureThread(bool suspend);

  void Init();
  void DeInit(base::Closure task);
  void DoDeInitOnCaptureThread(base::Closure task);
  void StopDevice();
  void RestartCapture();
  void StartCaptureInternal();
  void AddDelegateOnIOThread();
  void RemoveDelegateOnIOThread(base::Closure task);
  virtual void Send(IPC::Message* message);

  
  bool RemoveClient(media::VideoCapture::EventHandler* handler,
                    ClientInfo* clients);

  const scoped_refptr<VideoCaptureMessageFilter> message_filter_;
  const scoped_refptr<base::MessageLoopProxy> capture_message_loop_proxy_;
  const scoped_refptr<base::MessageLoopProxy> io_message_loop_proxy_;
  int device_id_;
  const int session_id_;

  
  typedef std::map<int32, scoped_refptr<ClientBuffer> > ClientBufferMap;
  ClientBufferMap client_buffers_;

  ClientInfo clients_;
  ClientInfo clients_pending_on_filter_;
  ClientInfo clients_pending_on_restart_;

  
  
  media::VideoCaptureParams params_;

  
  media::VideoCaptureFormat last_frame_format_;

  bool suspended_;
  VideoCaptureState state_;

  
  
  
  base::WeakPtrFactory<VideoCaptureImpl> weak_this_factory_;

  DISALLOW_COPY_AND_ASSIGN(VideoCaptureImpl);
};

}  

#endif  
