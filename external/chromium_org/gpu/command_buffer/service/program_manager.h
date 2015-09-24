// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_PROGRAM_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_PROGRAM_MANAGER_H_

#include <map>
#include <string>
#include <vector>
#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "gpu/command_buffer/service/common_decoder.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/command_buffer/service/gles2_cmd_decoder.h"
#include "gpu/command_buffer/service/shader_manager.h"
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

class ProgramCache;
class ProgramManager;
class Shader;
class ShaderManager;
class ShaderTranslator;

class GPU_EXPORT Program : public base::RefCounted<Program> {
 public:
  static const int kMaxAttachedShaders = 2;

  enum VaryingsPackingOption {
    kCountOnlyStaticallyUsed,
    kCountAll
  };

  struct UniformInfo {
    UniformInfo();
    UniformInfo(
        GLsizei _size, GLenum _type, GLint _fake_location_base,
        const std::string& _name);
    ~UniformInfo();

    bool IsValid() const {
      return size != 0;
    }

    bool IsSampler() const {
      return type == GL_SAMPLER_2D || type == GL_SAMPLER_2D_RECT_ARB ||
             type == GL_SAMPLER_CUBE || type == GL_SAMPLER_EXTERNAL_OES;
    }

    GLsizei size;
    GLenum type;
    GLint fake_location_base;
    bool is_array;
    std::string name;
    std::vector<GLint> element_locations;
    std::vector<GLuint> texture_units;
  };
  struct VertexAttrib {
    VertexAttrib(GLsizei _size, GLenum _type, const std::string& _name,
                     GLint _location)
        : size(_size),
          type(_type),
          location(_location),
          name(_name) {
    }
    GLsizei size;
    GLenum type;
    GLint location;
    std::string name;
  };

  typedef std::vector<UniformInfo> UniformInfoVector;
  typedef std::vector<VertexAttrib> AttribInfoVector;
  typedef std::vector<int> SamplerIndices;
  typedef std::map<std::string, GLint> LocationMap;

  Program(ProgramManager* manager, GLuint service_id);

  GLuint service_id() const {
    return service_id_;
  }

  const SamplerIndices& sampler_indices() {
    return sampler_indices_;
  }

  const AttribInfoVector& GetAttribInfos() const {
    return attrib_infos_;
  }

  const VertexAttrib* GetAttribInfo(GLint index) const {
    return (static_cast<size_t>(index) < attrib_infos_.size()) ?
       &attrib_infos_[index] : NULL;
  }

  GLint GetAttribLocation(const std::string& name) const;

  const VertexAttrib* GetAttribInfoByLocation(GLuint location) const {
    if (location < attrib_location_to_index_map_.size()) {
      GLint index = attrib_location_to_index_map_[location];
      if (index >= 0) {
        return &attrib_infos_[index];
      }
    }
    return NULL;
  }

  const UniformInfo* GetUniformInfo(GLint index) const;

  
  const std::string* GetAttribMappedName(
      const std::string& original_name) const;

  
  const std::string* GetOriginalNameFromHashedName(
      const std::string& hashed_name) const;

  
  GLint GetUniformFakeLocation(const std::string& name) const;

  
  const UniformInfo* GetUniformInfoByFakeLocation(
      GLint fake_location, GLint* real_location, GLint* array_index) const;

  
  void GetProgramInfo(
      ProgramManager* manager, CommonDecoder::Bucket* bucket) const;

  
  
  
  
  
  bool SetSamplers(
      GLint num_texture_units, GLint fake_location,
      GLsizei count, const GLint* value);

  bool IsDeleted() const {
    return deleted_;
  }

  void GetProgramiv(GLenum pname, GLint* params);

  bool IsValid() const {
    return valid_;
  }

  bool AttachShader(ShaderManager* manager, Shader* shader);
  bool DetachShader(ShaderManager* manager, Shader* shader);

  bool CanLink() const;

  
  bool Link(ShaderManager* manager,
            ShaderTranslator* vertex_translator,
            ShaderTranslator* fragment_shader,
            VaryingsPackingOption varyings_packing_option,
            const ShaderCacheCallback& shader_callback);

  
  void Validate();

  const std::string* log_info() const {
    return log_info_.get();
  }

  bool InUse() const {
    DCHECK_GE(use_count_, 0);
    return use_count_ != 0;
  }

  
  void SetAttribLocationBinding(const std::string& attrib, GLint location) {
    bind_attrib_location_map_[attrib] = location;
  }

  
  
  bool SetUniformLocationBinding(const std::string& name, GLint location);

  
  
  
  bool DetectAttribLocationBindingConflicts() const;

  
  
  
  bool DetectUniformsMismatch() const;

  
  
