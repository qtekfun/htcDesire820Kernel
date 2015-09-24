// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_MODE_INDICATOR_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_MODE_INDICATOR_CONTROLLER_H_

#include "base/memory/scoped_ptr.h"
#include "chromeos/ime/input_method_manager.h"
#include "ui/gfx/rect.h"
#include "ui/views/widget/widget_observer.h"

namespace chromeos {
namespace input_method {

class ModeIndicatorObserverInterface : public views::WidgetObserver {
 public:
  ModeIndicatorObserverInterface() {}
  virtual ~ModeIndicatorObserverInterface() {}

  virtual void AddModeIndicatorWidget(views::Widget* widget) = 0;
};


class ModeIndicatorController
    : public InputMethodManager::Observer {
 public:
  
  explicit ModeIndicatorController(InputMethodManager* imm);
  virtual ~ModeIndicatorController();

  
  
  void SetCursorBounds(const gfx::Rect& cursor_location);

  
  void FocusStateChanged(bool is_focused);

  
  
  static void SetModeIndicatorObserverForTesting(
      ModeIndicatorObserverInterface* observer);
  static ModeIndicatorObserverInterface* GetModeIndicatorObserverForTesting();

 private:
  
  virtual void InputMethodChanged(InputMethodManager* manager,
                                  bool show_message) OVERRIDE;
  virtual void InputMethodPropertyChanged(InputMethodManager* manager) OVERRIDE;

  
  
  void ShowModeIndicator();

  InputMethodManager* imm_;

  
  gfx::Rect cursor_bounds_;

  
  bool is_focused_;

  
  
  scoped_ptr<ModeIndicatorObserverInterface> mi_observer_;

  DISALLOW_COPY_AND_ASSIGN(ModeIndicatorController);
};

}  
}  

#endif  
