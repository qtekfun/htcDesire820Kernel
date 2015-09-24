// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GPU_SWITCHING_MANAGER_H_
#define UI_GL_GPU_SWITCHING_MANAGER_H_

#include <vector>

#include "base/memory/singleton.h"
#include "ui/gl/gl_export.h"
#include "ui/gl/gpu_preference.h"

#if defined(OS_MACOSX)
#include <OpenGL/OpenGL.h>
#endif  

namespace ui {

class GL_EXPORT GpuSwitchingManager {
 public:
  
  static GpuSwitchingManager* GetInstance();

  
  void ForceUseOfIntegratedGpu();
  
  
  void ForceUseOfDiscreteGpu();

  
  
  gfx::GpuPreference AdjustGpuPreference(gfx::GpuPreference gpu_preference);

  
  
  
  
  bool SupportsDualGpus();

  void SetGpuCount(size_t gpu_count);

 private:
  friend struct DefaultSingletonTraits<GpuSwitchingManager>;

  GpuSwitchingManager();
  virtual ~GpuSwitchingManager();

#if defined(OS_MACOSX)
  void SwitchToDiscreteGpuMac();

  CGLPixelFormatObj discrete_pixel_format_;
#endif  

  gfx::GpuPreference gpu_switching_option_;
  bool gpu_switching_option_set_;

  bool supports_dual_gpus_;
  bool supports_dual_gpus_set_;

  size_t gpu_count_;

  DISALLOW_COPY_AND_ASSIGN(GpuSwitchingManager);
};

}  

#endif  
