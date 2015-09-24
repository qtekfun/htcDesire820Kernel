// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_STATUS_BUBBLE_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_STATUS_BUBBLE_VIEWS_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/task.h"
#include "chrome/browser/ui/status_bubble.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/rect.h"

class GURL;
namespace gfx {
class Point;
}
namespace views {
class View;
class Widget;
}

class StatusBubbleViews : public StatusBubble {
 public:
  
  static const int kShadowThickness;

  
  static const int kTotalVerticalPadding = 7;

  
  explicit StatusBubbleViews(views::View* base_view);
  ~StatusBubbleViews();

  views::View* base_view() { return base_view_; }

  
  
  void Reposition();

  
  
  gfx::Size GetPreferredSize();

  
  void SetBounds(int x, int y, int w, int h);

  
  void SetBubbleWidth(int width);

  
  virtual void SetStatus(const string16& status);
  virtual void SetURL(const GURL& url, const string16& languages);
  virtual void Hide();
  virtual void MouseMoved(const gfx::Point& location, bool left_content);
  virtual void UpdateDownloadShelfVisibility(bool visible);

 private:
  class StatusView;
  class StatusViewExpander;

  
  void Init();

  
  
  void AvoidMouse(const gfx::Point& location);

  
  bool IsFrameVisible();

  
  void ExpandBubble();

  
  void CancelExpandTimer();

  
  int GetStandardStatusBubbleWidth();

  
  
  int GetMaxStatusBubbleWidth();

  
  string16 status_text_;

  
  string16 url_text_;

  
  GURL url_;

  
  string16 languages_;

  
  gfx::Point original_position_;
  
  gfx::Point position_;
  gfx::Size size_;

  
  int offset_;

  
  
  scoped_ptr<views::Widget> popup_;
  double opacity_;

  views::View* base_view_;
  StatusView* view_;

  
  scoped_ptr<StatusViewExpander> expand_view_;

  
  bool download_shelf_is_visible_;

  
  
  bool is_expanded_;

  
  ScopedRunnableMethodFactory<StatusBubbleViews> expand_timer_factory_;

  DISALLOW_COPY_AND_ASSIGN(StatusBubbleViews);
};

#endif  
