// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_MOUSE_WATCHER_H_
#define UI_VIEWS_MOUSE_WATCHER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "ui/gfx/insets.h"
#include "ui/views/views_export.h"

namespace gfx {
class Point;
}

namespace views {

class VIEWS_EXPORT MouseWatcherListener {
 public:
  virtual void MouseMovedOutOfHost() = 0;

 protected:
  virtual ~MouseWatcherListener();
};

class VIEWS_EXPORT MouseWatcherHost {
 public:
  
  enum MouseEventType {
    
    
    MOUSE_MOVE,
    
    
    MOUSE_EXIT
  };

  virtual ~MouseWatcherHost();

  
  virtual bool Contains(const gfx::Point& screen_point,
                        MouseEventType type) = 0;
};

class VIEWS_EXPORT MouseWatcher {
 public:
  
  
  
  MouseWatcher(MouseWatcherHost* host, MouseWatcherListener* listener);
  ~MouseWatcher();

  
  
  void set_notify_on_exit_time(base::TimeDelta time) {
    notify_on_exit_time_ = time;
  }

  
  
  
  
  void Start();

  
  void Stop();

 private:
  class Observer;

  
  bool is_observing() const { return observer_.get() != NULL; }

  
  void NotifyListener();

  
  scoped_ptr<MouseWatcherHost> host_;

  
  MouseWatcherListener* listener_;

  
  scoped_ptr<Observer> observer_;

  
  base::TimeDelta notify_on_exit_time_;

  DISALLOW_COPY_AND_ASSIGN(MouseWatcher);
};

}  

#endif  
