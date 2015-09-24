// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_WIN_EVENT_RECEIVER_H_
#define CHROME_FRAME_TEST_WIN_EVENT_RECEIVER_H_

#include <windows.h>

#include <string>
#include <vector>
#include <utility>

#include "base/memory/linked_ptr.h"
#include "base/win/object_watcher.h"

struct FunctionStub;

class WinEventListener {
 public:
  virtual ~WinEventListener() {}
  
  
  virtual void OnEventReceived(DWORD event, HWND hwnd, LONG object_id,
                               LONG child_id) = 0;
};

class WinEventReceiver {
 public:
  WinEventReceiver();
  ~WinEventReceiver();

  
  
  
  void SetListenerForEvent(WinEventListener* listener, DWORD event);

  
  void SetListenerForEvents(WinEventListener* listener, DWORD event_min,
                            DWORD event_max);

  
  
  void StopReceivingEvents();

 private:
  bool InitializeHook(DWORD event_min, DWORD event_max);

  static void CALLBACK WinEventHook(WinEventReceiver* me, HWINEVENTHOOK hook,
      DWORD event, HWND hwnd, LONG object_id, LONG child_id,
      DWORD event_thread_id, DWORD event_time);

  WinEventListener* listener_;
  HWINEVENTHOOK hook_;
  FunctionStub* hook_stub_;
};

class WindowObserver {
 public:
  virtual ~WindowObserver() {}
  virtual void OnWindowOpen(HWND hwnd) = 0;
  virtual void OnWindowClose(HWND hwnd) = 0;
};

class WindowWatchdog : public WinEventListener {
 public:
  WindowWatchdog();
  
  
  
  
  
  
  void AddObserver(WindowObserver* observer,
                   const std::string& caption_pattern,
                   const std::string& class_name_pattern);

  
  
  void RemoveObserver(WindowObserver* observer);

 private:
  class ProcessExitObserver;

  
  
  
  typedef std::pair<HWND, linked_ptr<base::win::ObjectWatcher::Delegate> >
      OpenWindowEntry;
  typedef std::vector<OpenWindowEntry> OpenWindowList;

  struct ObserverEntry {
    WindowObserver* observer;
    std::string caption_pattern;
    std::string class_name_pattern;
    OpenWindowList open_windows;
  };

  typedef std::vector<ObserverEntry> ObserverEntryList;

  
  virtual void OnEventReceived(
      DWORD event, HWND hwnd, LONG object_id, LONG child_id);

  static std::string GetWindowCaption(HWND hwnd);

  void HandleOnOpen(HWND hwnd);
  void HandleOnClose(HWND hwnd);
  void OnHwndProcessExited(HWND hwnd);

  
  
  bool MatchingWindow(const ObserverEntry& entry,
                      const std::string& caption,
                      const std::string& class_name);

  ObserverEntryList observers_;
  WinEventReceiver win_event_receiver_;

  DISALLOW_COPY_AND_ASSIGN(WindowWatchdog);
};



#endif  
