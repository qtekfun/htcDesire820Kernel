// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_RESOURCES_RESOURCE_PROVIDER_H_
#define CC_RESOURCES_RESOURCE_PROVIDER_H_

#include <deque>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread_checker.h"
#include "cc/base/cc_export.h"
#include "cc/output/context_provider.h"
#include "cc/output/output_surface.h"
#include "cc/resources/release_callback.h"
#include "cc/resources/resource_format.h"
#include "cc/resources/return_callback.h"
#include "cc/resources/single_release_callback.h"
#include "cc/resources/texture_mailbox.h"
#include "cc/resources/transferable_resource.h"
#include "third_party/khronos/GLES2/gl2.h"
#include "third_party/khronos/GLES2/gl2ext.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "ui/gfx/size.h"

namespace gpu {
namespace gles {
class GLES2Interface;
}
}

namespace gfx {
class Rect;
class Vector2d;
}

namespace cc {
class IdAllocator;
class SharedBitmap;
class SharedBitmapManager;
class TextureUploader;

class CC_EXPORT ResourceProvider {
 public:
  typedef unsigned ResourceId;
  typedef std::vector<ResourceId> ResourceIdArray;
  typedef std::set<ResourceId> ResourceIdSet;
  typedef base::hash_map<ResourceId, ResourceId> ResourceIdMap;
  enum TextureUsageHint {
    TextureUsageAny,
    TextureUsageFramebuffer,
  };
  enum ResourceType {
    InvalidType = 0,
    GLTexture = 1,
    Bitmap,
  };

  static scoped_ptr<ResourceProvider> Create(
      OutputSurface* output_surface,
      SharedBitmapManager* shared_bitmap_manager,
      int highp_threshold_min,
      bool use_rgba_4444_texture_format,
      size_t id_allocation_chunk_size);
  virtual ~ResourceProvider();

  void InitializeSoftware();
  bool InitializeGL();

  void DidLoseOutputSurface() { lost_output_surface_ = true; }

  int max_texture_size() const { return max_texture_size_; }
  ResourceFormat memory_efficient_texture_format() const {
    return use_rgba_4444_texture_format_ ? RGBA_4444 : best_texture_format_;
  }
  ResourceFormat best_texture_format() const { return best_texture_format_; }
  size_t num_resources() const { return resources_.size(); }

  
  bool InUseByConsumer(ResourceId id);

  bool IsLost(ResourceId id);

  

  ResourceType default_resource_type() const { return default_resource_type_; }
  ResourceType GetResourceType(ResourceId id);

  
  ResourceId CreateResource(gfx::Size size,
                            GLint wrap_mode,
                            TextureUsageHint hint,
                            ResourceFormat format);

  
  
  ResourceId CreateManagedResource(gfx::Size size,
                                   GLenum target,
                                   GLint wrap_mode,
                                   TextureUsageHint hint,
                                   ResourceFormat format);

  
  ResourceId CreateGLTexture(gfx::Size size,
                             GLenum target,
                             GLenum texture_pool,
                             GLint wrap_mode,
                             TextureUsageHint hint,
                             ResourceFormat format);

  ResourceId CreateBitmap(gfx::Size size, GLint wrap_mode);
  
  ResourceId CreateResourceFromExternalTexture(
      unsigned texture_target,
      unsigned texture_id);

  
  ResourceId CreateResourceFromTextureMailbox(
      const TextureMailbox& mailbox,
      scoped_ptr<SingleReleaseCallback> release_callback);

  void DeleteResource(ResourceId id);

  
  
  void SetPixels(ResourceId id,
                 const uint8_t* image,
                 gfx::Rect image_rect,
                 gfx::Rect source_rect,
                 gfx::Vector2d dest_offset);

  
  size_t NumBlockingUploads();
  void MarkPendingUploadsAsNonBlocking();
  size_t EstimatedUploadsPerTick();
  void FlushUploads();
  void ReleaseCachedData();
  base::TimeTicks EstimatedUploadCompletionTime(size_t uploads_per_tick);

  
  
