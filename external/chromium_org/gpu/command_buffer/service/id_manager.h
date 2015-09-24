// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_ID_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_ID_MANAGER_H_

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

class GPU_EXPORT IdManager {
 public:
  IdManager();
  ~IdManager();

  
  
  bool AddMapping(GLuint client_id, GLuint service_id);

  
  bool RemoveMapping(GLuint client_id, GLuint service_id);

  
  
  bool GetServiceId(GLuint client_id, GLuint* service_id);

  
  
  bool GetClientId(GLuint service_id, GLuint* client_id);

 private:
  typedef base::hash_map<GLuint, GLuint> MapType;
  MapType id_map_;

  DISALLOW_COPY_AND_ASSIGN(IdManager);
};

}  
}  

#endif  

