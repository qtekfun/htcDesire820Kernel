// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_COMMAND_BUFFER_SERVICE_ERROR_STATE_H_
#define GPU_COMMAND_BUFFER_SERVICE_ERROR_STATE_H_

#include "base/compiler_specific.h"
#include "gpu/command_buffer/common/types.h"
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

class Logger;


#define ERRORSTATE_SET_GL_ERROR(error_state, error, function_name, msg) \
    error_state->SetGLError(__FILE__, __LINE__, error, function_name, msg)

#define ERRORSTATE_SET_GL_ERROR_INVALID_ENUM( \
    error_state, function_name, value, label) \
    error_state->SetGLErrorInvalidEnum( \
        __FILE__, __LINE__, function_name, value, label)

#define ERRORSTATE_SET_GL_ERROR_INVALID_PARAM( \
    error_state, error, function_name, pname, param) \
    error_state->SetGLErrorInvalidParam( \
        __FILE__, __LINE__, error, function_name, pname, param)

#define ERRORSTATE_COPY_REAL_GL_ERRORS_TO_WRAPPER(error_state, function_name) \
    error_state->CopyRealGLErrorsToWrapper(__FILE__, __LINE__, function_name)
#define ERRORSTATE_PEEK_GL_ERROR(error_state, function_name) \
    error_state->PeekGLError(__FILE__, __LINE__, function_name)
#define ERRORSTATE_CLEAR_REAL_GL_ERRORS(error_state, function_name) \
    error_state->ClearRealGLErrors(__FILE__, __LINE__, function_name)


class GPU_EXPORT ErrorState {
 public:
  virtual ~ErrorState();

  static ErrorState* Create(Logger* logger);

  virtual uint32 GetGLError() = 0;

  virtual void SetGLError(
      const char* filename,
      int line,
      unsigned int error,
      const char* function_name,
      const char* msg) = 0;
  virtual void SetGLErrorInvalidEnum(
      const char* filename,
      int line,
      const char* function_name,
      unsigned int value,
      const char* label) = 0;
  virtual void SetGLErrorInvalidParam(
      const char* filename,
      int line,
      unsigned int error,
      const char* function_name,
      unsigned int pname,
      int param) = 0;

  
  
  virtual unsigned int PeekGLError(
      const char* filename, int line, const char* function_name) = 0;

  
  
  
  
  virtual void CopyRealGLErrorsToWrapper(
      const char* filename, int line, const char* function_name) = 0;

  
  
  virtual void ClearRealGLErrors(
      const char* filename, int line, const char* function_name) = 0;

 protected:
  ErrorState();

  DISALLOW_COPY_AND_ASSIGN(ErrorState);
};

}  
}  

#endif  

