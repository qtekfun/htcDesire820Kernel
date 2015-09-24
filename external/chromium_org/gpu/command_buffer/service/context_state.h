// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_COMMAND_BUFFER_SERVICE_CONTEXT_STATE_H_
#define GPU_COMMAND_BUFFER_SERVICE_CONTEXT_STATE_H_

#include <vector>
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/command_buffer/service/query_manager.h"
#include "gpu/command_buffer/service/texture_manager.h"
#include "gpu/command_buffer/service/vertex_attrib_manager.h"
#include "gpu/command_buffer/service/vertex_array_manager.h"
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

class Buffer;
class ErrorState;
class FeatureInfo;
class Framebuffer;
class Program;
class Renderbuffer;

struct GPU_EXPORT TextureUnit {
  TextureUnit();
  ~TextureUnit();

  
  GLenum bind_target;

  
  scoped_refptr<TextureRef> bound_texture_2d;

  
  
  scoped_refptr<TextureRef> bound_texture_cube_map;

  
  
  scoped_refptr<TextureRef> bound_texture_external_oes;

  
  
  scoped_refptr<TextureRef> bound_texture_rectangle_arb;

  scoped_refptr<TextureRef> GetInfoForSamplerType(
      GLenum type) {
    DCHECK(type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE ||
           type == GL_SAMPLER_EXTERNAL_OES || type == GL_SAMPLER_2D_RECT_ARB);
    switch (type) {
      case GL_SAMPLER_2D:
        return bound_texture_2d;
      case GL_SAMPLER_CUBE:
        return bound_texture_cube_map;
      case GL_SAMPLER_EXTERNAL_OES:
        return bound_texture_external_oes;
      case GL_SAMPLER_2D_RECT_ARB:
        return bound_texture_rectangle_arb;
    }

    NOTREACHED();
    return NULL;
  }

  void Unbind(TextureRef* texture) {
    if (bound_texture_2d.get() == texture) {
      bound_texture_2d = NULL;
    }
    if (bound_texture_cube_map.get() == texture) {
      bound_texture_cube_map = NULL;
    }
    if (bound_texture_external_oes.get() == texture) {
      bound_texture_external_oes = NULL;
    }
  }
};

struct Vec4 {
  Vec4() {
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 0.0f;
    v[3] = 1.0f;
  }
  float v[4];
};

struct GPU_EXPORT ContextState {
  ContextState(FeatureInfo* feature_info, Logger* logger);
  ~ContextState();

  void Initialize();

  void RestoreState() const;
  void InitCapabilities() const;
  void InitState() const;

  void RestoreActiveTexture() const;
  void RestoreAllTextureUnitBindings() const;
  void RestoreAttribute(GLuint index) const;
  void RestoreBufferBindings() const;
  void RestoreGlobalState() const;
  void RestoreProgramBindings() const;
  void RestoreRenderbufferBindings() const;
  void RestoreTextureUnitBindings(GLuint unit) const;

  
  bool GetStateAsGLint(
      GLenum pname, GLint* params, GLsizei* num_written) const;
  bool GetStateAsGLfloat(
      GLenum pname, GLfloat* params, GLsizei* num_written) const;
  bool GetEnabled(GLenum cap) const;

  ErrorState* GetErrorState();

  #include "gpu/command_buffer/service/context_state_autogen.h"

  EnableFlags enable_flags;

  
  
  
  GLuint active_texture_unit;

  
  
  scoped_refptr<Buffer> bound_array_buffer;

  
  std::vector<TextureUnit> texture_units;

  
  std::vector<Vec4> attrib_values;

  
  scoped_refptr<VertexAttribManager> vertex_attrib_manager;

  
  scoped_refptr<Program> current_program;

  
  scoped_refptr<Renderbuffer> bound_renderbuffer;

  scoped_refptr<QueryManager::Query> current_query;

  bool pack_reverse_row_order;

  mutable bool fbo_binding_for_scissor_workaround_dirty_;
  FeatureInfo* feature_info_;

 private:
  scoped_ptr<ErrorState> error_state_;
};

}  
}  

#endif  

