// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_CLIENT_GL_HELPER_SCALING_H_
#define CONTENT_COMMON_GPU_CLIENT_GL_HELPER_SCALING_H_

#include <vector>

#include "content/common/gpu/client/gl_helper.h"

namespace content {

class ShaderProgram;
class ScalerImpl;
class GLHelperTest;

class CONTENT_EXPORT GLHelperScaling {
 public:
  enum ShaderType {
    SHADER_BILINEAR,
    SHADER_BILINEAR2,
    SHADER_BILINEAR3,
    SHADER_BILINEAR4,
    SHADER_BILINEAR2X2,
    SHADER_BICUBIC_UPSCALE,
    SHADER_BICUBIC_HALF_1D,
    SHADER_PLANAR,
    SHADER_YUV_MRT_PASS1,
    SHADER_YUV_MRT_PASS2,
  };

  
  
  class CONTENT_EXPORT ShaderInterface {
   public:
    ShaderInterface() {}
    virtual ~ShaderInterface() {}
    
    
    virtual void Execute(blink::WebGLId source_texture,
                         const std::vector<blink::WebGLId>& dest_textures) = 0;
  };

  typedef std::pair<ShaderType, bool> ShaderProgramKeyType;

  GLHelperScaling(blink::WebGraphicsContext3D* context,
                  GLHelper* helper);
  ~GLHelperScaling();
  void InitBuffer();

  GLHelper::ScalerInterface* CreateScaler(
      GLHelper::ScalerQuality quality,
      gfx::Size src_size,
      gfx::Rect src_subrect,
      const gfx::Size& dst_size,
      bool vertically_flip_texture,
      bool swizzle);

  GLHelper::ScalerInterface* CreatePlanarScaler(
      const gfx::Size& src_size,
      const gfx::Rect& src_subrect,
      const gfx::Size& dst_size,
      bool vertically_flip_texture,
      const float color_weights[4]);

  ShaderInterface* CreateYuvMrtShader(
      const gfx::Size& src_size,
      const gfx::Rect& src_subrect,
      const gfx::Size& dst_size,
      bool vertically_flip_texture,
      ShaderType shader);

 private:
  
  
  
  
  struct ScaleOp {
    ScaleOp(int factor, bool x, int size)
        : scale_factor(factor), scale_x(x), scale_size(size) {
    }

    
    
    
    
    
    
    static void AddOps(int src,
                       int dst,
                       bool scale_x,
                       bool allow3,
                       std::deque<ScaleOp>* ops) {
      int num_downscales = 0;
      if (allow3 && dst * 3 >= src && dst * 2 < src) {
        
        
        
        ops->push_back(ScaleOp(3, scale_x, dst));
        return;
      }
      while ((dst << num_downscales) < src) {
        num_downscales++;
      }
      if ((dst << num_downscales) != src) {
        ops->push_back(ScaleOp(0, scale_x, dst << num_downscales));
      }
      while (num_downscales) {
        num_downscales--;
        ops->push_back(ScaleOp(2, scale_x, dst << num_downscales));
      }
    }

    
    
    
    
    void UpdateSize(gfx::Size* subrect) {
      if (scale_x) {
        subrect->set_width(scale_size);
      } else {
        subrect->set_height(scale_size);
      }
    }

    
    
    
    int scale_factor;
    bool scale_x;  
    int scale_size;  
  };

  
  struct ScalerStage {
    ScalerStage(ShaderType shader_,
                gfx::Size src_size_,
                gfx::Rect src_subrect_,
                gfx::Size dst_size_,
                bool scale_x_,
                bool vertically_flip_texture_,
                bool swizzle_);
    ShaderType shader;
    gfx::Size src_size;
    gfx::Rect src_subrect;
    gfx::Size dst_size;
    bool scale_x;
    bool vertically_flip_texture;
    bool swizzle;
  };

  
  
  void ComputeScalerStages(GLHelper::ScalerQuality quality,
                           const gfx::Size& src_size,
                           const gfx::Rect& src_subrect,
                           const gfx::Size& dst_size,
                           bool vertically_flip_texture,
                           bool swizzle,
                           std::vector<ScalerStage> *scaler_stages);

  
  
  
  void ConvertScalerOpsToScalerStages(
      GLHelper::ScalerQuality quality,
      gfx::Size src_size,
      gfx::Rect src_subrect,
      const gfx::Size& dst_size,
      bool vertically_flip_texture,
      bool swizzle,
      std::deque<GLHelperScaling::ScaleOp>* x_ops,
      std::deque<GLHelperScaling::ScaleOp>* y_ops,
      std::vector<ScalerStage> *scaler_stages);


  scoped_refptr<ShaderProgram> GetShaderProgram(ShaderType type, bool swizzle);

  
  
  static const blink::WGC3Dfloat kVertexAttributes[];

  blink::WebGraphicsContext3D* context_;
  GLHelper* helper_;

  
  
  ScopedBuffer vertex_attributes_buffer_;

  std::map<ShaderProgramKeyType,
           scoped_refptr<ShaderProgram> > shader_programs_;

  friend class ShaderProgram;
  friend class ScalerImpl;
  friend class GLHelperTest;
  DISALLOW_COPY_AND_ASSIGN(GLHelperScaling);
};


}  

#endif  
