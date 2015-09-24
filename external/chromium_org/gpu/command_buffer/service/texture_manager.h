// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_TEXTURE_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_TEXTURE_MANAGER_H_

#include <list>
#include <set>
#include <string>
#include <vector>
#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "gpu/command_buffer/service/async_pixel_transfer_delegate.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/command_buffer/service/memory_tracking.h"
#include "gpu/gpu_export.h"
#include "ui/gl/gl_image.h"

namespace gpu {

class StreamTextureManager;

namespace gles2 {

class GLES2Decoder;
struct ContextState;
struct DecoderFramebufferState;
class Display;
class ErrorState;
class FeatureInfo;
class FramebufferManager;
class MailboxManager;
class TextureManager;
class TextureRef;

class GPU_EXPORT Texture {
 public:
  explicit Texture(GLuint service_id);

  GLenum min_filter() const {
    return min_filter_;
  }

  GLenum mag_filter() const {
    return mag_filter_;
  }

  GLenum wrap_s() const {
    return wrap_s_;
  }

  GLenum wrap_t() const {
    return wrap_t_;
  }

  GLenum usage() const {
    return usage_;
  }

  GLenum pool() const {
    return pool_;
  }

  int num_uncleared_mips() const {
    return num_uncleared_mips_;
  }

  uint32 estimated_size() const {
    return estimated_size_;
  }

  bool CanRenderTo() const {
    return !stream_texture_ && target_ != GL_TEXTURE_EXTERNAL_OES;
  }

  
  GLuint service_id() const {
    return service_id_;
  }

  void SetServiceId(GLuint service_id) {
    DCHECK(service_id);
    service_id_ = service_id;
  }

  
  
  
  GLenum target() const {
    return target_;
  }

  bool SafeToRenderFrom() const {
    return cleared_;
  }

  
  
  bool GetLevelSize(
      GLint target, GLint level, GLsizei* width, GLsizei* height) const;

  
  bool GetLevelType(
      GLint target, GLint level, GLenum* type, GLenum* internal_format) const;

  
  
  gfx::GLImage* GetLevelImage(GLint target, GLint level) const;

  bool HasImages() const {
    return has_images_;
  }

  
  
  bool ValidForTexture(
      GLint target,
      GLint level,
      GLint xoffset,
      GLint yoffset,
      GLsizei width,
      GLsizei height,
      GLenum format,
      GLenum type) const;

  bool IsValid() const {
    return !!target();
  }

  bool IsAttachedToFramebuffer() const {
    return framebuffer_attachment_count_ != 0;
  }

  void AttachToFramebuffer() {
    ++framebuffer_attachment_count_;
  }

  void DetachFromFramebuffer() {
    DCHECK_GT(framebuffer_attachment_count_, 0);
    --framebuffer_attachment_count_;
  }

  bool IsStreamTexture() const {
    return stream_texture_;
  }

  void SetImmutable(bool immutable) {
    immutable_ = immutable;
  }

  bool IsImmutable() const {
    return immutable_;
  }

  
  bool IsLevelCleared(GLenum target, GLint level) const;

  
  bool IsDefined() const {
    return estimated_size() > 0;
  }

  void OnWillModifyPixels();
  void OnDidModifyPixels();

 private:
  friend class MailboxManager;
  friend class MailboxManagerTest;
  friend class TextureDefinition;
  friend class TextureManager;
  friend class TextureRef;
  friend class TextureTestHelper;

  ~Texture();
  void AddTextureRef(TextureRef* ref);
  void RemoveTextureRef(TextureRef* ref, bool have_context);
  MemoryTypeTracker* GetMemTracker();

  
  
  
  
  
  
  enum CanRenderCondition {
    CAN_RENDER_ALWAYS,
    CAN_RENDER_NEVER,
    CAN_RENDER_ONLY_IF_NPOT
  };

  struct LevelInfo {
    LevelInfo();
    LevelInfo(const LevelInfo& rhs);
    ~LevelInfo();

    bool cleared;
    GLenum target;
    GLint level;
    GLenum internal_format;
    GLsizei width;
    GLsizei height;
    GLsizei depth;
    GLint border;
    GLenum format;
    GLenum type;
    scoped_refptr<gfx::GLImage> image;
    uint32 estimated_size;
  };

  
  void SetLevelInfo(
      const FeatureInfo* feature_info,
      GLenum target,
      GLint level,
      GLenum internal_format,
      GLsizei width,
      GLsizei height,
      GLsizei depth,
      GLint border,
      GLenum format,
      GLenum type,
      bool cleared);

  
  
  
  bool texture_complete() const {
    return texture_complete_;
  }

  
  
