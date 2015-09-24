// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_DRAG_DEST_WIN_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_DRAG_DEST_WIN_H_

#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "content/public/common/drop_data.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"
#include "ui/base/dragdrop/drop_target_win.h"

namespace content {
class InterstitialDropTarget;
class RenderViewHost;
class WebContents;
class WebDragDestDelegate;

class CONTENT_EXPORT WebDragDest : public ui::DropTargetWin {
 public:
  
  
  WebDragDest(HWND source_hwnd, WebContents* contents);
  virtual ~WebDragDest();

  DropData* current_drop_data() const { return drop_data_.get(); }

  void set_drag_cursor(blink::WebDragOperation op) {
    drag_cursor_ = op;
  }

  WebDragDestDelegate* delegate() const { return delegate_; }
  void set_delegate(WebDragDestDelegate* d) { delegate_ = d; }

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
  
  WebContents* web_contents_;

  
  
  
  
  RenderViewHost* current_rvh_;

  
  
  blink::WebDragOperation drag_cursor_;

  
  
  scoped_ptr<InterstitialDropTarget> interstitial_drop_target_;

  
  WebDragDestDelegate* delegate_;

  
  scoped_ptr<DropData> drop_data_;

  
  bool canceled_;

  DISALLOW_COPY_AND_ASSIGN(WebDragDest);
};

}  

#endif  
