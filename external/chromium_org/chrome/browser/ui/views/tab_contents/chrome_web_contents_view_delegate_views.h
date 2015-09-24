// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_CHROME_WEB_CONTENTS_VIEW_DELEGATE_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_CHROME_WEB_CONTENTS_VIEW_DELEGATE_VIEWS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/web_contents_view_delegate.h"

class RenderViewContextMenuViews;

namespace content {
class WebContents;
class WebDragDestDelegate;
}

namespace views {
class FocusManager;
class Widget;
}

class ChromeWebContentsViewDelegateViews
    : public content::WebContentsViewDelegate {
 public:
  explicit ChromeWebContentsViewDelegateViews(
      content::WebContents* web_contents);
  virtual ~ChromeWebContentsViewDelegateViews();

  
  virtual content::WebDragDestDelegate* GetDragDestDelegate() OVERRIDE;
  virtual void StoreFocus() OVERRIDE;
  virtual void RestoreFocus() OVERRIDE;
  virtual bool Focus() OVERRIDE;
  virtual void TakeFocus(bool reverse) OVERRIDE;
  virtual void ShowContextMenu(
      const content::ContextMenuParams& params) OVERRIDE;
  virtual void SizeChanged(const gfx::Size& size) OVERRIDE;

 private:
  views::Widget* GetTopLevelWidget();
  views::FocusManager* GetFocusManager();
  void SetInitialFocus();

  
  int last_focused_view_storage_id_;

  
  
  scoped_ptr<RenderViewContextMenuViews> context_menu_;

  
  scoped_ptr<content::WebDragDestDelegate> bookmark_handler_;

  content::WebContents* web_contents_;

  DISALLOW_COPY_AND_ASSIGN(ChromeWebContentsViewDelegateViews);
};

#endif  
