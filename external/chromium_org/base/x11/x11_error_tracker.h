// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_X11_X11_ERROR_TRACKER_H_
#define BASE_X11_X11_ERROR_TRACKER_H_

#include <X11/Xlib.h>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {

class BASE_EXPORT X11ErrorTracker {
 public:
  X11ErrorTracker();
  ~X11ErrorTracker();

  
  
  
  bool FoundNewError();

 private:
#if !defined(TOOLKIT_GTK)
  XErrorHandler old_handler_;
#endif

  DISALLOW_COPY_AND_ASSIGN(X11ErrorTracker);
};

}  

#endif  
