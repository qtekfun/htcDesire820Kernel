// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ANDROID_SURFACE_TEXTURE_PEER_BROWSER_IMPL_H_
#define CONTENT_BROWSER_ANDROID_SURFACE_TEXTURE_PEER_BROWSER_IMPL_H_

#include "base/compiler_specific.h"
#include "content/common/android/surface_texture_peer.h"

namespace content {

class SurfaceTexturePeerBrowserImpl : public SurfaceTexturePeer {
 public:
  
  
  
  
  
  SurfaceTexturePeerBrowserImpl();
  virtual ~SurfaceTexturePeerBrowserImpl();

  
  virtual void EstablishSurfaceTexturePeer(
      base::ProcessHandle render_process_handle,
      scoped_refptr<gfx::SurfaceTexture> surface_texture,
      int render_view_id,
      int player_id) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(SurfaceTexturePeerBrowserImpl);
};

}  

#endif  
