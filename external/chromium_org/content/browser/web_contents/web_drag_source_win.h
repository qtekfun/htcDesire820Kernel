// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_DRAG_SOURCE_WIN_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_DRAG_SOURCE_WIN_H_

#include "base/basictypes.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/dragdrop/drag_source_win.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"

namespace ui {
class OSExchangeData;
}  

namespace content {
class RenderViewHost;
class WebContents;
class WebContentsImpl;

class WebDragSource : public ui::DragSourceWin,
                      public NotificationObserver {
 public:
  
  WebDragSource(gfx::NativeWindow source_wnd, WebContents* web_contents);
  virtual ~WebDragSource();

  
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  void set_effect(DWORD effect) { effect_ = effect; }
  
  
  
  void set_data(ui::OSExchangeData* data) { data_ = data; }

 protected:
  
  virtual void OnDragSourceCancel();
  virtual void OnDragSourceDrop();
  virtual void OnDragSourceMove();

 private:
  
  WebDragSource();

  
  
  void DelayedOnDragSourceDrop();

  
  gfx::NativeWindow source_wnd_;

  
  
  
  WebContentsImpl* web_contents_;

  NotificationRegistrar registrar_;

  DWORD effect_;

  ui::OSExchangeData* data_;

  DISALLOW_COPY_AND_ASSIGN(WebDragSource);
};

}  

#endif  
