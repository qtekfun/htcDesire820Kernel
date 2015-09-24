// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_USER_INPUT_MONITOR_H_
#define MEDIA_BASE_USER_INPUT_MONITOR_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list_threadsafe.h"
#include "base/synchronization/lock.h"
#include "media/base/media_export.h"

struct SkIPoint;

namespace base {
class SingleThreadTaskRunner;
}  

namespace media {

class MEDIA_EXPORT UserInputMonitor {
 public:
  
  class MEDIA_EXPORT MouseEventListener {
   public:
    
    virtual void OnMouseMoved(const SkIPoint& position) = 0;

   protected:
    virtual ~MouseEventListener() {}
  };
  typedef ObserverListThreadSafe<UserInputMonitor::MouseEventListener>
      MouseListenerList;

  UserInputMonitor();
  virtual ~UserInputMonitor();

  
  
  
  static scoped_ptr<UserInputMonitor> Create(
      const scoped_refptr<base::SingleThreadTaskRunner>& io_task_runner,
      const scoped_refptr<base::SingleThreadTaskRunner>& ui_task_runner);

  
  
  
  void AddMouseListener(MouseEventListener* listener);
  void RemoveMouseListener(MouseEventListener* listener);

  
  
  void EnableKeyPressMonitoring();
  void DisableKeyPressMonitoring();

  
  
  
  
  
  
  virtual size_t GetKeyPressCount() const = 0;

 protected:
  scoped_refptr<MouseListenerList> mouse_listeners() {
    return mouse_listeners_;
  }

 private:
  virtual void StartKeyboardMonitoring() = 0;
  virtual void StopKeyboardMonitoring() = 0;
  virtual void StartMouseMonitoring() = 0;
  virtual void StopMouseMonitoring() = 0;

  base::Lock lock_;
  size_t key_press_counter_references_;
  size_t mouse_listeners_count_;
  scoped_refptr<MouseListenerList> mouse_listeners_;

  DISALLOW_COPY_AND_ASSIGN(UserInputMonitor);
};

}  

#endif  
