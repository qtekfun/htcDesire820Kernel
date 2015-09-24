// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_RECTANGLE_UPDATE_DECODER_H_
#define REMOTING_CLIENT_RECTANGLE_UPDATE_DECODER_H_

#include <list>

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/codec/video_decoder.h"
#include "remoting/client/chromoting_stats.h"
#include "remoting/client/frame_consumer_proxy.h"
#include "remoting/client/frame_producer.h"
#include "remoting/protocol/video_stub.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {

class ChromotingStats;

namespace protocol {
class SessionConfig;
}  

class RectangleUpdateDecoder
    : public base::RefCountedThreadSafe<RectangleUpdateDecoder>,
      public FrameProducer,
      public protocol::VideoStub {
 public:
  
  
  
  
  
  RectangleUpdateDecoder(
      scoped_refptr<base::SingleThreadTaskRunner> main_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> decode_task_runner,
      scoped_refptr<FrameConsumerProxy> consumer);

  
  void Initialize(const protocol::SessionConfig& config);

  
  
  virtual void DrawBuffer(webrtc::DesktopFrame* buffer) OVERRIDE;
  virtual void InvalidateRegion(const webrtc::DesktopRegion& region) OVERRIDE;
  virtual void RequestReturnBuffers(const base::Closure& done) OVERRIDE;
  virtual void SetOutputSizeAndClip(
      const webrtc::DesktopSize& view_size,
      const webrtc::DesktopRect& clip_area) OVERRIDE;
  virtual const webrtc::DesktopRegion* GetBufferShape() OVERRIDE;

  
  virtual void ProcessVideoPacket(scoped_ptr<VideoPacket> packet,
                                  const base::Closure& done) OVERRIDE;

  
  ChromotingStats* GetStats();

 private:
  friend class base::RefCountedThreadSafe<RectangleUpdateDecoder>;
  virtual ~RectangleUpdateDecoder();

  
  
  void SchedulePaint();
  void DoPaint();

  
  
  
  void DecodePacket(scoped_ptr<VideoPacket> packet, const base::Closure& done);

  
  
  void OnPacketDone(base::Time decode_start, const base::Closure& done);

  scoped_refptr<base::SingleThreadTaskRunner> main_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> decode_task_runner_;
  scoped_refptr<FrameConsumerProxy> consumer_;
  scoped_ptr<VideoDecoder> decoder_;

  
  webrtc::DesktopSize source_size_;

  
  webrtc::DesktopVector source_dpi_;

  
  webrtc::DesktopSize view_size_;
  webrtc::DesktopRect clip_area_;

  
  std::list<webrtc::DesktopFrame*> buffers_;

  
  bool paint_scheduled_;

  ChromotingStats stats_;

  
  int64 latest_sequence_number_;
};

}  

#endif  
