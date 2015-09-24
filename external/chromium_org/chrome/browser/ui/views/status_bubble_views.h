// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_STATUS_BUBBLE_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_STATUS_BUBBLE_VIEWS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/status_bubble.h"
#include "ui/gfx/rect.h"
#include "url/gurl.h"

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
  virtual ~StatusBubbleViews();

  views::View* base_view() { return base_view_; }

  
  
  virtual void Reposition();

  
  
  gfx::Size GetPreferredSize();

  
  void SetBounds(int x, int y, int w, int h);

  
  void SetBubbleWidth(int width);

  
  virtual void SetStatus(const base::string16& status) OVERRIDE;
  virtual void SetURL(const GURL& url, const std::string& languages) OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void MouseMoved(const gfx::Point& location,
                          bool left_content) OVERRIDE;
  virtual void UpdateDownloadShelfVisibility(bool visible) OVERRIDE;

  views::Widget* GetPopupForTest() { return popup(); }

 protected:
  views::Widget* popup() { return popup_.get(); }

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

  
  base::string16 status_text_;

  
  base::string16 url_text_;

  
  GURL url_;

  
  std::string languages_;

  
  gfx::Point original_position_;
  
  gfx::Point position_;
  gfx::Size size_;

  
  gfx::Point last_mouse_moved_location_;

  
  bool contains_mouse_;

  
  int offset_;

  
  
  scoped_ptr<views::Widget> popup_;
  double opacity_;

  views::View* base_view_;
  StatusView* view_;

  
  scoped_ptr<StatusViewExpander> expand_view_;

  
  bool download_shelf_is_visible_;

  
  
  bool is_expanded_;

  
  base::WeakPtrFactory<StatusBubbleViews> expand_timer_factory_;

  DISALLOW_COPY_AND_ASSIGN(StatusBubbleViews);
};

#endif  
