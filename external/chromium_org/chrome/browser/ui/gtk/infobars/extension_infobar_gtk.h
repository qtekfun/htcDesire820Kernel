// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_INFOBARS_EXTENSION_INFOBAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_INFOBARS_EXTENSION_INFOBAR_GTK_H_

#include "base/compiler_specific.h"
#include "chrome/browser/extensions/extension_infobar_delegate.h"
#include "chrome/browser/ui/gtk/extensions/extension_view_gtk.h"
#include "chrome/browser/ui/gtk/infobars/infobar_gtk.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "ui/gfx/gtk_util.h"

class ExtensionContextMenuModel;
class ExtensionViewGtk;
class MenuGtk;

class ExtensionInfoBarGtk : public InfoBarGtk, public MenuGtk::Delegate {
 public:
  explicit ExtensionInfoBarGtk(scoped_ptr<ExtensionInfoBarDelegate> delegate);

 private:
  virtual ~ExtensionInfoBarGtk();

  
  virtual void PlatformSpecificSetOwner() OVERRIDE;
  virtual void PlatformSpecificHide(bool animate) OVERRIDE;
  virtual void GetTopColor(InfoBarDelegate::Type type,
                           double* r, double* g, double* b) OVERRIDE;
  virtual void GetBottomColor(InfoBarDelegate::Type type,
                              double* r, double* g, double* b) OVERRIDE;

  
  virtual void StoppedShowing() OVERRIDE;

  void OnImageLoaded(const gfx::Image& image);

  ExtensionInfoBarDelegate* GetDelegate();

  
  
  Browser* GetBrowser();

  
  
  ExtensionContextMenuModel* BuildMenuModel();

  CHROMEGTK_CALLBACK_1(ExtensionInfoBarGtk, void, OnSizeAllocate,
                       GtkAllocation*);

  CHROMEGTK_CALLBACK_1(ExtensionInfoBarGtk, gboolean, OnButtonPress,
                       GdkEventButton*);

  CHROMEGTK_CALLBACK_1(ExtensionInfoBarGtk, gboolean, OnExpose,
                       GdkEventExpose*);

  ExtensionViewGtk* view_;

  
  
  
  GtkWidget* button_;

  
  
  
  GtkWidget* icon_;

  
  
  
  
  GtkWidget* alignment_;

  
  scoped_refptr<ExtensionContextMenuModel> context_menu_model_;

  base::WeakPtrFactory<ExtensionInfoBarGtk> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInfoBarGtk);
};

#endif  
