// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SYSTEM_KEY_EVENT_LISTENER_H_
#define CHROME_BROWSER_CHROMEOS_SYSTEM_KEY_EVENT_LISTENER_H_

#include "base/memory/singleton.h"
#include "base/message_loop/message_loop.h"

typedef union _XEvent XEvent;

namespace chromeos {

class SystemKeyEventListener : public base::MessageLoopForUI::Observer {
 public:
  
  class CapsLockObserver {
   public:
    virtual void OnCapsLockChange(bool enabled) = 0;

   protected:
    CapsLockObserver() {}
    virtual ~CapsLockObserver() {}

    DISALLOW_COPY_AND_ASSIGN(CapsLockObserver);
  };

  
  class ModifiersObserver {
   public:
    enum {
      SHIFT_PRESSED = 1,
      CTRL_PRESSED = 1 << 1,
      ALT_PRESSED = 1 << 2,
    };

    
    
    virtual void OnModifiersChange(int pressed_modifiers) = 0;

   protected:
    ModifiersObserver() {}
    virtual ~ModifiersObserver() {}

    DISALLOW_COPY_AND_ASSIGN(ModifiersObserver);
  };

  static void Initialize();
  static void Shutdown();
  
  static SystemKeyEventListener* GetInstance();

  void Stop();

  void AddCapsLockObserver(CapsLockObserver* observer);
  void AddModifiersObserver(ModifiersObserver* observer);
  void RemoveCapsLockObserver(CapsLockObserver* observer);
  void RemoveModifiersObserver(ModifiersObserver* observer);

 private:
  
  
  friend struct DefaultSingletonTraits<SystemKeyEventListener>;
  friend class SystemKeyEventListenerTest;

  SystemKeyEventListener();
  virtual ~SystemKeyEventListener();

  
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE;
  virtual void DidProcessEvent(const base::NativeEvent& event) OVERRIDE;

  void OnCapsLock(bool enabled);
  void OnModifiers(int state);

  
  virtual bool ProcessedXEvent(XEvent* xevent);

  bool stopped_;

  unsigned int num_lock_mask_;
  bool caps_lock_is_on_;
  int pressed_modifiers_;
  ObserverList<CapsLockObserver> caps_lock_observers_;
  ObserverList<ModifiersObserver> modifiers_observers_;

  
  int xkb_event_base_;

  DISALLOW_COPY_AND_ASSIGN(SystemKeyEventListener);
};

}  

#endif  
