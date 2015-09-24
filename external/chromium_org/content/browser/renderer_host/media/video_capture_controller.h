// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_CONTROLLER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_CONTROLLER_H_

#include <list>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/process/process.h"
#include "content/browser/renderer_host/media/video_capture_buffer_pool.h"
#include "content/browser/renderer_host/media/video_capture_controller_event_handler.h"
#include "content/common/content_export.h"
#include "content/common/media/video_capture.h"
#include "media/video/capture/video_capture.h"
#include "media/video/capture/video_capture_device.h"
#include "media/video/capture/video_capture_types.h"

namespace content {
class VideoCaptureBufferPool;

class CONTENT_EXPORT VideoCaptureController {
 public:
  VideoCaptureController();
  virtual ~VideoCaptureController();

  base::WeakPtr<VideoCaptureController> GetWeakPtr();

  
  
  scoped_ptr<media::VideoCaptureDevice::Client> NewDeviceClient();

  
  
  
  void AddClient(const VideoCaptureControllerID& id,
                 VideoCaptureControllerEventHandler* event_handler,
                 base::ProcessHandle render_process,
                 media::VideoCaptureSessionId session_id,
                 const media::VideoCaptureParams& params);

  
  
  
  
  int RemoveClient(const VideoCaptureControllerID& id,
                   VideoCaptureControllerEventHandler* event_handler);

  int GetClientCount();

  
  
  void StopSession(int session_id);

  
  
  void ReturnBuffer(const VideoCaptureControllerID& id,
                    VideoCaptureControllerEventHandler* event_handler,
                    int buffer_id);

  const media::VideoCaptureFormat& GetVideoCaptureFormat() const;

 private:
  class VideoCaptureDeviceClient;

  struct ControllerClient;
  typedef std::list<ControllerClient*> ControllerClients;

  
  void DoIncomingCapturedI420BufferOnIOThread(
      scoped_refptr<media::VideoCaptureDevice::Client::Buffer> buffer,
      const gfx::Size& dimensions,
      int frame_rate,
      base::Time timestamp);
  void DoErrorOnIOThread();
  void DoDeviceStoppedOnIOThread();
  void DoBufferDestroyedOnIOThread(int buffer_id_to_drop);

  
  ControllerClient* FindClient(
      const VideoCaptureControllerID& id,
      VideoCaptureControllerEventHandler* handler,
      const ControllerClients& clients);

  
  ControllerClient* FindClient(
      int session_id,
      const ControllerClients& clients);

  
  const scoped_refptr<VideoCaptureBufferPool> buffer_pool_;

  
  ControllerClients controller_clients_;

  
  
  VideoCaptureState state_;

  media::VideoCaptureFormat video_capture_format_;

  base::WeakPtrFactory<VideoCaptureController> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(VideoCaptureController);
};

}  

#endif  
