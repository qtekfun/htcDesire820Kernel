// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_WEB_DROP_TARGET_WIN_H_
#define CHROME_BROWSER_TAB_CONTENTS_WEB_DROP_TARGET_WIN_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebDragOperation.h"
#include "ui/base/dragdrop/drop_target.h"

class InterstitialDropTarget;
class RenderViewHost;
class TabContents;

class WebDropTarget : public ui::DropTarget {
 public:
  
  
  WebDropTarget(HWND source_hwnd, TabContents* contents);
  virtual ~WebDropTarget();

  void set_drag_cursor(WebKit::WebDragOperation op) {
    drag_cursor_ = op;
  }

 protected:
  virtual DWORD OnDragEnter(IDataObject* data_object,
                            DWORD key_state,
                            POINT cursor_position,
                            DWORD effect);

  virtual DWORD OnDragOver(IDataObject* data_object,
                           DWORD key_state,
                           POINT cursor_position,
                           DWORD effect);

  virtual void OnDragLeave(IDataObject* data_object);

  virtual DWORD OnDrop(IDataObject* data_object,
                       DWORD key_state,
                       POINT cursor_position,
                       DWORD effect);

 private:
  
  TabContents* tab_contents_;

  
  
  
  
  RenderViewHost* current_rvh_;

  
  
  WebKit::WebDragOperation drag_cursor_;

  
  
  scoped_ptr<InterstitialDropTarget> interstitial_drop_target_;

  DISALLOW_COPY_AND_ASSIGN(WebDropTarget);
};

#endif  