  void Flush();

  
  
  void Finish();

  
  
  bool ShallowFlushIfSupported();

  
  int CreateChild(const ReturnCallback& return_callback);

  
  void DestroyChild(int child);

  
  const ResourceIdMap& GetChildToParentMap(int child) const;

  
  
  
  
  void PrepareSendToParent(const ResourceIdArray& resources,
                           TransferableResourceArray* transferable_resources);

  
  
  
  
  
  
  
  
  
  void ReceiveFromChild(
      int child, const TransferableResourceArray& transferable_resources);

  
  
  
  void DeclareUsedResourcesFromChild(
      int child,
      const ResourceIdArray& resources_from_child);

  
  
  
  
  void ReceiveReturnsFromParent(
      const ReturnedResourceArray& transferable_resources);

  
  
  
  
  class CC_EXPORT ScopedReadLockGL {
   public:
    ScopedReadLockGL(ResourceProvider* resource_provider,
                     ResourceProvider::ResourceId resource_id);
    virtual ~ScopedReadLockGL();

    unsigned texture_id() const { return texture_id_; }

   protected:
    ResourceProvider* resource_provider_;
    ResourceProvider::ResourceId resource_id_;

   private:
    unsigned texture_id_;

    DISALLOW_COPY_AND_ASSIGN(ScopedReadLockGL);
  };

  class CC_EXPORT ScopedSamplerGL : public ScopedReadLockGL {
   public:
    ScopedSamplerGL(ResourceProvider* resource_provider,
                    ResourceProvider::ResourceId resource_id,
                    GLenum filter);
    ScopedSamplerGL(ResourceProvider* resource_provider,
                    ResourceProvider::ResourceId resource_id,
                    GLenum unit,
                    GLenum filter);
    virtual ~ScopedSamplerGL();

    GLenum target() const { return target_; }

   private:
    GLenum unit_;
    GLenum target_;

    DISALLOW_COPY_AND_ASSIGN(ScopedSamplerGL);
  };

  class CC_EXPORT ScopedWriteLockGL {
   public:
    ScopedWriteLockGL(ResourceProvider* resource_provider,
                      ResourceProvider::ResourceId resource_id);
    ~ScopedWriteLockGL();

    unsigned texture_id() const { return texture_id_; }

   private:
    ResourceProvider* resource_provider_;
    ResourceProvider::ResourceId resource_id_;
    unsigned texture_id_;

    DISALLOW_COPY_AND_ASSIGN(ScopedWriteLockGL);
  };

  class CC_EXPORT ScopedReadLockSoftware {
   public:
    ScopedReadLockSoftware(ResourceProvider* resource_provider,
                           ResourceProvider::ResourceId resource_id);
    ~ScopedReadLockSoftware();

    const SkBitmap* sk_bitmap() const { return &sk_bitmap_; }
    GLint wrap_mode() const { return wrap_mode_; }

   private:
    ResourceProvider* resource_provider_;
    ResourceProvider::ResourceId resource_id_;
    SkBitmap sk_bitmap_;
    GLint wrap_mode_;

    DISALLOW_COPY_AND_ASSIGN(ScopedReadLockSoftware);
  };

  class CC_EXPORT ScopedWriteLockSoftware {
   public:
    ScopedWriteLockSoftware(ResourceProvider* resource_provider,
                            ResourceProvider::ResourceId resource_id);
    ~ScopedWriteLockSoftware();

    SkCanvas* sk_canvas() { return sk_canvas_.get(); }

   private:
    ResourceProvider* resource_provider_;
    ResourceProvider::ResourceId resource_id_;
    SkBitmap sk_bitmap_;
    scoped_ptr<SkCanvas> sk_canvas_;

    DISALLOW_COPY_AND_ASSIGN(ScopedWriteLockSoftware);
  };

