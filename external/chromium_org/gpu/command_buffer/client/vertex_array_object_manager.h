// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_CLIENT_VERTEX_ARRAY_OBJECT_MANAGER_H_
#define GPU_COMMAND_BUFFER_CLIENT_VERTEX_ARRAY_OBJECT_MANAGER_H_

#include <GLES2/gl2.h>
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "gles2_impl_export.h"
#include "gpu/command_buffer/common/types.h"

namespace gpu {
namespace gles2 {

class GLES2Implementation;
class GLES2CmdHelper;
class VertexArrayObject;

class GLES2_IMPL_EXPORT VertexArrayObjectManager {
 public:
  VertexArrayObjectManager(
      GLuint max_vertex_attribs,
      GLuint array_buffer_id,
      GLuint element_array_buffer_id);
  ~VertexArrayObjectManager();

  bool IsReservedId(GLuint id) const;

  
  
  bool BindElementArray(GLuint id);

  
  void UnbindBuffer(GLuint id);

  
  void GenVertexArrays(GLsizei n, const GLuint* arrays);

  
  void DeleteVertexArrays(GLsizei n, const GLuint* arrays);

  
  
  
  bool BindVertexArray(GLuint array, bool* changed);

  
  
  bool SetupSimulatedClientSideBuffers(
      const char* function_name,
      GLES2Implementation* gl,
      GLES2CmdHelper* gl_helper,
      GLsizei num_elements,
      GLsizei primcount,
      bool* simulated);

  
  bool SetupSimulatedIndexAndClientSideBuffers(
      const char* function_name,
      GLES2Implementation* gl,
      GLES2CmdHelper* gl_helper,
      GLsizei count,
      GLenum type,
      GLsizei primcount,
      const void* indices,
      GLuint* offset,
      bool* simulated);

  bool HaveEnabledClientSideBuffers() const;

  void SetAttribEnable(GLuint index, bool enabled);

  bool GetVertexAttrib(GLuint index, GLenum pname, uint32* param);

  bool GetAttribPointer(GLuint index, GLenum pname, void** ptr) const;

  
  bool SetAttribPointer(
      GLuint buffer_id,
      GLuint index,
      GLint size,
      GLenum type,
      GLboolean normalized,
      GLsizei stride,
      const void* ptr);

  void SetAttribDivisor(GLuint index, GLuint divisor);

  GLuint bound_element_array_buffer() const;

 private:
  typedef base::hash_map<GLuint, VertexArrayObject*> VertexArrayObjectMap;

  bool IsDefaultVAOBound() const;

  GLsizei CollectData(const void* data,
                      GLsizei bytes_per_element,
                      GLsizei real_stride,
                      GLsizei num_elements);

  GLuint max_vertex_attribs_;
  GLuint array_buffer_id_;
  GLsizei array_buffer_size_;
  GLsizei array_buffer_offset_;
  GLuint element_array_buffer_id_;
  GLsizei element_array_buffer_size_;
  GLsizei collection_buffer_size_;
  scoped_ptr<int8[]> collection_buffer_;

  VertexArrayObject* default_vertex_array_object_;
  VertexArrayObject* bound_vertex_array_object_;
  VertexArrayObjectMap vertex_array_objects_;

  DISALLOW_COPY_AND_ASSIGN(VertexArrayObjectManager);
};

}  
}  

#endif  

