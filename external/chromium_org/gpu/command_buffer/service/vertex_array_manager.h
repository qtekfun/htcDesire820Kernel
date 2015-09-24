// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_VERTEX_ARRAY_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_VERTEX_ARRAY_MANAGER_H_

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

class VertexAttribManager;

class GPU_EXPORT VertexArrayManager {
 public:
  VertexArrayManager();
  ~VertexArrayManager();

  
  void Destroy(bool have_context);

  
  void CreateVertexAttribManager(GLuint client_id, GLuint service_id,
      uint32 num_vertex_attribs);

  
  VertexAttribManager* GetVertexAttribManager(GLuint client_id);

  
  void RemoveVertexAttribManager(GLuint client_id);

  
  bool GetClientId(GLuint service_id, GLuint* client_id) const;

 private:
  friend class VertexAttribManager;

  void StartTracking(VertexAttribManager* vertex_attrib_manager);
  void StopTracking(VertexAttribManager* vertex_attrib_manager);

  
  typedef base::hash_map<GLuint, scoped_refptr<VertexAttribManager> >
      VertexAttribManagerMap;
  VertexAttribManagerMap vertex_attrib_managers_;

  
  
  unsigned int vertex_attrib_manager_count_;

  bool have_context_;

  DISALLOW_COPY_AND_ASSIGN(VertexArrayManager);
};

}  
}  

#endif  
