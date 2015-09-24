// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_CLIENT_PLUGIN_PEPPER_VIEW_H_
#define REMOTING_CLIENT_PLUGIN_PEPPER_VIEW_H_

#include <list>

#include "base/compiler_specific.h"
#include "ppapi/cpp/graphics_2d.h"
#include "ppapi/cpp/view.h"
#include "ppapi/cpp/point.h"
#include "ppapi/utility/completion_callback_factory.h"
#include "remoting/client/frame_consumer.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_region.h"

namespace base {
class Time;
}  

namespace webrtc {
class DesktopFrame;
}  

namespace remoting {

class ChromotingInstance;
class ClientContext;
class FrameProducer;

class PepperView : public FrameConsumer {
 public:
  
  
  PepperView(ChromotingInstance* instance, ClientContext* context);
  virtual ~PepperView();

  
  
  void Initialize(FrameProducer* producer);

  
  virtual void ApplyBuffer(const webrtc::DesktopSize& view_size,
                           const webrtc::DesktopRect& clip_area,
                           webrtc::DesktopFrame* buffer,
                           const webrtc::DesktopRegion& region) OVERRIDE;
  virtual void ReturnBuffer(webrtc::DesktopFrame* buffer) OVERRIDE;
  virtual void SetSourceSize(const webrtc::DesktopSize& source_size,
                             const webrtc::DesktopVector& dpi) OVERRIDE;
  virtual PixelFormat GetPixelFormat() OVERRIDE;

  
  
  void SetView(const pp::View& view);

  
  const webrtc::DesktopSize& get_source_size() const {
    return source_size_;
  }

  
  
  const webrtc::DesktopSize& get_view_size_dips() const {
    return dips_size_;
  }

 private:
  
  
  webrtc::DesktopFrame* AllocateBuffer();

  
  void FreeBuffer(webrtc::DesktopFrame* buffer);

  
  
  
  
  void FlushBuffer(const webrtc::DesktopRect& clip_area,
                   webrtc::DesktopFrame* buffer,
                   const webrtc::DesktopRegion& region);

  
  
  void OnFlushDone(int result,
                   const base::Time& paint_start,
                   webrtc::DesktopFrame* buffer);

  
  
  
  ChromotingInstance* const instance_;

  
  ClientContext* const context_;

  pp::Graphics2D graphics2d_;

  FrameProducer* producer_;

  
  std::list<webrtc::DesktopFrame*> buffers_;

  
  webrtc::DesktopFrame* merge_buffer_;
  webrtc::DesktopRect merge_clip_area_;
  webrtc::DesktopRegion merge_region_;

  
  webrtc::DesktopSize dips_size_;

  
  float dips_to_device_scale_;

  
  
  
  webrtc::DesktopSize view_size_;

  
  float dips_to_view_scale_;

  
  webrtc::DesktopRect clip_area_;

  
  webrtc::DesktopSize source_size_;

  
  webrtc::DesktopVector source_dpi_;

  
  bool flush_pending_;

  
  bool is_initialized_;

  
  bool frame_received_;

  pp::CompletionCallbackFactory<PepperView> callback_factory_;

  DISALLOW_COPY_AND_ASSIGN(PepperView);
};

}  

#endif  
