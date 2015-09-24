// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_TOP_CONTAINER_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_TOP_CONTAINER_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/view.h"

class BrowserView;

class TopContainerView : public views::View {
 public:
  explicit TopContainerView(BrowserView* browser_view);
  virtual ~TopContainerView();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual void OnPaintBackground(gfx::Canvas* canvas) OVERRIDE;

 private:
  
  BrowserView* browser_view_;

  DISALLOW_COPY_AND_ASSIGN(TopContainerView);
};

#endif  
