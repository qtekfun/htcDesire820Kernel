// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_CLIENT_GL_HELPER_H_
#define CONTENT_COMMON_GPU_CLIENT_GL_HELPER_H_

#include "base/atomicops.h"
#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "content/common/gpu/client/webgraphicscontext3d_command_buffer_impl.h"

namespace gfx {
class Rect;
class Size;
}

namespace gpu {
class ContextSupport;
struct Mailbox;
}

namespace media {
class VideoFrame;
};

class SkRegion;

namespace content {

class GLHelperScaling;

class ScopedWebGLId {
 public:
  typedef void (blink::WebGraphicsContext3D::*DeleteFunc)(WebGLId);
  ScopedWebGLId(blink::WebGraphicsContext3D* context,
                WebGLId id,
                DeleteFunc delete_func)
      : context_(context),
        id_(id),
        delete_func_(delete_func) {
  }

  operator WebGLId() const {
    return id_;
  }

  WebGLId id() const { return id_; }

  WebGLId Detach() {
    WebGLId id = id_;
    id_ = 0;
    return id;
  }

  ~ScopedWebGLId() {
    if (id_ != 0) {
      (context_->*delete_func_)(id_);
    }
  }

 private:
  blink::WebGraphicsContext3D* context_;
  WebGLId id_;
  DeleteFunc delete_func_;

  DISALLOW_COPY_AND_ASSIGN(ScopedWebGLId);
};

class ScopedBuffer : public ScopedWebGLId {
 public:
  ScopedBuffer(blink::WebGraphicsContext3D* context,
               WebGLId id)
      : ScopedWebGLId(context,
                      id,
                      &blink::WebGraphicsContext3D::deleteBuffer) {}
};

class ScopedFramebuffer : public ScopedWebGLId {
 public:
  ScopedFramebuffer(blink::WebGraphicsContext3D* context,
                    WebGLId id)
      : ScopedWebGLId(context,
                      id,
                      &blink::WebGraphicsContext3D::deleteFramebuffer) {}
};

class ScopedProgram : public ScopedWebGLId {
 public:
  ScopedProgram(blink::WebGraphicsContext3D* context,
                WebGLId id)
      : ScopedWebGLId(context,
                      id,
                      &blink::WebGraphicsContext3D::deleteProgram) {}
};

class ScopedShader : public ScopedWebGLId {
 public:
  ScopedShader(blink::WebGraphicsContext3D* context,
               WebGLId id)
      : ScopedWebGLId(context,
                      id,
                      &blink::WebGraphicsContext3D::deleteShader) {}
};

class ScopedTexture : public ScopedWebGLId {
 public:
  ScopedTexture(blink::WebGraphicsContext3D* context,
                WebGLId id)
      : ScopedWebGLId(context,
                      id,
                      &blink::WebGraphicsContext3D::deleteTexture) {}
};

template <blink::WGC3Denum target>
class ScopedBinder {
 public:
  typedef void (blink::WebGraphicsContext3D::*BindFunc)(blink::WGC3Denum,
                                                         WebGLId);
  ScopedBinder(blink::WebGraphicsContext3D* context,
               WebGLId id,
               BindFunc bind_func)
      : context_(context),
        bind_func_(bind_func) {
    (context_->*bind_func_)(target, id);
  }

  virtual ~ScopedBinder() {
    (context_->*bind_func_)(target, 0);
  }

 private:
  blink::WebGraphicsContext3D* context_;
  BindFunc bind_func_;

  DISALLOW_COPY_AND_ASSIGN(ScopedBinder);
};

template <blink::WGC3Denum target>
class ScopedBufferBinder : ScopedBinder<target> {
 public:
  ScopedBufferBinder(blink::WebGraphicsContext3D* context,
                     WebGLId id)
      : ScopedBinder<target>(
          context,
          id,
          &blink::WebGraphicsContext3D::bindBuffer) {}
};

template <blink::WGC3Denum target>
class ScopedFramebufferBinder : ScopedBinder<target> {
 public:
  ScopedFramebufferBinder(blink::WebGraphicsContext3D* context,
                          WebGLId id)
      : ScopedBinder<target>(
          context,
          id,
          &blink::WebGraphicsContext3D::bindFramebuffer) {}
};

template <blink::WGC3Denum target>
class ScopedTextureBinder : ScopedBinder<target> {
 public:
  ScopedTextureBinder(blink::WebGraphicsContext3D* context,
                      WebGLId id)
      : ScopedBinder<target>(
          context,
          id,
          &blink::WebGraphicsContext3D::bindTexture) {}
};

class ScopedFlush {
 public:
  explicit ScopedFlush(blink::WebGraphicsContext3D* context)
      : context_(context) {
  }

  ~ScopedFlush() {
    context_->flush();
  }

 private:
  blink::WebGraphicsContext3D* context_;

