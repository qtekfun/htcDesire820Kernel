// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_PROGRAM_CACHE_H_
#define GPU_COMMAND_BUFFER_SERVICE_PROGRAM_CACHE_H_

#include <map>
#include <string>

#include "base/containers/hash_tables.h"
#include "base/sha1.h"
#include "gpu/command_buffer/common/gles2_cmd_format.h"
#include "gpu/command_buffer/service/gles2_cmd_decoder.h"
#include "gpu/command_buffer/service/shader_manager.h"

namespace gpu {
namespace gles2 {

class Shader;
class ShaderTranslator;

class GPU_EXPORT ProgramCache {
 public:
  static const size_t kHashLength = base::kSHA1Length;

  typedef std::map<std::string, GLint> LocationMap;

  enum LinkedProgramStatus {
    LINK_UNKNOWN,
    LINK_SUCCEEDED
  };

  enum ProgramLoadResult {
    PROGRAM_LOAD_FAILURE,
    PROGRAM_LOAD_SUCCESS
  };

  ProgramCache();
  virtual ~ProgramCache();

  LinkedProgramStatus GetLinkedProgramStatus(
      const std::string& untranslated_shader_a,
      const ShaderTranslatorInterface* translator_a,
      const std::string& untranslated_shader_b,
      const ShaderTranslatorInterface* translator_b,
      const LocationMap* bind_attrib_location_map) const;

  
  
  virtual ProgramLoadResult LoadLinkedProgram(
      GLuint program,
      Shader* shader_a,
      const ShaderTranslatorInterface* translator_a,
      Shader* shader_b,
      const ShaderTranslatorInterface* translator_b,
      const LocationMap* bind_attrib_location_map,
      const ShaderCacheCallback& shader_callback) = 0;

  
  
  virtual void SaveLinkedProgram(
      GLuint program,
      const Shader* shader_a,
      const ShaderTranslatorInterface* translator_a,
      const Shader* shader_b,
      const ShaderTranslatorInterface* translator_b,
      const LocationMap* bind_attrib_location_map,
      const ShaderCacheCallback& shader_callback) = 0;

  virtual void LoadProgram(const std::string& program) = 0;

  
  void Clear();

  
  void LinkedProgramCacheSuccess(const std::string& shader_a,
                                 const ShaderTranslatorInterface* translator_a,
                                 const std::string& shader_b,
                                 const ShaderTranslatorInterface* translator_b,
                                 const LocationMap* bind_attrib_location_map);

 protected:
  
  void LinkedProgramCacheSuccess(const std::string& program_hash);

  
  void ComputeShaderHash(const std::string& shader,
                         const ShaderTranslatorInterface* translator,
                         char* result) const;

  
  
  void ComputeProgramHash(
      const char* hashed_shader_0,
      const char* hashed_shader_1,
      const LocationMap* bind_attrib_location_map,
      char* result) const;

  void Evict(const std::string& program_hash);

 private:
  typedef base::hash_map<std::string,
                         LinkedProgramStatus> LinkStatusMap;

  
  virtual void ClearBackend() = 0;

  LinkStatusMap link_status_;

  DISALLOW_COPY_AND_ASSIGN(ProgramCache);
};

}  
}  

#endif  
