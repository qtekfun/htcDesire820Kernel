// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TAB_CONTENTS_CHROME_WEB_CONTENTS_VIEW_DELEGATE_GTK_H_
#define CHROME_BROWSER_UI_GTK_TAB_CONTENTS_CHROME_WEB_CONTENTS_VIEW_DELEGATE_GTK_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/web_contents_view_delegate.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/scoped_gobject.h"

class RenderViewContextMenuGtk;
class WebDragBookmarkHandlerGtk;

namespace content {
class WebContents;
}

class ChromeWebContentsViewDelegateGtk
    : public content::WebContentsViewDelegate {
 public:
  explicit ChromeWebContentsViewDelegateGtk(content::WebContents* web_contents);
  virtual ~ChromeWebContentsViewDelegateGtk();

  static ChromeWebContentsViewDelegateGtk* GetFor(
      content::WebContents* web_contents);

  GtkWidget* expanded_container() { return expanded_container_; }
  ui::FocusStoreGtk* focus_store() { return focus_store_; }

  
  
  void AttachWebContentsModalDialog(GtkWidget* web_contents_modal_dialog);
  void RemoveWebContentsModalDialog(GtkWidget* web_contents_modal_dialog);

  
  virtual void ShowContextMenu(
      const content::ContextMenuParams& params) OVERRIDE;
  virtual content::WebDragDestDelegate* GetDragDestDelegate() OVERRIDE;
  virtual void Initialize(GtkWidget* expanded_container,
                          ui::FocusStoreGtk* focus_store) OVERRIDE;
  virtual gfx::NativeView GetNativeView() const OVERRIDE;
  virtual void Focus() OVERRIDE;
  virtual gboolean OnNativeViewFocusEvent(GtkWidget* widget,
                                          GtkDirectionType type,
                                          gboolean* return_value) OVERRIDE;

 private:
  
  CHROMEGTK_CALLBACK_1(ChromeWebContentsViewDelegateGtk, void,
                       OnSetFloatingPosition,
                       GtkAllocation*);

  
  ui::ScopedGObject<GtkWidget>::Type floating_;

  
  
  GtkWidget* web_contents_modal_dialog_;

  
  
  scoped_ptr<RenderViewContextMenuGtk> context_menu_;

  
  scoped_ptr<WebDragBookmarkHandlerGtk> bookmark_handler_gtk_;

  content::WebContents* web_contents_;

  GtkWidget* expanded_container_;
  ui::FocusStoreGtk* focus_store_;
};

#endif  
