// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FRAME_DOM_BROWSER_VIEW_LAYOUT_H_
#define CHROME_BROWSER_CHROMEOS_FRAME_DOM_BROWSER_VIEW_LAYOUT_H_
#pragma once

#include "base/compiler_specific.h"
#include "chrome/browser/chromeos/frame/dom_browser_view.h"
#include "chrome/browser/chromeos/status/status_area_host.h"
#include "chrome/browser/ui/views/frame/browser_view_layout.h"

namespace chromeos {

class DOMBrowserView;
class StatusAreaView;


class DOMBrowserViewLayout : public ::BrowserViewLayout {
 public:
  DOMBrowserViewLayout();
  virtual ~DOMBrowserViewLayout();

  
  virtual void Installed(views::View* host) OVERRIDE;
  virtual void ViewAdded(views::View* host,
                         views::View* view) OVERRIDE;
  virtual bool IsPositionInWindowCaption(const gfx::Point& point) OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;

 protected:
  
  virtual int LayoutTabStrip() OVERRIDE;
  virtual int LayoutToolbar(int top) OVERRIDE;
  virtual int LayoutBookmarkAndInfoBars(int top) OVERRIDE;

 private:
  const DOMBrowserView* GetDOMBrowserView();
  StatusAreaView* status_area_;

  
  
  bool IsPointInViewsInTitleArea(const gfx::Point& point) const;

  
  
  int LayoutTitlebarComponents(const gfx::Rect& bounds);

  DISALLOW_COPY_AND_ASSIGN(DOMBrowserViewLayout);
};

}  

#endif  