  class Fence : public base::RefCounted<Fence> {
   public:
    Fence() {}
    virtual bool HasPassed() = 0;

   protected:
    friend class base::RefCounted<Fence>;
    virtual ~Fence() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Fence);
  };

  
  
  void AcquirePixelBuffer(ResourceId id);
  void ReleasePixelBuffer(ResourceId id);

  
  uint8_t* MapPixelBuffer(ResourceId id);
  void UnmapPixelBuffer(ResourceId id);

  
  void BeginSetPixels(ResourceId id);
  void ForceSetPixelsToComplete(ResourceId id);
  bool DidSetPixelsComplete(ResourceId id);

  
  
  void AcquireImage(ResourceId id);
  void ReleaseImage(ResourceId id);

  
  
  uint8_t* MapImage(ResourceId id);
  void UnmapImage(ResourceId id);

  
  int GetImageStride(ResourceId id);

  base::SharedMemory* GetSharedMemory(ResourceId id);

  
  
  void AllocateForTesting(ResourceId id);

  
  void CreateForTesting(ResourceId id);

  GLenum TargetForTesting(ResourceId id);

  
  
  
  void SetReadLockFence(scoped_refptr<Fence> fence) {
    current_read_lock_fence_ = fence;
  }
  Fence* GetReadLockFence() { return current_read_lock_fence_.get(); }

  
  void EnableReadLockFences(ResourceProvider::ResourceId id, bool enable);

  
  bool CanLockForWrite(ResourceId id);

  static GLint GetActiveTextureUnit(gpu::gles2::GLES2Interface* gl);

 private:
  struct Resource {
    Resource();
    ~Resource();
    Resource(unsigned texture_id,
             gfx::Size size,
             GLenum target,
             GLenum filter,
             GLenum texture_pool,
             GLint wrap_mode,
             TextureUsageHint hint,
             ResourceFormat format);
    Resource(uint8_t* pixels,
             SharedBitmap* bitmap,
             gfx::Size size,
             GLenum filter,
             GLint wrap_mode);

    int child_id;
    unsigned gl_id;
    
    unsigned gl_pixel_buffer_id;
    
    unsigned gl_upload_query_id;
    TextureMailbox mailbox;
    ReleaseCallback release_callback;
    uint8_t* pixels;
    uint8_t* pixel_buffer;
    int lock_for_read_count;
    int imported_count;
    int exported_count;
    bool locked_for_write;
    bool external;
    bool marked_for_deletion;
    bool pending_set_pixels;
    bool set_pixels_completion_forced;
    bool allocated;
    bool enable_read_lock_fences;
    scoped_refptr<Fence> read_lock_fence;
    gfx::Size size;
    GLenum target;
    
    GLenum original_filter;
    GLenum filter;
    unsigned image_id;
    unsigned bound_image_id;
    bool dirty_image;
    GLenum texture_pool;
    GLint wrap_mode;
    bool lost;
    TextureUsageHint hint;
    ResourceType type;
    ResourceFormat format;
    SharedBitmap* shared_bitmap;
  };
  typedef base::hash_map<ResourceId, Resource> ResourceMap;

  static bool CompareResourceMapIteratorsByChildId(
      const std::pair<ReturnedResource, ResourceMap::iterator>& a,
      const std::pair<ReturnedResource, ResourceMap::iterator>& b);

  struct Child {
    Child();
    ~Child();

    ResourceIdMap child_to_parent_map;
    ResourceIdMap parent_to_child_map;
    ReturnCallback return_callback;
    ResourceIdSet in_use_resources;
    bool marked_for_deletion;
  };
  typedef base::hash_map<int, Child> ChildMap;

  bool ReadLockFenceHasPassed(Resource* resource) {
    return !resource->read_lock_fence.get() ||
           resource->read_lock_fence->HasPassed();
  }

  ResourceProvider(OutputSurface* output_surface,
                   SharedBitmapManager* shared_bitmap_manager,
                   int highp_threshold_min,
                   bool use_rgba_4444_texture_format,
                   size_t id_allocation_chunk_size);

