// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_STREAM_TEXTURE_MANAGER_ANDROID_H_
#define CONTENT_COMMON_GPU_STREAM_TEXTURE_MANAGER_ANDROID_H_

#include "base/callback.h"
#include "base/id_map.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/common/android/surface_texture_peer.h"
#include "gpu/command_buffer/service/stream_texture.h"
#include "gpu/command_buffer/service/stream_texture_manager.h"

struct GpuStreamTextureMsg_MatrixChanged_Params;

namespace gfx {
class Size;
class SurfaceTexture;
}

namespace content {
class GpuChannel;

class StreamTextureManagerAndroid : public gpu::StreamTextureManager {
 public:
  StreamTextureManagerAndroid(GpuChannel* channel);
  virtual ~StreamTextureManagerAndroid();

  
  virtual uint32 CreateStreamTexture(uint32 service_id,
                                     uint32 client_id) OVERRIDE;
  virtual void DestroyStreamTexture(uint32 service_id) OVERRIDE;
  virtual gpu::StreamTexture* LookupStreamTexture(uint32 service_id) OVERRIDE;

  
  void RegisterStreamTextureProxy(int32 stream_id, int32 route_id);
  void EstablishStreamTexture(
      int32 stream_id, int32 primary_id, int32 secondary_id);
  void SetStreamTextureSize(int32 stream_id, const gfx::Size& size);

  
  void SendMatrixChanged(int route_id,
      const GpuStreamTextureMsg_MatrixChanged_Params& params);

 private:
  
  class StreamTextureAndroid
      : public gpu::StreamTexture,
        public base::SupportsWeakPtr<StreamTextureAndroid> {
   public:
    StreamTextureAndroid(GpuChannel* channel, int service_id);
    virtual ~StreamTextureAndroid();

    virtual void Update() OVERRIDE;
    virtual gfx::Size GetSize() OVERRIDE;

    scoped_refptr<gfx::SurfaceTexture> surface_texture() {
        return surface_texture_;
    }

    
    void OnFrameAvailable(int route_id);

    
    void SetSize(const gfx::Size& size) { size_ = size; }

    
    
    typedef base::Callback<
        void(const GpuStreamTextureMsg_MatrixChanged_Params&)>
            MatrixChangedCB;

    void set_matrix_changed_callback(const MatrixChangedCB& callback) {
        matrix_callback_ = callback;
    }

   private:
    scoped_refptr<gfx::SurfaceTexture> surface_texture_;

    
    float current_matrix_[16];

    
    gfx::Size size_;

    
    bool has_updated_;

    MatrixChangedCB matrix_callback_;

    GpuChannel* channel_;
    DISALLOW_COPY_AND_ASSIGN(StreamTextureAndroid);
  };

  GpuChannel* channel_;

  typedef IDMap<StreamTextureAndroid, IDMapOwnPointer> TextureMap;
  TextureMap textures_;

  
  typedef IDMap<gpu::StreamTexture, IDMapExternalPointer> TextureServiceIdMap;
  TextureServiceIdMap textures_from_service_id_;

  DISALLOW_COPY_AND_ASSIGN(StreamTextureManagerAndroid);
};

}  

#endif  