  DISALLOW_COPY_AND_ASSIGN(ScopedFlush);
};


class ReadbackYUVInterface;

class CONTENT_EXPORT GLHelper {
 public:
  GLHelper(blink::WebGraphicsContext3D* context,
           gpu::ContextSupport* context_support);
  ~GLHelper();

  enum ScalerQuality {
    
    SCALER_QUALITY_FAST = 1,

    
    
    
    SCALER_QUALITY_GOOD = 2,

    
    
    
    
    SCALER_QUALITY_BEST = 3,
  };


  
  
  
  
  
  
  
  
  void CropScaleReadbackAndCleanTexture(
      blink::WebGLId src_texture,
      const gfx::Size& src_size,
      const gfx::Rect& src_subrect,
      const gfx::Size& dst_size,
      unsigned char* out,
      const base::Callback<void(bool)>& callback);

  
  
  
  
  
  
  
  
  
  void CropScaleReadbackAndCleanMailbox(
      const gpu::Mailbox& src_mailbox,
      uint32 sync_point,
      const gfx::Size& src_size,
      const gfx::Rect& src_subrect,
      const gfx::Size& dst_size,
      unsigned char* out,
      const base::Callback<void(bool)>& callback);

  
  
  
  
  
  void ReadbackTextureSync(blink::WebGLId texture,
                           const gfx::Rect& src_rect,
                           unsigned char* out);

  
  
  
  blink::WebGLId CopyTexture(blink::WebGLId texture,
                              const gfx::Size& size);

  
  
  
  
  blink::WebGLId CopyAndScaleTexture(
      blink::WebGLId texture,
      const gfx::Size& src_size,
      const gfx::Size& dst_size,
      bool vertically_flip_texture,
      ScalerQuality quality);

  
  blink::WebGLId CompileShaderFromSource(const blink::WGC3Dchar* source,
                                          blink::WGC3Denum type);

  
  
  void CopySubBufferDamage(blink::WebGLId texture,
                           blink::WebGLId previous_texture,
                           const SkRegion& new_damage,
                           const SkRegion& old_damage);

  
  blink::WebGLId CreateTexture();
  
  void DeleteTexture(blink::WebGLId texture_id);

  
  uint32 InsertSyncPoint();
  
  void WaitSyncPoint(uint32 sync_point);

  
  
  
  
  gpu::Mailbox ProduceMailboxFromTexture(blink::WebGLId texture_id,
                                         uint32* sync_point);

  
  
  blink::WebGLId ConsumeMailboxToTexture(const gpu::Mailbox& mailbox,
                                          uint32 sync_point);

  
  void ResizeTexture(blink::WebGLId texture, const gfx::Size& size);

  
  void CopyTextureSubImage(blink::WebGLId texture, const gfx::Rect& rect);

  
  
  void CopyTextureFullImage(blink::WebGLId texture, const gfx::Size& size);

  
  
  
  
  class CONTENT_EXPORT ScalerInterface {
   public:
    ScalerInterface() {}
    virtual ~ScalerInterface() {}

    
    
    virtual void Scale(blink::WebGLId source_texture,
                       blink::WebGLId dest_texture) = 0;
    virtual const gfx::Size& SrcSize() = 0;
    virtual const gfx::Rect& SrcSubrect() = 0;
    virtual const gfx::Size& DstSize() = 0;
  };

  
  
  
  
  ScalerInterface* CreateScaler(ScalerQuality quality,
                                const gfx::Size& src_size,
                                const gfx::Rect& src_subrect,
                                const gfx::Size& dst_size,
                                bool vertically_flip_texture,
                                bool swizzle);

  
  
  
  
  
  
  
  
  ReadbackYUVInterface* CreateReadbackPipelineYUV(
      ScalerQuality quality,
      const gfx::Size& src_size,
      const gfx::Rect& src_subrect,
      const gfx::Size& dst_size,
      const gfx::Rect& dst_subrect,
      bool flip_vertically,
      bool use_mrt);

  
  
  blink::WGC3Dint MaxDrawBuffers();

 protected:
  class CopyTextureToImpl;

  
  void InitCopyTextToImpl();
  
  void InitScalerImpl();

  blink::WebGraphicsContext3D* context_;
  gpu::ContextSupport* context_support_;
  scoped_ptr<CopyTextureToImpl> copy_texture_to_impl_;
  scoped_ptr<GLHelperScaling> scaler_impl_;

  DISALLOW_COPY_AND_ASSIGN(GLHelper);
};

class CONTENT_EXPORT ReadbackYUVInterface {
public:
  ReadbackYUVInterface() {}
  virtual ~ReadbackYUVInterface() {}

  
  virtual void ReadbackYUV(
      const gpu::Mailbox& mailbox,
      uint32 sync_point,
      const scoped_refptr<media::VideoFrame>& target,
      const base::Callback<void(bool)>& callback) = 0;
  virtual GLHelper::ScalerInterface* scaler() = 0;
};

}  

#endif  
