// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_VIDEO_DETECTOR_H_
#define ASH_WM_VIDEO_DETECTOR_H_

#include <map>

#include "ash/ash_export.h"
#include "ash/shell_observer.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/observer_list.h"
#include "base/scoped_observer.h"
#include "base/time/time.h"
#include "ui/aura/env_observer.h"
#include "ui/aura/window_observer.h"

namespace aura {
class Window;
}

namespace gfx {
class Rect;
}

namespace ash {

class ASH_EXPORT VideoDetectorObserver {
 public:
  
  virtual void OnVideoDetected(bool is_fullscreen) = 0;

 protected:
  virtual ~VideoDetectorObserver() {}
};

class ASH_EXPORT VideoDetector : public aura::EnvObserver,
                                 public aura::WindowObserver,
                                 public ShellObserver  {
 public:
  
  
  static const int kMinUpdateWidth;
  static const int kMinUpdateHeight;

  
  
  static const int kMinFramesPerSecond;

  
  
  static const double kNotifyIntervalSec;

  VideoDetector();
  virtual ~VideoDetector();

  void set_now_for_test(base::TimeTicks now) { now_for_test_ = now; }

  void AddObserver(VideoDetectorObserver* observer);
  void RemoveObserver(VideoDetectorObserver* observer);

  
  virtual void OnWindowInitialized(aura::Window* window) OVERRIDE;

  
  virtual void OnWindowPaintScheduled(aura::Window* window,
                                      const gfx::Rect& region) OVERRIDE;
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

  
  virtual void OnAppTerminating() OVERRIDE;

 private:
  class WindowInfo;
  typedef std::map<aura::Window*, linked_ptr<WindowInfo> > WindowInfoMap;

  
  
  
  void MaybeNotifyObservers(aura::Window* window, base::TimeTicks now);

  
  WindowInfoMap window_infos_;

  ObserverList<VideoDetectorObserver> observers_;

  
  base::TimeTicks last_observer_notification_time_;

  
  
  base::TimeTicks now_for_test_;

  ScopedObserver<aura::Window, aura::WindowObserver> observer_manager_;

  bool is_shutting_down_;

  DISALLOW_COPY_AND_ASSIGN(VideoDetector);
};

}  

#endif  
