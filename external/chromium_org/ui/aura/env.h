// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ENV_H_
#define UI_AURA_ENV_H_

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/observer_list.h"
#include "ui/aura/aura_export.h"
#include "ui/events/event_handler.h"
#include "ui/events/event_target.h"
#include "ui/gfx/point.h"

#if defined(USE_X11)
#include "ui/aura/device_list_updater_aurax11.h"
#endif

namespace aura {

namespace test {
class EnvTestHelper;
}

class EnvObserver;
class InputStateLookup;
class RootWindow;
class Window;

#if !defined(OS_MACOSX) && !defined(OS_ANDROID) && !defined(USE_X11)
base::MessageLoop::Dispatcher* CreateDispatcher();
#endif

class AURA_EXPORT Env : public ui::EventTarget {
 public:
  Env();
  virtual ~Env();

  static void CreateInstance();
  static Env* GetInstance();
  static void DeleteInstance();

  void AddObserver(EnvObserver* observer);
  void RemoveObserver(EnvObserver* observer);

  void set_mouse_button_flags(int mouse_button_flags) {
    mouse_button_flags_ = mouse_button_flags;
  }
  
  
  bool IsMouseButtonDown() const;

  
  
  const gfx::Point& last_mouse_location() const { return last_mouse_location_; }
  void set_last_mouse_location(const gfx::Point& last_mouse_location) {
    last_mouse_location_ = last_mouse_location;
  }

  
  bool is_touch_down() const { return is_touch_down_; }
  void set_touch_down(bool value) { is_touch_down_ = value; }

  
  
  
#if !defined(OS_MACOSX) && !defined(OS_ANDROID) && \
    !defined(USE_GTK_MESSAGE_PUMP)
  base::MessageLoop::Dispatcher* GetDispatcher();
#endif

  
  void RootWindowActivated(RootWindow* root_window);

 private:
  friend class test::EnvTestHelper;
  friend class Window;
  friend class RootWindow;

  void Init();

  
  void NotifyWindowInitialized(Window* window);

  
  void NotifyRootWindowInitialized(RootWindow* root_window);

  
  virtual bool CanAcceptEvent(const ui::Event& event) OVERRIDE;
  virtual ui::EventTarget* GetParentTarget() OVERRIDE;
  virtual scoped_ptr<ui::EventTargetIterator> GetChildIterator() const OVERRIDE;
  virtual ui::EventTargeter* GetEventTargeter() OVERRIDE;

  ObserverList<EnvObserver> observers_;
#if !defined(OS_MACOSX) && !defined(OS_ANDROID) && !defined(USE_X11)
  scoped_ptr<base::MessageLoop::Dispatcher> dispatcher_;
#endif

  static Env* instance_;
  int mouse_button_flags_;
  
  gfx::Point last_mouse_location_;
  bool is_touch_down_;

#if defined(USE_X11)
  DeviceListUpdaterAuraX11 device_list_updater_aurax11_;
#endif

  scoped_ptr<InputStateLookup> input_state_lookup_;

  DISALLOW_COPY_AND_ASSIGN(Env);
};

}  

#endif  
