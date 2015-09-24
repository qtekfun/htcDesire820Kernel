// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PANELS_TASKBAR_WINDOW_THUMBNAILER_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_PANELS_TASKBAR_WINDOW_THUMBNAILER_WIN_H_

#include <vector>
#include "base/memory/scoped_ptr.h"
#include "ui/base/win/hwnd_subclass.h"

class SkBitmap;

class TaskbarWindowThumbnailerDelegateWin {
 public:
  
  
  
  virtual std::vector<HWND> GetSnapshotWindowHandles() const = 0;
};

class TaskbarWindowThumbnailerWin : public ui::HWNDMessageFilter {
 public:
  TaskbarWindowThumbnailerWin(HWND hwnd,
                              TaskbarWindowThumbnailerDelegateWin* delegate);
  virtual ~TaskbarWindowThumbnailerWin();

  
  
  
  void Start();

  
  void Stop();

  
  void CaptureSnapshot();

  
  
  void InvalidateSnapshot();

  
  
  void ReplaceWindow(HWND new_hwnd);

 private:
  
  virtual bool FilterMessage(HWND hwnd,
                             UINT message,
                             WPARAM w_param,
                             LPARAM l_param,
                             LRESULT* l_result) OVERRIDE;

  
  bool OnDwmSendIconicThumbnail(int width, int height, LRESULT* l_result);
  bool OnDwmSendIconicLivePreviewBitmap(LRESULT* l_result);

  
  
  SkBitmap* CaptureWindowImage() const;

  HWND hwnd_;
  TaskbarWindowThumbnailerDelegateWin* delegate_;  
  scoped_ptr<SkBitmap> capture_bitmap_;

  DISALLOW_COPY_AND_ASSIGN(TaskbarWindowThumbnailerWin);
};

#endif  