  bool cube_complete() const {
    return cube_complete_;
  }

  
  bool npot() const {
    return npot_;
  }

  void SetStreamTexture(bool stream_texture) {
    stream_texture_ = stream_texture;
    UpdateCanRenderCondition();
  }

  
  void SetLevelCleared(GLenum target, GLint level, bool cleared);

  
  void UpdateCleared();

  
  
  bool ClearRenderableLevels(GLES2Decoder* decoder);

  
  
  bool ClearLevel(GLES2Decoder* decoder, GLenum target, GLint level);

  
  
  
  GLenum SetParameter(
      const FeatureInfo* feature_info, GLenum pname, GLint param);

  
  bool MarkMipmapsGenerated(const FeatureInfo* feature_info);

  bool NeedsMips() const {
    return min_filter_ != GL_NEAREST && min_filter_ != GL_LINEAR;
  }

  
  
  bool CanRender(const FeatureInfo* feature_info) const;

  
  bool CanGenerateMipmaps(const FeatureInfo* feature_info) const;

  
  
  
  
  
  void SetTarget(
      const FeatureInfo* feature_info, GLenum target, GLint max_levels);

  
  void Update(const FeatureInfo* feature_info);

  
  void SetLevelImage(
      const FeatureInfo* feature_info,
      GLenum target,
      GLint level,
      gfx::GLImage* image);

  
  void AddToSignature(
      const FeatureInfo* feature_info,
      GLenum target, GLint level, std::string* signature) const;

  void SetMailboxManager(MailboxManager* mailbox_manager);

  
  
  void UpdateSafeToRenderFrom(bool cleared);

  
  
  void UpdateMipCleared(LevelInfo* info, bool cleared);

  
  CanRenderCondition GetCanRenderCondition() const;

  
  
  void UpdateCanRenderCondition();

  
  
  void UpdateHasImages();

  
  
  void IncAllFramebufferStateChangeCount();

  MailboxManager* mailbox_manager_;

  
  std::vector<std::vector<LevelInfo> > level_infos_;

  
  typedef std::set<TextureRef*> RefSet;
  RefSet refs_;

  
  
  TextureRef* memory_tracking_ref_;

  
  GLuint service_id_;

  
  bool cleared_;

  int num_uncleared_mips_;

  
  GLenum target_;

  
  GLenum min_filter_;
  GLenum mag_filter_;
  GLenum wrap_s_;
  GLenum wrap_t_;
  GLenum usage_;
  GLenum pool_;

  
  GLint max_level_set_;

  
  bool texture_complete_;

  
  bool cube_complete_;

  
  bool npot_;

  
  bool has_been_bound_;

  
  int framebuffer_attachment_count_;

  
  bool stream_texture_;

  
  
  bool immutable_;

  
  bool has_images_;

  
  uint32 estimated_size_;

  
  CanRenderCondition can_render_condition_;

  DISALLOW_COPY_AND_ASSIGN(Texture);
};

class GPU_EXPORT TextureRef : public base::RefCounted<TextureRef> {
 public:
  TextureRef(TextureManager* manager, GLuint client_id, Texture* texture);
  static scoped_refptr<TextureRef> Create(TextureManager* manager,
                                          GLuint client_id,
                                          GLuint service_id);
  const Texture* texture() const { return texture_; }
  Texture* texture() { return texture_; }
  GLuint client_id() const { return client_id_; }
  GLuint service_id() const { return texture_->service_id(); }

 private:
  friend class base::RefCounted<TextureRef>;
  friend class Texture;
  friend class TextureManager;

  ~TextureRef();
  const TextureManager* manager() const { return manager_; }
  TextureManager* manager() { return manager_; }
  void reset_client_id() { client_id_ = 0; }
  void set_is_stream_texture_owner(bool owner) {
    is_stream_texture_owner_ = owner;
  }
  bool is_stream_texture_owner() const { return is_stream_texture_owner_; }

  TextureManager* manager_;
  Texture* texture_;
  GLuint client_id_;
  bool is_stream_texture_owner_;

  DISALLOW_COPY_AND_ASSIGN(TextureRef);
};

struct DecoderTextureState {
  
  
  DecoderTextureState(bool texsubimage2d_faster_than_teximage2d)
      : tex_image_2d_failed(false),
        texture_upload_count(0),
        texsubimage2d_faster_than_teximage2d(
            texsubimage2d_faster_than_teximage2d) {}

  
  
  bool tex_image_2d_failed;

  
  int texture_upload_count;
  base::TimeDelta total_texture_upload_time;

