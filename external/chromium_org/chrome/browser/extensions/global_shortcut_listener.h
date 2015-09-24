// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_GLOBAL_SHORTCUT_LISTENER_H_
#define CHROME_BROWSER_EXTENSIONS_GLOBAL_SHORTCUT_LISTENER_H_

#include <map>

#include "base/observer_list.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/native_widget_types.h"

namespace ui {
class Accelerator;
}

namespace extensions {

class GlobalShortcutListener {
 public:
  class Observer {
   public:
    
    virtual void OnKeyPressed(const ui::Accelerator& accelerator) = 0;
  };

  virtual ~GlobalShortcutListener();

  static GlobalShortcutListener* GetInstance();

  
  virtual void StartListening() = 0;
  virtual void StopListening() = 0;

  
  virtual void RegisterAccelerator(
      const ui::Accelerator& accelerator, Observer* observer);
  
  virtual void UnregisterAccelerator(
      const ui::Accelerator& accelerator, Observer* observer);

 protected:
  GlobalShortcutListener();

  
  
  void NotifyKeyPressed(const ui::Accelerator& accelerator);

  
  
  typedef ObserverList<Observer> Observers;
  typedef std::map< ui::Accelerator, Observers* > AcceleratorMap;
  AcceleratorMap accelerator_map_;

 private:
  DISALLOW_COPY_AND_ASSIGN(GlobalShortcutListener);
};

}  

#endif  
