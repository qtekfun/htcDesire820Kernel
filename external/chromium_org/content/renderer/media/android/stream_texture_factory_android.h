// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_ANDROID_STREAM_TEXTURE_FACTORY_ANDROID_H_
#define CONTENT_RENDERER_MEDIA_ANDROID_STREAM_TEXTURE_FACTORY_ANDROID_H_

#include "base/memory/scoped_ptr.h"
#include "cc/layers/video_frame_provider.h"
#include "gpu/command_buffer/common/mailbox.h"
#include "ui/gfx/size.h"

namespace blink {
class WebGraphicsContext3D;
}

namespace content {

class StreamTextureProxy {
 public:
  virtual ~StreamTextureProxy() {}

  
  
  virtual void BindToCurrentThread(int32 stream_id) = 0;

  virtual bool IsBoundToThread() = 0;

  
  
  virtual void SetClient(cc::VideoFrameProvider::Client* client) = 0;

  
  virtual void Release() = 0;

  struct Deleter {
    inline void operator()(StreamTextureProxy* ptr) const { ptr->Release(); }
  };
};

typedef scoped_ptr<StreamTextureProxy, StreamTextureProxy::Deleter>
    ScopedStreamTextureProxy;

class StreamTextureFactory {
 public:
  virtual ~StreamTextureFactory() {}

  
  virtual StreamTextureProxy* CreateProxy() = 0;

  
  
  
  
  virtual void EstablishPeer(int32 stream_id, int player_id) = 0;

  
  
  
  
  virtual unsigned CreateStreamTexture(
      unsigned texture_target,
      unsigned* texture_id,
      gpu::Mailbox* texture_mailbox,
      unsigned* texture_mailbox_sync_point) = 0;

  
  
  virtual void DestroyStreamTexture(unsigned texture_id) = 0;

  
  virtual void SetStreamTextureSize(int32 texture_id,
                                    const gfx::Size& size) = 0;

  virtual blink::WebGraphicsContext3D* Context3d() = 0;
};

}  

#endif  