  bool texsubimage2d_faster_than_teximage2d;
};

class GPU_EXPORT TextureManager {
 public:
  class GPU_EXPORT DestructionObserver {
   public:
    DestructionObserver();
    virtual ~DestructionObserver();

    
    virtual void OnTextureManagerDestroying(TextureManager* manager) = 0;

    
    virtual void OnTextureRefDestroying(TextureRef* texture) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(DestructionObserver);
  };

  enum DefaultAndBlackTextures {
    kTexture2D,
    kCubeMap,
    kExternalOES,
    kRectangleARB,
    kNumDefaultTextures
  };

  TextureManager(MemoryTracker* memory_tracker,
                 FeatureInfo* feature_info,
                 GLsizei max_texture_size,
                 GLsizei max_cube_map_texture_size);
  ~TextureManager();

  void set_framebuffer_manager(FramebufferManager* manager) {
    framebuffer_manager_ = manager;
  }

  void set_stream_texture_manager(StreamTextureManager* manager) {
    stream_texture_manager_ = manager;
  }

  
  bool Initialize();

  
  void Destroy(bool have_context);

  
  GLint MaxLevelsForTarget(GLenum target) const {
    switch (target) {
      case GL_TEXTURE_2D:
        return  max_levels_;
      case GL_TEXTURE_EXTERNAL_OES:
        return 1;
      default:
        return max_cube_map_levels_;
    }
  }

  
  GLsizei MaxSizeForTarget(GLenum target) const {
    switch (target) {
      case GL_TEXTURE_2D:
      case GL_TEXTURE_EXTERNAL_OES:
        return max_texture_size_;
      default:
        return max_cube_map_texture_size_;
    }
  }

  
  static GLsizei ComputeMipMapCount(GLenum target,
                                    GLsizei width,
                                    GLsizei height,
                                    GLsizei depth);

  
  bool ValidForTarget(
      GLenum target, GLint level,
      GLsizei width, GLsizei height, GLsizei depth);

  
  
  bool CanRender(const TextureRef* ref) const {
    return ref->texture()->CanRender(feature_info_.get());
  }

  
  bool CanGenerateMipmaps(const TextureRef* ref) const {
    return ref->texture()->CanGenerateMipmaps(feature_info_.get());
  }

  
  
  
  
  void SetTarget(
      TextureRef* ref,
      GLenum target);

  
  
  void SetStreamTexture(TextureRef* ref, bool stream_texture);

  
  bool IsStreamTextureOwner(TextureRef* ref);

  
  void SetLevelInfo(
      TextureRef* ref,
      GLenum target,
      GLint level,
      GLenum internal_format,
      GLsizei width,
      GLsizei height,
      GLsizei depth,
      GLint border,
      GLenum format,
      GLenum type,
      bool cleared);

  
  void SetLevelInfoFromParams(TextureRef* ref,
                              const gpu::AsyncTexImage2DParams& params) {
    SetLevelInfo(
        ref, params.target, params.level, params.internal_format,
        params.width, params.height, 1 ,
        params.border, params.format,
        params.type, true  );
  }

  Texture* Produce(TextureRef* ref);

  
  TextureRef* Consume(GLuint client_id, Texture* texture);

  
  void SetLevelCleared(TextureRef* ref, GLenum target,
                       GLint level, bool cleared);

  
  
  
  void SetParameter(
      const char* function_name, ErrorState* error_state,
      TextureRef* ref, GLenum pname, GLint param);

  
  
  bool MarkMipmapsGenerated(TextureRef* ref);

  
  bool ClearRenderableLevels(GLES2Decoder* decoder, TextureRef* ref);

  
  bool ClearTextureLevel(
      GLES2Decoder* decoder, TextureRef* ref, GLenum target, GLint level);

  
  TextureRef* CreateTexture(GLuint client_id, GLuint service_id);

  
  TextureRef* GetTexture(GLuint client_id) const;

  
  void RemoveTexture(GLuint client_id);

  
  
  Texture* GetTextureForServiceId(GLuint service_id) const;

  TextureRef* GetDefaultTextureInfo(GLenum target) {
    switch (target) {
      case GL_TEXTURE_2D:
        return default_textures_[kTexture2D].get();
      case GL_TEXTURE_CUBE_MAP:
        return default_textures_[kCubeMap].get();
      case GL_TEXTURE_EXTERNAL_OES:
        return default_textures_[kExternalOES].get();
      case GL_TEXTURE_RECTANGLE_ARB:
        return default_textures_[kRectangleARB].get();
      default:
        NOTREACHED();
        return NULL;
    }
  }

  bool HaveUnrenderableTextures() const {
    return num_unrenderable_textures_ > 0;
  }

  bool HaveUnsafeTextures() const {
    return num_unsafe_textures_ > 0;
  }

  bool HaveUnclearedMips() const {
    return num_uncleared_mips_ > 0;
  }

