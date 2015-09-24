// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_VERTEX_ATTRIB_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_VERTEX_ATTRIB_MANAGER_H_

#include <list>
#include <vector>
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "build/build_config.h"
#include "gpu/command_buffer/service/buffer_manager.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

class FeatureInfo;
class GLES2Decoder;
class Program;
class VertexArrayManager;

class GPU_EXPORT VertexAttrib {
 public:
  typedef std::list<VertexAttrib*> VertexAttribList;

  VertexAttrib();
  ~VertexAttrib();

  
  bool CanAccess(GLuint index) const;

  Buffer* buffer() const { return buffer_.get(); }

  GLsizei offset() const {
    return offset_;
  }

  GLuint index() const {
    return index_;
  }

  GLint size() const {
    return size_;
  }

  GLenum type() const {
    return type_;
  }

  GLboolean normalized() const {
    return normalized_;
  }

  GLsizei gl_stride() const {
    return gl_stride_;
  }

  GLuint divisor() const {
    return divisor_;
  }

  bool enabled() const {
    return enabled_;
  }

  
  GLuint MaxVertexAccessed(GLsizei primcount,
                           GLuint max_vertex_accessed) const {
    return (primcount && divisor_) ? ((primcount - 1) / divisor_) :
                                     max_vertex_accessed;
  }

  bool is_client_side_array() const {
    return is_client_side_array_;
  }

  void set_is_client_side_array(bool value) {
    is_client_side_array_ = value;
  }

 private:
  friend class VertexAttribManager;

  void set_enabled(bool enabled) {
    enabled_ = enabled;
  }

  void set_index(GLuint index) {
    index_ = index;
  }

  void SetList(VertexAttribList* new_list) {
    DCHECK(new_list);

    if (list_) {
      list_->erase(it_);
    }

    it_ = new_list->insert(new_list->end(), this);
    list_ = new_list;
  }

  void SetInfo(
      Buffer* buffer,
      GLint size,
      GLenum type,
      GLboolean normalized,
      GLsizei gl_stride,
      GLsizei real_stride,
      GLsizei offset);

  void SetDivisor(GLsizei divisor) {
    divisor_ = divisor;
  }

  void Unbind(Buffer* buffer);

  
  GLuint index_;

  
  bool enabled_;

  
  GLint size_;

  
  GLenum type_;

  
  GLsizei offset_;

  GLboolean normalized_;

  
  GLsizei gl_stride_;

  
  
  
  GLsizei real_stride_;

  GLsizei divisor_;

  
  bool is_client_side_array_;

  
  scoped_refptr<Buffer> buffer_;

  
  VertexAttribList* list_;

  
  VertexAttribList::iterator it_;
};

class GPU_EXPORT VertexAttribManager :
    public base::RefCounted<VertexAttribManager> {
 public:
  typedef std::list<VertexAttrib*> VertexAttribList;

  VertexAttribManager();

  void Initialize(uint32 num_vertex_attribs, bool init_attribs = true);

  bool Enable(GLuint index, bool enable);

  bool HaveFixedAttribs() const {
    return num_fixed_attribs_ != 0;
  }

  const VertexAttribList& GetEnabledVertexAttribs() const {
    return enabled_vertex_attribs_;
  }

  VertexAttrib* GetVertexAttrib(GLuint index) {
    if (index < vertex_attribs_.size()) {
      return &vertex_attribs_[index];
    }
    return NULL;
  }

  void SetAttribInfo(
      GLuint index,
      Buffer* buffer,
      GLint size,
      GLenum type,
      GLboolean normalized,
      GLsizei gl_stride,
      GLsizei real_stride,
      GLsizei offset) {
    VertexAttrib* attrib = GetVertexAttrib(index);
    if (attrib) {
      if (attrib->type() == GL_FIXED) {
        --num_fixed_attribs_;
      }
      if (type == GL_FIXED) {
        ++num_fixed_attribs_;
      }
      attrib->SetInfo(
          buffer, size, type, normalized, gl_stride, real_stride, offset);
    }
  }

  void SetDivisor(GLuint index, GLuint divisor) {
    VertexAttrib* attrib = GetVertexAttrib(index);
    if (attrib) {
      attrib->SetDivisor(divisor);
    }
  }

  void SetElementArrayBuffer(Buffer* buffer);

  Buffer* element_array_buffer() const { return element_array_buffer_.get(); }

  GLuint service_id() const {
    return service_id_;
  }

  void Unbind(Buffer* buffer);

  bool IsDeleted() const {
    return deleted_;
  }

  bool IsValid() const {
    return !IsDeleted();
  }

  size_t num_attribs() const {
    return vertex_attribs_.size();
  }

  bool ValidateBindings(
      const char* function_name,
      GLES2Decoder* decoder,
      FeatureInfo* feature_info,
      Program* current_program,
      GLuint max_vertex_accessed,
      GLsizei primcount);

 private:
  friend class VertexArrayManager;
  friend class VertexArrayManagerTest;
  friend class base::RefCounted<VertexAttribManager>;

  
  VertexAttribManager(VertexArrayManager* manager, GLuint service_id,
      uint32 num_vertex_attribs);

  ~VertexAttribManager();

  void MarkAsDeleted() {
    deleted_ = true;
  }

  
  int num_fixed_attribs_;

  
  
  std::vector<VertexAttrib> vertex_attribs_;

  
  
  scoped_refptr<Buffer> element_array_buffer_;

  
  VertexAttribList enabled_vertex_attribs_;
  VertexAttribList disabled_vertex_attribs_;

  
  VertexArrayManager* manager_;

  
  bool deleted_;

  
  GLuint service_id_;
};

}  
}  

#endif  

