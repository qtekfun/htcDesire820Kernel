// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_JNI_JNI_FRAME_CONSUMER_H_
#define REMOTING_CLIENT_JNI_JNI_FRAME_CONSUMER_H_

#include <list>

#include "base/android/scoped_java_ref.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/client/frame_consumer.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"

namespace gfx {
class JavaBitmap;
}  

namespace webrtc {
class DesktopFrame;
}  

namespace remoting {
class ChromotingJniInstance;
class ChromotingJniRuntime;
class FrameProducer;

class JniFrameConsumer : public FrameConsumer {
 public:
  
  explicit JniFrameConsumer(ChromotingJniRuntime* jni_runtime,
                            scoped_refptr<ChromotingJniInstance> jni_instance);

  virtual ~JniFrameConsumer();

  
  void set_frame_producer(FrameProducer* producer);

  
  virtual void ApplyBuffer(const webrtc::DesktopSize& view_size,
                           const webrtc::DesktopRect& clip_area,
                           webrtc::DesktopFrame* buffer,
                           const webrtc::DesktopRegion& region) OVERRIDE;
  virtual void ReturnBuffer(webrtc::DesktopFrame* buffer) OVERRIDE;
  virtual void SetSourceSize(const webrtc::DesktopSize& source_size,
                             const webrtc::DesktopVector& dpi) OVERRIDE;
  virtual PixelFormat GetPixelFormat() OVERRIDE;

 private:
  
  
  void AllocateBuffer();

  
  void FreeBuffer(webrtc::DesktopFrame* buffer);

  

  
  ChromotingJniRuntime* jni_runtime_;

  
  scoped_refptr<ChromotingJniInstance> jni_instance_;

  FrameProducer* frame_producer_;
  webrtc::DesktopSize view_size_;
  webrtc::DesktopRect clip_area_;

  
  std::list<webrtc::DesktopFrame*> buffers_;

  
  
  
  
  base::android::ScopedJavaGlobalRef<jobject> bitmap_global_ref_;

  
  
  scoped_ptr<gfx::JavaBitmap> bitmap_;

  DISALLOW_COPY_AND_ASSIGN(JniFrameConsumer);
};

}  

#endif