  void CleanUpGLIfNeeded();

  Resource* GetResource(ResourceId id);
  const Resource* LockForRead(ResourceId id);
  void UnlockForRead(ResourceId id);
  const Resource* LockForWrite(ResourceId id);
  void UnlockForWrite(ResourceId id);
  static void PopulateSkBitmapWithResource(SkBitmap* sk_bitmap,
                                           const Resource* resource);

  void TransferResource(gpu::gles2::GLES2Interface* gl,
                        ResourceId id,
                        TransferableResource* resource);
  enum DeleteStyle {
    Normal,
    ForShutdown,
  };
  void DeleteResourceInternal(ResourceMap::iterator it, DeleteStyle style);
  void DeleteAndReturnUnusedResourcesToChild(ChildMap::iterator child_it,
                                             DeleteStyle style,
                                             const ResourceIdArray& unused);
  void DestroyChildInternal(ChildMap::iterator it, DeleteStyle style);
  void LazyCreate(Resource* resource);
  void LazyAllocate(Resource* resource);

  
  
  
  GLenum BindForSampling(ResourceProvider::ResourceId resource_id,
                         GLenum unit,
                         GLenum filter);

  
  gpu::gles2::GLES2Interface* ContextGL() const;

  OutputSurface* output_surface_;
  SharedBitmapManager* shared_bitmap_manager_;
  bool lost_output_surface_;
  int highp_threshold_min_;
  ResourceId next_id_;
  ResourceMap resources_;
  int next_child_;
  ChildMap children_;

  ResourceType default_resource_type_;
  bool use_texture_storage_ext_;
  bool use_texture_usage_hint_;
  bool use_compressed_texture_etc1_;
  scoped_ptr<TextureUploader> texture_uploader_;
  int max_texture_size_;
  ResourceFormat best_texture_format_;

  base::ThreadChecker thread_checker_;

  scoped_refptr<Fence> current_read_lock_fence_;
  bool use_rgba_4444_texture_format_;

  const size_t id_allocation_chunk_size_;
  scoped_ptr<IdAllocator> texture_id_allocator_;
  scoped_ptr<IdAllocator> buffer_id_allocator_;

  DISALLOW_COPY_AND_ASSIGN(ResourceProvider);
};


inline unsigned BitsPerPixel(ResourceFormat format) {
  DCHECK_LE(format, RESOURCE_FORMAT_MAX);
  static const unsigned format_bits_per_pixel[RESOURCE_FORMAT_MAX + 1] = {
    32,  
    16,  
    32,  
    8,   
    16,  
    4    
  };
  return format_bits_per_pixel[format];
}

inline GLenum GLDataType(ResourceFormat format) {
  DCHECK_LE(format, RESOURCE_FORMAT_MAX);
  static const unsigned format_gl_data_type[RESOURCE_FORMAT_MAX + 1] = {
    GL_UNSIGNED_BYTE,           
    GL_UNSIGNED_SHORT_4_4_4_4,  
    GL_UNSIGNED_BYTE,           
    GL_UNSIGNED_BYTE,           
    GL_UNSIGNED_SHORT_5_6_5,    
    GL_UNSIGNED_BYTE            
  };
  return format_gl_data_type[format];
}

inline GLenum GLDataFormat(ResourceFormat format) {
  DCHECK_LE(format, RESOURCE_FORMAT_MAX);
  static const unsigned format_gl_data_format[RESOURCE_FORMAT_MAX + 1] = {
    GL_RGBA,           
    GL_RGBA,           
    GL_BGRA_EXT,       
    GL_LUMINANCE,      
    GL_RGB,            
    GL_ETC1_RGB8_OES   
  };
  return format_gl_data_format[format];
}

inline GLenum GLInternalFormat(ResourceFormat format) {
  return GLDataFormat(format);
}

}  

#endif  
