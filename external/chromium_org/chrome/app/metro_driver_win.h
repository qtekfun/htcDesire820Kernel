// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_APP_METRO_DRIVER_WIN_H_
#define CHROME_APP_METRO_DRIVER_WIN_H_

#include <Windows.h>

class MetroDriver {
 public:
  typedef int (*MainFn)(HINSTANCE instance);

  MetroDriver();
  
  
  bool in_metro_mode() const {  return (NULL != init_metro_fn_); }

  
  
  
  
  int RunInMetro(HINSTANCE instance, MainFn main_fn);

 private:
  void* init_metro_fn_;
};

#endif  