  bool HaveImages() const {
    return num_images_ > 0;
  }

  GLuint black_texture_id(GLenum target) const {
    switch (target) {
      case GL_SAMPLER_2D:
        return black_texture_ids_[kTexture2D];
      case GL_SAMPLER_CUBE:
        return black_texture_ids_[kCubeMap];
      case GL_SAMPLER_EXTERNAL_OES:
        return black_texture_ids_[kExternalOES];
      case GL_SAMPLER_2D_RECT_ARB:
        return black_texture_ids_[kRectangleARB];
      default:
        NOTREACHED();
        return 0;
    }
  }

  size_t mem_represented() const {
    return
        memory_tracker_managed_->GetMemRepresented() +
        memory_tracker_unmanaged_->GetMemRepresented();
  }

  void SetLevelImage(
      TextureRef* ref,
      GLenum target,
      GLint level,
      gfx::GLImage* image);

  void AddToSignature(
      TextureRef* ref,
      GLenum target,
      GLint level,
      std::string* signature) const;

  void AddObserver(DestructionObserver* observer) {
    destruction_observers_.AddObserver(observer);
  }

  void RemoveObserver(DestructionObserver* observer) {
    destruction_observers_.RemoveObserver(observer);
  }

  struct DoTextImage2DArguments {
    GLenum target;
    GLint level;
    GLenum internal_format;
    GLsizei width;
    GLsizei height;
    GLint border;
    GLenum format;
    GLenum type;
    const void* pixels;
    uint32 pixels_size;
  };

  bool ValidateTexImage2D(
    ContextState* state,
    const char* function_name,
    const DoTextImage2DArguments& args,
    
    
    TextureRef** texture_ref);

  void ValidateAndDoTexImage2D(
    DecoderTextureState* texture_state,
    ContextState* state,
    DecoderFramebufferState* framebuffer_state,
    const DoTextImage2DArguments& args);

  
  
  TextureRef* GetTextureInfoForTarget(ContextState* state, GLenum target);
  TextureRef* GetTextureInfoForTargetUnlessDefault(
      ContextState* state, GLenum target);

  bool ValidateTextureParameters(
    ErrorState* error_state, const char* function_name,
    GLenum target, GLenum format, GLenum type, GLint level);

 private:
  friend class Texture;
  friend class TextureRef;

  
  scoped_refptr<TextureRef> CreateDefaultAndBlackTextures(
      GLenum target,
      GLuint* black_texture);

  void DoTexImage2D(
    DecoderTextureState* texture_state,
    ErrorState* error_state,
    DecoderFramebufferState* framebuffer_state,
    TextureRef* texture_ref,
    const DoTextImage2DArguments& args);

  void StartTracking(TextureRef* texture);
  void StopTracking(TextureRef* texture);

  void UpdateSafeToRenderFrom(int delta);
  void UpdateUnclearedMips(int delta);
  void UpdateCanRenderCondition(Texture::CanRenderCondition old_condition,
                                Texture::CanRenderCondition new_condition);
  void UpdateNumImages(int delta);
  void IncFramebufferStateChangeCount();

  MemoryTypeTracker* GetMemTracker(GLenum texture_pool);
  scoped_ptr<MemoryTypeTracker> memory_tracker_managed_;
  scoped_ptr<MemoryTypeTracker> memory_tracker_unmanaged_;

  scoped_refptr<FeatureInfo> feature_info_;

  FramebufferManager* framebuffer_manager_;
  StreamTextureManager* stream_texture_manager_;

  
  typedef base::hash_map<GLuint, scoped_refptr<TextureRef> > TextureMap;
  TextureMap textures_;

  GLsizei max_texture_size_;
  GLsizei max_cube_map_texture_size_;
  GLint max_levels_;
  GLint max_cube_map_levels_;

  int num_unrenderable_textures_;
  int num_unsafe_textures_;
  int num_uncleared_mips_;
  int num_images_;

  
  
  unsigned int texture_count_;

  bool have_context_;

  
  
  
  GLuint black_texture_ids_[kNumDefaultTextures];

  
  scoped_refptr<TextureRef> default_textures_[kNumDefaultTextures];

  ObserverList<DestructionObserver> destruction_observers_;

  DISALLOW_COPY_AND_ASSIGN(TextureManager);
};

class ScopedTextureUploadTimer {
 public:
  explicit ScopedTextureUploadTimer(DecoderTextureState* texture_state);
  ~ScopedTextureUploadTimer();

 private:
  DecoderTextureState* texture_state_;
  base::TimeTicks begin_time_;
  DISALLOW_COPY_AND_ASSIGN(ScopedTextureUploadTimer);
};

}  
}  

#endif  
