// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_DISPLAY_H_
#define UI_GFX_DISPLAY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/rect.h"

namespace gfx {

class GFX_EXPORT Display {
 public:
  
  enum Rotation {
    ROTATE_0 = 0,
    ROTATE_90,
    ROTATE_180,
    ROTATE_270,
  };

  
  enum TouchSupport {
    TOUCH_SUPPORT_UNKNOWN,
    TOUCH_SUPPORT_AVAILABLE,
    TOUCH_SUPPORT_UNAVAILABLE,
  };

  
  Display();
  explicit Display(int64 id);
  Display(int64 id, const Rect& bounds);
  ~Display();

  
  
  static float GetForcedDeviceScaleFactor();

  
  
  static bool HasForceDeviceScaleFactor();

  
  
  int64 id() const { return id_; }
  void set_id(int64 id) { id_ = id; }

  
  const Rect& bounds() const { return bounds_; }
  void set_bounds(const Rect& bounds) { bounds_ = bounds; }

  
  const Rect& work_area() const { return work_area_; }
  void set_work_area(const Rect& work_area) { work_area_ = work_area; }

  
  
  
  
  float device_scale_factor() const { return device_scale_factor_; }
  void set_device_scale_factor(float scale) { device_scale_factor_ = scale; }

  Rotation rotation() const { return rotation_; }
  void set_rotation(Rotation rotation) { rotation_ = rotation; }

  TouchSupport touch_support() const { return touch_support_; }
  void set_touch_support(TouchSupport support) { touch_support_ = support; }

  
  
  const Size& size() const { return bounds_.size(); }
  const Size& work_area_size() const { return work_area_.size(); }

  
  Insets GetWorkAreaInsets() const;

  
  
  
  void SetScaleAndBounds(float device_scale_factor,
                         const gfx::Rect& bounds_in_pixel);

  
  
  void SetSize(const gfx::Size& size_in_pixel);

  
  
  void UpdateWorkAreaFromInsets(const gfx::Insets& work_area_insets);

  
  gfx::Size GetSizeInPixel() const;

  
  std::string ToString() const;

  
  bool is_valid() const { return id_ != kInvalidDisplayID; }

  
  bool IsInternal() const;

  
  static int64 InternalDisplayId();
  static void SetInternalDisplayId(int64 internal_display_id);

  static const int64 kInvalidDisplayID;

 private:
  int64 id_;
  Rect bounds_;
  Rect work_area_;
  float device_scale_factor_;
  Rotation rotation_;
  TouchSupport touch_support_;
};

}  

#endif  
