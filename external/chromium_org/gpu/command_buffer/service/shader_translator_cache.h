// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_SHADER_TRANSLATOR_CACHE_H_
#define GPU_COMMAND_BUFFER_SERVICE_SHADER_TRANSLATOR_CACHE_H_

#include <string.h>

#include <map>

#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "gpu/command_buffer/service/shader_translator.h"
#include "third_party/angle/include/GLSLANG/ShaderLang.h"

namespace gpu {
namespace gles2 {

class ShaderTranslatorCache : public ShaderTranslator::DestructionObserver {
 public:
  static ShaderTranslatorCache* GetInstance();

  
  virtual void OnDestruct(ShaderTranslator* translator) OVERRIDE;

  scoped_refptr<ShaderTranslator> GetTranslator(
      ShShaderType shader_type,
      ShShaderSpec shader_spec,
      const ShBuiltInResources* resources,
      ShaderTranslatorInterface::GlslImplementationType
          glsl_implementation_type,
      ShCompileOptions driver_bug_workarounds);

 private:
  ShaderTranslatorCache();
  virtual ~ShaderTranslatorCache();

  friend struct DefaultSingletonTraits<ShaderTranslatorCache>;

  
  struct ShaderTranslatorInitParams {
    ShShaderType shader_type;
    ShShaderSpec shader_spec;
    ShBuiltInResources resources;
    ShaderTranslatorInterface::GlslImplementationType
        glsl_implementation_type;
    ShCompileOptions driver_bug_workarounds;

    ShaderTranslatorInitParams(
        ShShaderType shader_type,
        ShShaderSpec shader_spec,
        const ShBuiltInResources& resources,
        ShaderTranslatorInterface::GlslImplementationType
            glsl_implementation_type,
        ShCompileOptions driver_bug_workarounds)
      : shader_type(shader_type),
        shader_spec(shader_spec),
        resources(resources),
        glsl_implementation_type(glsl_implementation_type),
        driver_bug_workarounds(driver_bug_workarounds) {
    }

    ShaderTranslatorInitParams(const ShaderTranslatorInitParams& params) {
      memcpy(this, &params, sizeof(*this));
    }

    bool operator== (const ShaderTranslatorInitParams& params) const {
      return memcmp(&params, this, sizeof(*this)) == 0;
    }

    bool operator< (const ShaderTranslatorInitParams& params) const {
      return memcmp(&params, this, sizeof(*this)) < 0;
    }
  };

  typedef std::map<ShaderTranslatorInitParams, ShaderTranslator* > Cache;
  Cache cache_;

  DISALLOW_COPY_AND_ASSIGN(ShaderTranslatorCache);
};

}  
}  

#endif  
