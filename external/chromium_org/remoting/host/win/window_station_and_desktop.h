// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_WINDOW_STATION_AND_DESKTOP_H_
#define REMOTING_HOST_WIN_WINDOW_STATION_AND_DESKTOP_H_

#include <windows.h>

#include "base/basictypes.h"

namespace remoting {

class WindowStationAndDesktop {
 public:
  WindowStationAndDesktop();
  ~WindowStationAndDesktop();

  HDESK desktop() const { return desktop_; }
  HWINSTA window_station() const { return window_station_; }

  
  void SetDesktop(HDESK desktop);

  
  
  void SetWindowStation(HWINSTA window_station);

  
  void Swap(WindowStationAndDesktop& other);

 private:
  HDESK desktop_;
  HWINSTA window_station_;

  DISALLOW_COPY_AND_ASSIGN(WindowStationAndDesktop);
};

}  

#endif  
