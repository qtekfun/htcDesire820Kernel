// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_WEB_DRAG_SOURCE_WIN_H_
#define CHROME_BROWSER_TAB_CONTENTS_WEB_DRAG_SOURCE_WIN_H_
#pragma once

#include "base/basictypes.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/dragdrop/drag_source.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"

class RenderViewHost;
class TabContents;

class WebDragSource : public ui::DragSource,
                      public NotificationObserver {
 public:
  
  WebDragSource(gfx::NativeWindow source_wnd, TabContents* tab_contents);
  virtual ~WebDragSource();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  void set_effect(DWORD effect) { effect_ = effect; }

 protected:
  
  virtual void OnDragSourceCancel();
  virtual void OnDragSourceDrop();
  virtual void OnDragSourceMove();

 private:
  
  WebDragSource();

  
  
  void DelayedOnDragSourceDrop();

  
  gfx::NativeWindow source_wnd_;

  
  
  
  RenderViewHost* render_view_host_;

  NotificationRegistrar registrar_;

  DWORD effect_;

  DISALLOW_COPY_AND_ASSIGN(WebDragSource);
};

#endif  
