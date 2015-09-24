// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_BUFFER_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_BUFFER_MANAGER_H_

#include <map>
#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/command_buffer/service/memory_tracking.h"
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

class BufferManager;
struct ContextState;
class ErrorState;
class FeatureInfo;
class TestHelper;

class GPU_EXPORT Buffer : public base::RefCounted<Buffer> {
 public:
  Buffer(BufferManager* manager, GLuint service_id);

  GLuint service_id() const {
    return service_id_;
  }

  GLenum target() const {
    return target_;
  }

  GLsizeiptr size() const {
    return size_;
  }

  GLenum usage() const {
    return usage_;
  }

  
  
  
  
  bool GetMaxValueForRange(GLuint offset, GLsizei count, GLenum type,
                           GLuint* max_value);

  
  const void* GetRange(GLintptr offset, GLsizeiptr size) const;

  bool IsDeleted() const {
    return deleted_;
  }

  bool IsValid() const {
    return target() && !IsDeleted();
  }

  bool IsClientSideArray() const {
    return is_client_side_array_;
  }

 private:
  friend class BufferManager;
  friend class BufferManagerTestBase;
  friend class base::RefCounted<Buffer>;

  
  class Range {
   public:
    Range(GLuint offset, GLsizei count, GLenum type)
        : offset_(offset),
          count_(count),
          type_(type) {
    }

    
    struct Less {
      bool operator() (const Range& lhs, const Range& rhs) const {
        if (lhs.offset_ != rhs.offset_) {
          return lhs.offset_ < rhs.offset_;
        }
        if (lhs.count_ != rhs.count_) {
          return lhs.count_ < rhs.count_;
        }
        return lhs.type_ < rhs.type_;
      }
    };

   private:
    GLuint offset_;
    GLsizei count_;
    GLenum type_;
  };

  ~Buffer();

  void set_target(GLenum target) {
    DCHECK_EQ(target_, 0u);  
    target_ = target;
  }

  bool shadowed() const {
    return shadowed_;
  }

  void MarkAsDeleted() {
    deleted_ = true;
  }

  
  
  void SetInfo(
      GLsizeiptr size, GLenum usage, bool shadow, const GLvoid* data,
      bool is_client_side_array);

  
  
  bool SetRange(
    GLintptr offset, GLsizeiptr size, const GLvoid * data);

  
  void ClearCache();

  
  bool CheckRange(GLintptr offset, GLsizeiptr size) const;

  
  BufferManager* manager_;

  
  
  scoped_ptr<int8[]> shadow_;

  
  GLsizeiptr size_;

  
  bool deleted_;

  
  bool shadowed_;

  
  
  bool is_client_side_array_;

  
  GLuint service_id_;

  
  
  
  GLenum target_;

  
  GLenum usage_;

  
  typedef std::map<Range, GLuint, Range::Less> RangeToMaxValueMap;
  RangeToMaxValueMap range_set_;
};

class GPU_EXPORT BufferManager {
 public:
  BufferManager(MemoryTracker* memory_tracker, FeatureInfo* feature_info);
  ~BufferManager();

  
  void Destroy(bool have_context);

  
  void CreateBuffer(GLuint client_id, GLuint service_id);

  
  Buffer* GetBuffer(GLuint client_id);

  
  void RemoveBuffer(GLuint client_id);

  
  bool GetClientId(GLuint service_id, GLuint* client_id) const;

  
  
  void ValidateAndDoBufferSubData(
      ContextState* context_state, GLenum target, GLintptr offset,
      GLsizeiptr size, const GLvoid * data);

  
  
  void ValidateAndDoBufferData(
    ContextState* context_state, GLenum target, GLsizeiptr size,
    const GLvoid * data, GLenum usage);

  
  
  void ValidateAndDoGetBufferParameteriv(
    ContextState* context_state, GLenum target, GLenum pname, GLint* params);

  
  bool SetTarget(Buffer* buffer, GLenum target);

  void set_allow_buffers_on_multiple_targets(bool allow) {
    allow_buffers_on_multiple_targets_ = allow;
  }

  size_t mem_represented() const {
    return memory_tracker_->GetMemRepresented();
  }

  
  bool IsUsageClientSideArray(GLenum usage);

  
  
  bool UseNonZeroSizeForClientSideArrayBuffer();

 private:
  friend class Buffer;
  friend class TestHelper;  
  friend class BufferManagerTestBase;  
  void StartTracking(Buffer* buffer);
  void StopTracking(Buffer* buffer);

  Buffer* GetBufferInfoForTarget(ContextState* state, GLenum target);

  
  
  void DoBufferSubData(
      ErrorState* error_state,
      Buffer* buffer,
      GLintptr offset,
      GLsizeiptr size,
      const GLvoid* data);

  
  
  void DoBufferData(
      ErrorState* error_state,
      Buffer* buffer,
      GLsizeiptr size,
      GLenum usage,
      const GLvoid* data);

  
  
  void SetInfo(
      Buffer* buffer, GLsizeiptr size, GLenum usage, const GLvoid* data);

  scoped_ptr<MemoryTypeTracker> memory_tracker_;
  scoped_refptr<FeatureInfo> feature_info_;

  
  typedef base::hash_map<GLuint, scoped_refptr<Buffer> > BufferMap;
  BufferMap buffers_;

  
  bool allow_buffers_on_multiple_targets_;

  
  
  unsigned int buffer_count_;

  bool have_context_;
  bool use_client_side_arrays_for_stream_buffers_;

  DISALLOW_COPY_AND_ASSIGN(BufferManager);
};

}  
}  

#endif  
