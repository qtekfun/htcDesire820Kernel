// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_COMPOSITOR_OBSERVER_H_
#define UI_COMPOSITOR_COMPOSITOR_OBSERVER_H_

#include "base/time/time.h"
#include "ui/compositor/compositor_export.h"

namespace ui {

class Compositor;

class COMPOSITOR_EXPORT CompositorObserver {
 public:
  
  
  
  
  
  
  
  virtual void OnCompositingDidCommit(Compositor* compositor) = 0;

  
  
  virtual void OnCompositingStarted(Compositor* compositor,
                                    base::TimeTicks start_time) = 0;

  
  virtual void OnCompositingEnded(Compositor* compositor) = 0;

  
  virtual void OnCompositingAborted(Compositor* compositor) = 0;

  
  virtual void OnCompositingLockStateChanged(Compositor* compositor) = 0;

  
  virtual void OnUpdateVSyncParameters(Compositor* compositor,
                                       base::TimeTicks timebase,
                                       base::TimeDelta interval) = 0;

 protected:
  virtual ~CompositorObserver() {}
};

}  

#endif  
