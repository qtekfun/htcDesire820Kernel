// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COLOR_CHOOSER_COLOR_CHOOSER_VIEW_H_
#define UI_VIEWS_COLOR_CHOOSER_COLOR_CHOOSER_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/skia/include/core/SkScalar.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/views_export.h"
#include "ui/views/widget/widget_delegate.h"

namespace views {

class ColorChooserListener;
class Textfield;

class VIEWS_EXPORT ColorChooserView : public WidgetDelegateView,
                                      public TextfieldController {
 public:
  ColorChooserView(ColorChooserListener* listener, SkColor initial_color);
  virtual ~ColorChooserView();

  
  void OnColorChanged(SkColor color);

  
  void OnHueChosen(SkScalar hue);

  
  void OnSaturationValueChosen(SkScalar saturation, SkScalar value);

  float hue() const { return hsv_[0]; }
  float saturation() const { return hsv_[1]; }
  float value() const { return hsv_[2]; }
  void set_listener(ColorChooserListener* listener) { listener_ = listener; }

 private:
  class HueView;
  class SaturationValueView;
  class SelectedColorPatchView;

  
  virtual View* GetInitiallyFocusedView() OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual void WindowClosing() OVERRIDE;
  virtual View* GetContentsView() OVERRIDE;

  
  virtual void ContentsChanged(Textfield* sender,
                               const string16& new_contents) OVERRIDE;
  virtual bool HandleKeyEvent(Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;

  
  SkScalar hsv_[3];

  
  
  
  ColorChooserListener* listener_;

  
  
  HueView* hue_;

  
  SaturationValueView* saturation_value_;

  
  Textfield* textfield_;

  
  SelectedColorPatchView* selected_color_patch_;

  DISALLOW_COPY_AND_ASSIGN(ColorChooserView);
};

}  

#endif  
