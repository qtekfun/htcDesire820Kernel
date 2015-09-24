// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_RESIZING_HOST_OBSERVER_H_
#define REMOTING_HOST_RESIZING_HOST_OBSERVER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "remoting/host/screen_controls.h"
#include "remoting/host/screen_resolution.h"

namespace remoting {

class DesktopResizer;


class ResizingHostObserver : public ScreenControls {
 public:
  explicit ResizingHostObserver(scoped_ptr<DesktopResizer> desktop_resizer);
  virtual ~ResizingHostObserver();

  
  virtual void SetScreenResolution(const ScreenResolution& resolution) OVERRIDE;

  
  
  
  void SetNowFunctionForTesting(
      const base::Callback<base::Time(void)>& now_function);

 private:
  scoped_ptr<DesktopResizer> desktop_resizer_;
  ScreenResolution original_resolution_;

  
  base::OneShotTimer<ResizingHostObserver> deferred_resize_timer_;
  base::Time previous_resize_time_;
  base::Callback<base::Time(void)> now_function_;

  base::WeakPtrFactory<ResizingHostObserver> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ResizingHostObserver);
};

}  

#endif  
