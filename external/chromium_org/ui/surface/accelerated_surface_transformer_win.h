// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SURFACE_ACCELERATED_SURFACE_TRANSFORMER_WIN_H_
#define UI_SURFACE_ACCELERATED_SURFACE_TRANSFORMER_WIN_H_

#include <d3d9.h>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/single_thread_task_runner.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "base/win/scoped_comptr.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/surface/surface_export.h"

namespace gfx {
class Size;
class Rect;
}  

class SURFACE_EXPORT AcceleratedSurfaceTransformer {
 public:
  
  
  AcceleratedSurfaceTransformer();

  
  
  
  
  
  bool Init(IDirect3DDevice9* device);

  
  void ReleaseAll();

  
  
  
  void DetachAll();

  
  bool CopyInverted(
      IDirect3DTexture9* src_texture,
      IDirect3DSurface9* dst_surface,
      const gfx::Size& dst_size);

  
  bool Copy(
      IDirect3DTexture9* src_texture,
      IDirect3DSurface9* dst_surface,
      const gfx::Size& dst_size);

  
  
  
  
  bool GetIntermediateTexture(
      const gfx::Size& size,
      IDirect3DTexture9** texture,
      IDirect3DSurface9** texture_level_zero);

  
  bool ResizeBilinear(
    IDirect3DSurface9* src_surface,
    const gfx::Rect& src_subrect,
    IDirect3DSurface9* dst_surface,
    const gfx::Rect& dst_subrect);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool TransformRGBToYV12(
      IDirect3DTexture9* src_texture,
      const gfx::Size& dst_size,
      IDirect3DSurface9** dst_y,
      IDirect3DSurface9** dst_u,
      IDirect3DSurface9** dst_v);

  
  
  
  bool ReadFast(IDirect3DSurface9* gpu_surface,
                uint8* dst,
                int dst_bytes_per_row,
                int dst_num_rows,
                int dst_stride);

  
  
  
  bool ReadByLockAndCopy(IDirect3DSurface9* gpu_surface,
                         uint8* dst,
                         int dst_bytes_per_row,
                         int dst_num_rows,
                         int dst_stride);
  bool ReadByGetRenderTargetData(IDirect3DSurface9* gpu_surface,
                                 uint8* dst,
                                 int dst_bytes_per_row,
                                 int dst_num_rows,
                                 int dst_stride);

 private:
  friend class AcceleratedSurfaceTransformerTest;
  FRIEND_TEST_ALL_PREFIXES(AcceleratedSurfaceTransformerTest, Init);

  enum ShaderCombo {
    ONE_TEXTURE,
    RGB_TO_YV12_FAST__PASS_1_OF_2,
    RGB_TO_YV12_FAST__PASS_2_OF_2,
    RGB_TO_YV12_SLOW__PASS_1_OF_3,
    RGB_TO_YV12_SLOW__PASS_2_OF_3,
    RGB_TO_YV12_SLOW__PASS_3_OF_3,
    NUM_SHADERS
  };

  
  
  
  
  bool TransformRGBToYV12_MRT(
      IDirect3DTexture9* src_surface,
      const gfx::Size& dst_size,
      const gfx::Size& packed_y_size,
      const gfx::Size& packed_uv_size,
      IDirect3DSurface9* dst_y,
      IDirect3DSurface9* dst_u,
      IDirect3DSurface9* dst_v);

  
  
  
  
  
  bool TransformRGBToYV12_WithoutMRT(
      IDirect3DTexture9* src_surface,
      const gfx::Size& dst_size,
      const gfx::Size& packed_y_size,
      const gfx::Size& packed_uv_size,
      IDirect3DSurface9* dst_y,
      IDirect3DSurface9* dst_u,
      IDirect3DSurface9* dst_v);

  
  
  
  
  
  
  
  bool AllocYUVBuffers(
      const gfx::Size& dst_size,
      gfx::Size* packed_y_size,
      gfx::Size* packed_uv_size,
      IDirect3DSurface9** dst_y,
      IDirect3DSurface9** dst_u,
      IDirect3DSurface9** dst_v);

  bool CopyWithTextureScale(
      IDirect3DTexture9* src_texture,
      IDirect3DSurface9* dst_surface,
      const gfx::Size& dst_size,
      float texture_scale_x,
      float texture_scale_y);

  
  
  
  bool SetShaderCombo(ShaderCombo combo);

  
  
  
  bool CompileShaderCombo(ShaderCombo shader_combo_name);

  bool DoInit(IDirect3DDevice9* device);

  void DrawScreenAlignedQuad(const gfx::Size& dst_size);

  bool device_supports_multiple_render_targets() const {
    return device_supports_multiple_render_targets_;
  }

  IDirect3DDevice9* device();

  base::win::ScopedComPtr<IDirect3DDevice9> device_;
  base::win::ScopedComPtr<IDirect3DVertexShader9> vertex_shaders_[NUM_SHADERS];
  base::win::ScopedComPtr<IDirect3DPixelShader9> pixel_shaders_[NUM_SHADERS];

  
  base::win::ScopedComPtr<IDirect3DTexture9> user_scratch_texture_;
  base::win::ScopedComPtr<IDirect3DTexture9> uv_scratch_texture_;
  base::win::ScopedComPtr<IDirect3DSurface9> y_scratch_surface_;
  base::win::ScopedComPtr<IDirect3DSurface9> u_scratch_surface_;
  base::win::ScopedComPtr<IDirect3DSurface9> v_scratch_surface_;
  base::win::ScopedComPtr<IDirect3DSurface9> scaler_scratch_surfaces_[2];

  bool device_supports_multiple_render_targets_;
  const BYTE* vertex_shader_sources_[NUM_SHADERS];
  const BYTE* pixel_shader_sources_[NUM_SHADERS];
  DISALLOW_COPY_AND_ASSIGN(AcceleratedSurfaceTransformer);
};

#endif  
