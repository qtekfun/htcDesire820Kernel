// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_NOTIFICATIONS_BALLOON_VIEW_HOST_GTK_H_
#define CHROME_BROWSER_UI_GTK_NOTIFICATIONS_BALLOON_VIEW_HOST_GTK_H_
#pragma once

#include "chrome/browser/notifications/balloon_host.h"
#include "ui/gfx/native_widget_types.h"

class RenderWidgetHostViewGtk;

class BalloonViewHost : public BalloonHost {
 public:
  explicit BalloonViewHost(Balloon* balloon);

  ~BalloonViewHost() {
    Shutdown();
  }

  
  void UpdateActualSize(const gfx::Size& new_size);

  
  gfx::NativeView native_view() const;

 protected:
  virtual void InitRenderWidgetHostView();
  virtual RenderWidgetHostView* render_widget_host_view() const;

 private:
  
  RenderWidgetHostViewGtk* render_widget_host_view_;

  DISALLOW_COPY_AND_ASSIGN(BalloonViewHost);
};

#endif  