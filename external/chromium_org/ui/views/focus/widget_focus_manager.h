// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_WIDGET_FOCUS_MANAGER_H_
#define UI_VIEWS_FOCUS_WIDGET_FOCUS_MANAGER_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

template <typename T> struct DefaultSingletonTraits;

namespace views {

class WidgetFocusChangeListener {
 public:
  virtual void OnNativeFocusChange(gfx::NativeView focused_before,
                                   gfx::NativeView focused_now) = 0;

 protected:
  virtual ~WidgetFocusChangeListener() {}
};

class VIEWS_EXPORT WidgetFocusManager {
 public:
  
  static WidgetFocusManager* GetInstance();

  
  
  void AddFocusChangeListener(WidgetFocusChangeListener* listener);
  void RemoveFocusChangeListener(WidgetFocusChangeListener* listener);

  
  
  
  
  
  void OnWidgetFocusEvent(gfx::NativeView focused_before,
                          gfx::NativeView focused_now);

  
  
  
  void EnableNotifications() { enabled_ = true; }
  void DisableNotifications() { enabled_ = false; }

 private:
  friend struct DefaultSingletonTraits<WidgetFocusManager>;

  WidgetFocusManager();
  ~WidgetFocusManager();

  ObserverList<WidgetFocusChangeListener> focus_change_listeners_;

  bool enabled_;

  DISALLOW_COPY_AND_ASSIGN(WidgetFocusManager);
};

class VIEWS_EXPORT AutoNativeNotificationDisabler {
 public:
  AutoNativeNotificationDisabler();
  ~AutoNativeNotificationDisabler();

 private:
  DISALLOW_COPY_AND_ASSIGN(AutoNativeNotificationDisabler);
};

}  

#endif  
