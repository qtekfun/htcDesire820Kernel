// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_INFOBARS_EXTENSION_INFOBAR_H_
#define CHROME_BROWSER_UI_VIEWS_INFOBARS_EXTENSION_INFOBAR_H_
#pragma once

#include "chrome/browser/extensions/extension_infobar_delegate.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/browser/ui/views/extensions/extension_view.h"
#include "chrome/browser/ui/views/infobars/infobar_view.h"
#include "views/controls/menu/view_menu_delegate.h"

class ExtensionContextMenuModel;
namespace views {
class MenuButton;
class Menu2;
}

class ExtensionInfoBar : public InfoBarView,
                         public ExtensionView::Container,
                         public ImageLoadingTracker::Observer,
                         public ExtensionInfoBarDelegate::DelegateObserver,
                         public views::ViewMenuDelegate {
 public:
  explicit ExtensionInfoBar(ExtensionInfoBarDelegate* delegate);

 private:
  virtual ~ExtensionInfoBar();

  
  virtual void Layout();
  virtual void ViewHierarchyChanged(bool is_add, View* parent, View* child);
  virtual int ContentMinimumWidth() const;

  
  virtual void OnExtensionMouseMove(ExtensionView* view);
  virtual void OnExtensionMouseLeave(ExtensionView* view);
  virtual void OnExtensionPreferredSizeChanged(ExtensionView* view);

  
  virtual void OnImageLoaded(SkBitmap* image,
                             const ExtensionResource& resource,
                             int index);

  
  virtual void OnDelegateDeleted();

  
  virtual void RunMenu(View* source, const gfx::Point& pt);

  ExtensionInfoBarDelegate* GetDelegate();

  
  
  
  InfoBarDelegate* delegate_;

  
  scoped_refptr<ExtensionContextMenuModel> options_menu_contents_;
  scoped_ptr<views::Menu2> options_menu_menu_;
  views::MenuButton* menu_;

  
  ImageLoadingTracker tracker_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInfoBar);
};

#endif  
