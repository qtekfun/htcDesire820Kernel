// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <windows.h>

#ifndef CHROME_FRAME_EVENT_HOOKER_H_
#define CHROME_FRAME_EVENT_HOOKER_H_

class EventHooker {
 public:
  EventHooker();
  ~EventHooker();

  
  
  
  bool StartHook();

  
  void StopHook();

  
  static VOID CALLBACK WindowCreationHookProc(HWINEVENTHOOK hook,
                                              DWORD event,
                                              HWND window,
                                              LONG object_id,
                                              LONG child_id,
                                              DWORD event_tid,
                                              DWORD event_time);

 private:
  HWINEVENTHOOK window_creation_hook_;
};

#endif  
