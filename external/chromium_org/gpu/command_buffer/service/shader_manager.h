// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_SHADER_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_SHADER_MANAGER_H_

#include <string>
#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/command_buffer/service/shader_translator.h"
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

// to emluate GLES2 the shaders will have to be re-written before passed to
// should get the source they passed in, not the re-written source.
class GPU_EXPORT Shader : public base::RefCounted<Shader> {
 public:
  typedef ShaderTranslator::VariableInfo VariableInfo;

  void UpdateSource(const char* source) {
    source_.reset(source ? new std::string(source) : NULL);
  }

  void UpdateTranslatedSource(const char* translated_source) {
    translated_source_.reset(
        translated_source ? new std::string(translated_source) : NULL);
  }

  GLuint service_id() const {
    return service_id_;
  }

  GLenum shader_type() const {
    return shader_type_;
  }

  const std::string* source() const {
    return source_.get();
  }

  const std::string* translated_source() const {
    return translated_source_.get();
  }

  const std::string* signature_source() const {
    return signature_source_.get();
  }

  void SetStatus(
      bool valid, const char* log,
      ShaderTranslatorInterface* translator);

  const VariableInfo* GetAttribInfo(const std::string& name) const;
  const VariableInfo* GetUniformInfo(const std::string& name) const;

  
  const std::string* GetAttribMappedName(
      const std::string& original_name) const;

  
  const std::string* GetOriginalNameFromHashedName(
      const std::string& hashed_name) const;

  const std::string* log_info() const {
    return log_info_.get();
  }

  bool IsValid() const {
    return valid_;
  }

  bool IsDeleted() const {
    return service_id_ == 0;
  }

  bool InUse() const {
    DCHECK_GE(use_count_, 0);
    return use_count_ != 0;
  }

  
  const ShaderTranslator::VariableMap& attrib_map() const {
    return attrib_map_;
  }

  
  const ShaderTranslator::VariableMap& uniform_map() const {
    return uniform_map_;
  }

  
  const ShaderTranslator::VariableMap& varying_map() const {
    return varying_map_;
  }

  
  void set_attrib_map(const ShaderTranslator::VariableMap& attrib_map) {
    
    attrib_map_ = ShaderTranslator::VariableMap(attrib_map);
  }

  
  void set_uniform_map(const ShaderTranslator::VariableMap& uniform_map) {
    
    uniform_map_ = ShaderTranslator::VariableMap(uniform_map);
  }

  
  void set_varying_map(const ShaderTranslator::VariableMap& varying_map) {
    
    varying_map_ = ShaderTranslator::VariableMap(varying_map);
  }

 private:
  typedef ShaderTranslator::VariableMap VariableMap;
  typedef ShaderTranslator::NameMap NameMap;

  friend class base::RefCounted<Shader>;
  friend class ShaderManager;

  Shader(GLuint service_id, GLenum shader_type);
  ~Shader();

  void IncUseCount();
  void DecUseCount();
  void MarkAsDeleted();

  int use_count_;

  
  GLuint service_id_;
  
  GLenum shader_type_;

  
  bool valid_;

  
  scoped_ptr<std::string> source_;

  
  scoped_ptr<std::string> signature_source_;

  
  scoped_ptr<std::string> translated_source_;

  
  scoped_ptr<std::string> log_info_;

  
  VariableMap attrib_map_;
  VariableMap uniform_map_;
  VariableMap varying_map_;

  
  NameMap name_map_;
};

class GPU_EXPORT ShaderManager {
 public:
  ShaderManager();
  ~ShaderManager();

  
  void Destroy(bool have_context);

  
  Shader* CreateShader(
      GLuint client_id,
      GLuint service_id,
      GLenum shader_type);

  
  
  Shader* GetShader(GLuint client_id);

  
  bool GetClientId(GLuint service_id, GLuint* client_id) const;

  void MarkAsDeleted(Shader* shader);

  
  void UseShader(Shader* shader);

  
  
  void UnuseShader(Shader* shader);

  
  bool IsOwned(Shader* shader);

 private:
  friend class Shader;

  
  typedef base::hash_map<GLuint, scoped_refptr<Shader> > ShaderMap;
  ShaderMap shaders_;

  void RemoveShader(Shader* shader);

  DISALLOW_COPY_AND_ASSIGN(ShaderManager);
};

}  
}  

#endif  