  bool DetectVaryingsMismatch() const;

  
  bool DetectGlobalNameConflicts() const;

  
  
  bool CheckVaryingsPacking(VaryingsPackingOption option) const;

  
  const LocationMap& bind_attrib_location_map() const {
    return bind_attrib_location_map_;
  }

 private:
  friend class base::RefCounted<Program>;
  friend class ProgramManager;

  ~Program();

  void set_log_info(const char* str) {
    log_info_.reset(str ? new std::string(str) : NULL);
  }

  void ClearLinkStatus() {
    link_status_ = false;
  }

  void IncUseCount() {
    ++use_count_;
  }

  void DecUseCount() {
    --use_count_;
    DCHECK_GE(use_count_, 0);
  }

  void MarkAsDeleted() {
    DCHECK(!deleted_);
    deleted_ =  true;
  }

  
  void Reset();

  
  void Update();

  
  std::string ProcessLogInfo(const std::string& log);

  
  void UpdateLogInfo();

  
  void ClearUniforms(std::vector<uint8>* zero_buffer);

  
  
  
  
  void ExecuteBindAttribLocationCalls();

  bool AddUniformInfo(
      GLsizei size, GLenum type, GLint location, GLint fake_base_location,
      const std::string& name, const std::string& original_name,
      size_t* next_available_index);

  void GetCorrectedVariableInfo(
      bool use_uniforms, const std::string& name, std::string* corrected_name,
      std::string* original_name, GLsizei* size, GLenum* type) const;

  void DetachShaders(ShaderManager* manager);

  static inline GLint GetUniformInfoIndexFromFakeLocation(
      GLint fake_location) {
    return fake_location & 0xFFFF;
  }

  static inline GLint GetArrayElementIndexFromFakeLocation(
      GLint fake_location) {
    return (fake_location >> 16) & 0xFFFF;
  }

  ProgramManager* manager_;

  int use_count_;

  GLsizei max_attrib_name_length_;

  
  AttribInfoVector attrib_infos_;

  
  std::vector<GLint> attrib_location_to_index_map_;

  GLsizei max_uniform_name_length_;

  
  UniformInfoVector uniform_infos_;

  
  SamplerIndices sampler_indices_;

  
  GLuint service_id_;

  
  scoped_refptr<Shader>
      attached_shaders_[kMaxAttachedShaders];

  
  bool deleted_;

  
  bool valid_;

  
  bool link_status_;

  
  bool uniforms_cleared_;

  
  
  GLint num_uniforms_;

  
  scoped_ptr<std::string> log_info_;

  
  LocationMap bind_attrib_location_map_;

  
  LocationMap bind_uniform_location_map_;
};

class GPU_EXPORT ProgramManager {
 public:
  enum TranslatedShaderSourceType {
    kANGLE,
    kGL,  
  };

  explicit ProgramManager(ProgramCache* program_cache,
                          uint32 max_varying_vectors);
  ~ProgramManager();

  
  void Destroy(bool have_context);

  
  Program* CreateProgram(GLuint client_id, GLuint service_id);

  
  Program* GetProgram(GLuint client_id);

  
  bool GetClientId(GLuint service_id, GLuint* client_id) const;

  
  ProgramCache* program_cache() const;

  
  void MarkAsDeleted(ShaderManager* shader_manager, Program* program);

  
  void UseProgram(Program* program);

  
  void UnuseProgram(ShaderManager* shader_manager, Program* program);

  
  void ClearUniforms(Program* program);

  
  static bool IsInvalidPrefix(const char* name, size_t length);

  
  bool IsOwned(Program* program);

  static int32 MakeFakeLocation(int32 index, int32 element);

  void DoCompileShader(
      Shader* shader,
      ShaderTranslator* translator,
      TranslatedShaderSourceType translated_shader_source_type);

  uint32 max_varying_vectors() const {
    return max_varying_vectors_;
  }

 private:
  friend class Program;

  void StartTracking(Program* program);
  void StopTracking(Program* program);

  void RemoveProgramInfoIfUnused(
      ShaderManager* shader_manager, Program* program);

  
  
  typedef std::map<GLuint, scoped_refptr<Program> > ProgramMap;
  ProgramMap programs_;

  
  
  unsigned int program_count_;

  bool have_context_;

  
  std::vector<uint8> zero_;

  ProgramCache* program_cache_;

  uint32 max_varying_vectors_;

  DISALLOW_COPY_AND_ASSIGN(ProgramManager);
};

}  
}  

#endif  
