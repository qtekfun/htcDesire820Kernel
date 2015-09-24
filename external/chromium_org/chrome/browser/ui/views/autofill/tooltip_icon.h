// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AUTOFILL_TOOLTIP_ICON_H_
#define CHROME_BROWSER_UI_VIEWS_AUTOFILL_TOOLTIP_ICON_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/scoped_observer.h"
#include "base/strings/string16.h"
#include "base/timer/timer.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/mouse_watcher.h"
#include "ui/views/widget/widget_observer.h"

namespace autofill {

class InfoBubble;

class TooltipIcon : public views::ImageView,
                    public views::MouseWatcherListener,
                    public views::WidgetObserver {
 public:
  static const char kViewClassName[];

  explicit TooltipIcon(const base::string16& tooltip);
  virtual ~TooltipIcon();

  
  virtual const char* GetClassName() const OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& prev_bounds) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

  
  virtual void MouseMovedOutOfHost() OVERRIDE;

  
  virtual void OnWidgetDestroyed(views::Widget* widget) OVERRIDE;

 private:
  
  void ChangeImageTo(int idr);

  
  
  void ShowBubble();

  
  void HideBubble();

  
  base::string16 tooltip_;

  
  bool mouse_inside_;

  
  InfoBubble* bubble_;

  
  base::OneShotTimer<TooltipIcon> show_timer_;

  
  scoped_ptr<views::MouseWatcher> mouse_watcher_;

  ScopedObserver<views::Widget, TooltipIcon> observer_;

  DISALLOW_COPY_AND_ASSIGN(TooltipIcon);
};

}  

#endif  
