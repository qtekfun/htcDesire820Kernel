// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_INFOBARS_EXTENSION_INFOBAR_H_
#define CHROME_BROWSER_UI_VIEWS_INFOBARS_EXTENSION_INFOBAR_H_

#include "base/compiler_specific.h"
#include "chrome/browser/ui/views/infobars/infobar_view.h"
#include "ui/views/controls/button/menu_button_listener.h"

class Browser;
class ExtensionInfoBarDelegate;

namespace views {
class ImageView;
class MenuButton;
}

class ExtensionInfoBar : public InfoBarView,
                         public views::MenuButtonListener {
 public:
  ExtensionInfoBar(scoped_ptr<ExtensionInfoBarDelegate> delegate,
                   Browser* browser);

 private:
  virtual ~ExtensionInfoBar();

  
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual int ContentMinimumWidth() const OVERRIDE;

  
  virtual void OnMenuButtonClicked(views::View* source,
                                   const gfx::Point& point) OVERRIDE;

  void OnImageLoaded(const gfx::Image& image);

  ExtensionInfoBarDelegate* GetDelegate();

  Browser* browser_;

  
  
  
  
  
  views::View* infobar_icon_;

  
  
  
  views::MenuButton* icon_as_menu_;

  
  
  
  views::ImageView* icon_as_image_;

  base::WeakPtrFactory<ExtensionInfoBar> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInfoBar);
};

#endif  
