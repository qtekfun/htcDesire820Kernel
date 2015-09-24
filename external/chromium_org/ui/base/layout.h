// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_LAYOUT_H_
#define UI_BASE_LAYOUT_H_

#include <vector>

#include "build/build_config.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/native_widget_types.h"

namespace ui {

enum DisplayLayout {
  
  LAYOUT_DESKTOP,

  
  LAYOUT_TOUCH,
};

UI_EXPORT DisplayLayout GetDisplayLayout();

enum ScaleFactor {
  SCALE_FACTOR_NONE = 0,
  SCALE_FACTOR_100P,
  SCALE_FACTOR_125P,
  SCALE_FACTOR_133P,
  SCALE_FACTOR_140P,
  SCALE_FACTOR_150P,
  SCALE_FACTOR_180P,
  SCALE_FACTOR_200P,
  SCALE_FACTOR_300P,

  NUM_SCALE_FACTORS  
};

UI_EXPORT void SetSupportedScaleFactors(
    const std::vector<ScaleFactor>& scale_factors);

UI_EXPORT const std::vector<ScaleFactor>& GetSupportedScaleFactors();

UI_EXPORT float GetImageScale(ScaleFactor scale_factor);

UI_EXPORT ScaleFactor GetSupportedScaleFactor(float image_scale);

UI_EXPORT ScaleFactor GetScaleFactorForNativeView(gfx::NativeView view);

UI_EXPORT bool IsScaleFactorSupported(ScaleFactor scale_factor);

UI_EXPORT ScaleFactor FindClosestScaleFactorUnsafe(float scale);

namespace test {
class UI_EXPORT ScopedSetSupportedScaleFactors {
 public:
  explicit ScopedSetSupportedScaleFactors(
      const std::vector<ui::ScaleFactor>& new_scale_factors);
  ~ScopedSetSupportedScaleFactors();

 private:
  std::vector<ui::ScaleFactor>* original_scale_factors_;

  DISALLOW_COPY_AND_ASSIGN(ScopedSetSupportedScaleFactors);
};

}  

}  

#endif  
