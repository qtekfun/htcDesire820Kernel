// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_BUTTON_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_BUTTON_DECORATION_H_

#import "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/cocoa/location_bar/location_bar_decoration.h"
#import "ui/base/cocoa/appkit_utils.h"


class ButtonDecoration : public LocationBarDecoration {
 public:
  enum ButtonState {
    kButtonStateNormal,
    kButtonStateHover,
    kButtonStatePressed
  };

  
  
  
  
  ButtonDecoration(ui::NinePartImageIds normal_image_ids,
                   int normal_icon_id,
                   ui::NinePartImageIds hover_image_ids,
                   int hover_icon_id,
                   ui::NinePartImageIds pressed_image_ids,
                   int pressed_icon_id,
                   CGFloat max_inner_padding);

  virtual ~ButtonDecoration();

  void SetButtonState(ButtonState state);
  ButtonState GetButtonState() const;

  
  void SetIcon(ButtonState state, int icon_id);

  
  void SetIcon(int icon_id);

  
  virtual CGFloat GetWidthForSpace(CGFloat width) OVERRIDE;
  virtual void DrawInFrame(NSRect frame, NSView* control_view) OVERRIDE;
  virtual bool AcceptsMousePress() OVERRIDE;
  virtual bool IsDraggable() OVERRIDE;
  virtual bool OnMousePressed(NSRect frame) OVERRIDE;
  virtual ButtonDecoration* AsButtonDecoration() OVERRIDE;

 private:
  ui::NinePartImageIds normal_image_ids_;
  ui::NinePartImageIds hover_image_ids_;
  ui::NinePartImageIds pressed_image_ids_;
  int normal_icon_id_;
  int hover_icon_id_;
  int pressed_icon_id_;
  ButtonState state_;
  CGFloat max_inner_padding_;

  ui::NinePartImageIds GetImageIds() const;
  int GetIconId() const;

  DISALLOW_COPY_AND_ASSIGN(ButtonDecoration);
};

#endif  
