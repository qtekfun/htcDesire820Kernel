// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_RENDERBUFFER_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_RENDERBUFFER_MANAGER_H_

#include <string>
#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/command_buffer/service/memory_tracking.h"
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

class RenderbufferManager;

class GPU_EXPORT Renderbuffer
    : public base::RefCounted<Renderbuffer> {
 public:
  Renderbuffer(RenderbufferManager* manager,
               GLuint client_id,
               GLuint service_id);

  GLuint service_id() const {
    return service_id_;
  }

  GLuint client_id() const {
    return client_id_;
  }

  bool cleared() const {
    return cleared_;
  }

  GLenum internal_format() const {
    return internal_format_;
  }

  GLsizei samples() const {
    return samples_;
  }

  GLsizei width() const {
    return width_;
  }

  GLsizei height() const {
    return height_;
  }

  bool IsDeleted() const {
    return client_id_ == 0;
  }

  void MarkAsValid() {
    has_been_bound_ = true;
  }

  bool IsValid() const {
    return has_been_bound_ && !IsDeleted();
  }

  size_t EstimatedSize();

  void AddToSignature(std::string* signature) const;

 private:
  friend class RenderbufferManager;
  friend class base::RefCounted<Renderbuffer>;

  ~Renderbuffer();

  void set_cleared(bool cleared) {
    cleared_ = cleared;
  }

  void SetInfo(
      GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
    samples_ = samples;
    internal_format_ = internalformat;
    width_ = width;
    height_ = height;
    cleared_ = false;
  }

  void MarkAsDeleted() {
    client_id_ = 0;
  }

  
  RenderbufferManager* manager_;

  
  GLuint client_id_;

  
  GLuint service_id_;

  
  bool cleared_;

  
  bool has_been_bound_;

  
  GLsizei samples_;

  
  GLenum internal_format_;

  
  GLsizei width_;
  GLsizei height_;
};

class GPU_EXPORT RenderbufferManager {
 public:
  RenderbufferManager(MemoryTracker* memory_tracker,
                      GLint max_renderbuffer_size,
                      GLint max_samples,
                      bool depth24_supported);
  ~RenderbufferManager();

  GLint max_renderbuffer_size() const {
    return max_renderbuffer_size_;
  }

  GLint max_samples() const {
    return max_samples_;
  }

  bool HaveUnclearedRenderbuffers() const {
    return num_uncleared_renderbuffers_ != 0;
  }

  void SetInfo(
      Renderbuffer* renderbuffer,
      GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

  void SetCleared(Renderbuffer* renderbuffer, bool cleared);

  
  void Destroy(bool have_context);

  
  void CreateRenderbuffer(GLuint client_id, GLuint service_id);

  
  Renderbuffer* GetRenderbuffer(GLuint client_id);

  
  void RemoveRenderbuffer(GLuint client_id);

  size_t mem_represented() const {
    return memory_tracker_->GetMemRepresented();
  }

  bool ComputeEstimatedRenderbufferSize(int width,
                                        int height,
                                        int samples,
                                        int internal_format,
                                        uint32* size) const;
  GLenum InternalRenderbufferFormatToImplFormat(GLenum impl_format) const;

 private:
  friend class Renderbuffer;

  void StartTracking(Renderbuffer* renderbuffer);
  void StopTracking(Renderbuffer* renderbuffer);

  scoped_ptr<MemoryTypeTracker> memory_tracker_;

  GLint max_renderbuffer_size_;
  GLint max_samples_;
  bool depth24_supported_;

  int num_uncleared_renderbuffers_;

  
  
  unsigned renderbuffer_count_;

  bool have_context_;

  
  typedef base::hash_map<GLuint, scoped_refptr<Renderbuffer> > RenderbufferMap;
  RenderbufferMap renderbuffers_;

  DISALLOW_COPY_AND_ASSIGN(RenderbufferManager);
};

}  
}  

#endif  
