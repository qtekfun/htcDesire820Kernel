// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_HOST_H_

#include <map>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "content/browser/renderer_host/media/video_capture_controller.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_message_filter.h"
#include "ipc/ipc_message.h"

namespace content {
class MediaStreamManager;

class CONTENT_EXPORT VideoCaptureHost
    : public BrowserMessageFilter,
      public VideoCaptureControllerEventHandler {
 public:
  explicit VideoCaptureHost(MediaStreamManager* media_stream_manager);

  
  virtual void OnChannelClosing() OVERRIDE;
  virtual void OnDestruct() const OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

  
  virtual void OnError(const VideoCaptureControllerID& id) OVERRIDE;
  virtual void OnBufferCreated(const VideoCaptureControllerID& id,
                               base::SharedMemoryHandle handle,
                               int length,
                               int buffer_id) OVERRIDE;
  virtual void OnBufferDestroyed(const VideoCaptureControllerID& id,
                                 int buffer_id) OVERRIDE;
  virtual void OnBufferReady(
      const VideoCaptureControllerID& id,
      int buffer_id,
      base::Time timestamp,
      const media::VideoCaptureFormat& format) OVERRIDE;
  virtual void OnEnded(const VideoCaptureControllerID& id) OVERRIDE;

 private:
  friend class BrowserThread;
  friend class base::DeleteHelper<VideoCaptureHost>;
  friend class MockVideoCaptureHost;
  friend class VideoCaptureHostTest;

  virtual ~VideoCaptureHost();

  
  
  
  
  void OnStartCapture(int device_id,
                      media::VideoCaptureSessionId session_id,
                      const media::VideoCaptureParams& params);
  void OnControllerAdded(
      int device_id,
      const base::WeakPtr<VideoCaptureController>& controller);
  void DoControllerAddedOnIOThread(
      int device_id,
      const base::WeakPtr<VideoCaptureController>& controller);

  
  void OnStopCapture(int device_id);

  
  void OnPauseCapture(int device_id);

  
  
  void OnReceiveEmptyBuffer(int device_id, int buffer_id);

  
  void DoSendNewBufferOnIOThread(
      const VideoCaptureControllerID& controller_id,
      base::SharedMemoryHandle handle,
      int length,
      int buffer_id);

  void DoSendFreeBufferOnIOThread(
      const VideoCaptureControllerID& controller_id,
      int buffer_id);

  
  void DoSendFilledBufferOnIOThread(
      const VideoCaptureControllerID& controller_id,
      int buffer_id,
      base::Time timestamp,
      const media::VideoCaptureFormat& format);

  
  void DoHandleErrorOnIOThread(const VideoCaptureControllerID& controller_id);

  void DoEndedOnIOThread(const VideoCaptureControllerID& controller_id);

  void DeleteVideoCaptureControllerOnIOThread(
      const VideoCaptureControllerID& controller_id);

  MediaStreamManager* media_stream_manager_;

  typedef std::map<VideoCaptureControllerID,
                   base::WeakPtr<VideoCaptureController> > EntryMap;

  
  
  
  EntryMap entries_;

  DISALLOW_COPY_AND_ASSIGN(VideoCaptureHost);
};

}  

#endif  
