// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_DISPLAY_INFO_H_
#define ASH_DISPLAY_DISPLAY_INFO_H_

#include <string>
#include <vector>

#include "ash/ash_export.h"
#include "ui/gfx/display.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/rect.h"

namespace ash {
namespace internal {

struct ASH_EXPORT Resolution {
  Resolution(const gfx::Size& size, bool interlaced);

  gfx::Size size;
  bool interlaced;
};

class ASH_EXPORT DisplayInfo {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static DisplayInfo CreateFromSpec(const std::string& spec);

  
  static DisplayInfo CreateFromSpecWithID(const std::string& spec,
                                          int64 id);

  DisplayInfo();
  DisplayInfo(int64 id, const std::string& name, bool has_overscan);
  ~DisplayInfo();

  int64 id() const { return id_; }

  
  const std::string& name() const { return name_; }

  
  
  bool has_overscan() const { return has_overscan_; }

  void set_rotation(gfx::Display::Rotation rotation) { rotation_ = rotation; }
  gfx::Display::Rotation rotation() const { return rotation_; }

  void set_touch_support(gfx::Display::TouchSupport support) {
    touch_support_ = support;
  }
  gfx::Display::TouchSupport touch_support() const { return touch_support_; }

  
  float device_scale_factor() const { return device_scale_factor_; }
  void set_device_scale_factor(float scale) { device_scale_factor_ = scale; }

  
  
  
  const gfx::Rect bounds_in_native() const {
    return bounds_in_native_;
  }

  
  const gfx::Size& size_in_pixel() const { return size_in_pixel_; }

  
  const gfx::Insets& overscan_insets_in_dip() const {
    return overscan_insets_in_dip_;
  }

  
  
  
  float configured_ui_scale() const { return configured_ui_scale_; }
  void set_configured_ui_scale(float scale) { configured_ui_scale_ = scale; }

  
  
  float GetEffectiveUIScale() const;

  
  
  
  void Copy(const DisplayInfo& another_info);

  
  
  void SetBounds(const gfx::Rect& bounds_in_native);

  
  
  void UpdateDisplaySize();

  
  void SetOverscanInsets(const gfx::Insets& insets_in_dip);
  gfx::Insets GetOverscanInsetsInPixel() const;

  void set_native(bool native) { native_ = native; }
  bool native() const { return native_; }

  const std::vector<Resolution>& resolutions() const {
    return resolutions_;
  }
  void set_resolutions(std::vector<Resolution>& resolution) {
    resolutions_.swap(resolution);
  }

  
  
  std::string ToString() const;

  
  
  std::string ToFullString() const;

 private:
  int64 id_;
  std::string name_;
  bool has_overscan_;
  gfx::Display::Rotation rotation_;
  gfx::Display::TouchSupport touch_support_;

  
  
  
  
  
  float device_scale_factor_;
  gfx::Rect bounds_in_native_;

  
  
  gfx::Size size_in_pixel_;
  gfx::Insets overscan_insets_in_dip_;

  
  
  
  
  
  
  float configured_ui_scale_;

  
  bool native_;

  
  std::vector<Resolution> resolutions_;
};

}  
}  

#endif  
