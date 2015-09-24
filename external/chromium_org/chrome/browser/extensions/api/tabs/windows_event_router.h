// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_TABS_WINDOWS_EVENT_ROUTER_H_
#define CHROME_BROWSER_EXTENSIONS_API_TABS_WINDOWS_EVENT_ROUTER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/window_controller_list_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#if defined(TOOLKIT_VIEWS)
#include "ui/views/focus/widget_focus_manager.h"
#elif defined(TOOLKIT_GTK)
#include "ui/base/x/active_window_watcher_x_observer.h"
#endif

class Profile;

namespace base {
class ListValue;
}

namespace extensions {

class WindowsEventRouter : public WindowControllerListObserver,
#if defined(TOOLKIT_VIEWS)
                          public views::WidgetFocusChangeListener,
#elif defined(TOOLKIT_GTK)
                          public ui::ActiveWindowWatcherXObserver,
#endif
                          public content::NotificationObserver {
 public:
  explicit WindowsEventRouter(Profile* profile);
  virtual ~WindowsEventRouter();

  
  virtual void OnWindowControllerAdded(
      WindowController* window_controller) OVERRIDE;
  virtual void OnWindowControllerRemoved(
      WindowController* window) OVERRIDE;

#if defined(TOOLKIT_VIEWS)
  virtual void OnNativeFocusChange(gfx::NativeView focused_before,
                                   gfx::NativeView focused_now) OVERRIDE;
#elif defined(TOOLKIT_GTK)
  virtual void ActiveWindowChanged(GdkWindow* active_window) OVERRIDE;
#endif

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void OnActiveWindowChanged(WindowController* window_controller);

 private:
  void DispatchEvent(const std::string& event_name,
                     Profile* profile,
                     scoped_ptr<base::ListValue> args);

  content::NotificationRegistrar registrar_;

  
  Profile* profile_;

  
  
  
  Profile* focused_profile_;

  
  
  int focused_window_id_;

  DISALLOW_COPY_AND_ASSIGN(WindowsEventRouter);
};

}  

#endif  
