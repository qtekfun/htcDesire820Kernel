// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_NOTIFICATIONS_BALLOON_VIEW_HOST_H_
#define CHROME_BROWSER_UI_VIEWS_NOTIFICATIONS_BALLOON_VIEW_HOST_H_
#pragma once

#include "chrome/browser/notifications/balloon_host.h"
#include "views/controls/native/native_view_host.h"

class BalloonViewHost : public BalloonHost {
 public:
  explicit BalloonViewHost(Balloon* balloon);

  virtual ~BalloonViewHost() {
     Shutdown();
  }

  void SetPreferredSize(const gfx::Size& size) {
    native_host_->SetPreferredSize(size);
  }

  
  views::View* view() {
    return native_host_;
  }

  gfx::NativeView native_view() const {
    return native_host_->native_view();
  }

  
  void Init(gfx::NativeView parent);

 protected:
  virtual void InitRenderWidgetHostView();
  virtual RenderWidgetHostView* render_widget_host_view() const;

 private:
  
  RenderWidgetHostView* render_widget_host_view_;

  
  views::NativeViewHost* native_host_;

  
  gfx::NativeView parent_native_view_;

  DISALLOW_COPY_AND_ASSIGN(BalloonViewHost);
};

#endif  
