// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_MOUSE_WATCHER_VIEW_HOST_H_
#define UI_VIEWS_MOUSE_WATCHER_VIEW_HOST_H_

#include "ui/views/mouse_watcher.h"

namespace views {

class View;

class VIEWS_EXPORT MouseWatcherViewHost : public MouseWatcherHost {
 public:
  
  
  
  
  
  MouseWatcherViewHost(View* view, const gfx::Insets& hot_zone_insets);
  virtual ~MouseWatcherViewHost();

  
  virtual bool Contains(const gfx::Point& screen_point,
                        MouseEventType type) OVERRIDE;

 private:
  bool IsCursorInViewZone(const gfx::Point& screen_point);
  bool IsMouseOverWindow();

  
  View* view_;
  
  const gfx::Insets hot_zone_insets_;

  DISALLOW_COPY_AND_ASSIGN(MouseWatcherViewHost);
};

}  

#endif  
