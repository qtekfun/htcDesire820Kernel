// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_BHO_LOADER_H_
#define CHROME_FRAME_BHO_LOADER_H_

#include <windows.h>

class EventHooker;

class BHOLoader {
 public:
  BHOLoader();
  ~BHOLoader();

  
  void OnHookEvent(DWORD event, HWND window);

  
  
  
  bool StartHook();

  
  void StopHook();

  
  static BHOLoader* GetInstance();

 private:
  EventHooker* hooker_;
};

#endif  
