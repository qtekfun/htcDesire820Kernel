// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_INFOBARS_INFOBAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_INFOBARS_INFOBAR_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/infobars/infobar.h"
#include "chrome/browser/infobars/infobar_delegate.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"

class CustomDrawButton;
class GtkThemeService;

namespace ui {
class GtkSignalRegistrar;
class MenuModel;
}

class InfoBarGtk : public InfoBar,
                   public content::NotificationObserver {
 public:
  
  typedef void (InfoBarGtk::*ColorGetter)(InfoBarDelegate::Type,
                                          double* r, double* g, double* b);

  explicit InfoBarGtk(scoped_ptr<InfoBarDelegate> delegate);
  virtual ~InfoBarGtk();

  
  GtkWidget* widget() { return widget_.get(); }

  GdkColor GetBorderColor() const;

  
  
  
  int AnimatingHeight() const;

  SkColor ConvertGetColor(ColorGetter getter);

  
  
  virtual void GetTopColor(InfoBarDelegate::Type type,
                           double* r, double* g, double* b);
  virtual void GetBottomColor(InfoBarDelegate::Type type,
                              double* r, double* g, double* b);

 protected:
  
  static const int kEndOfLabelSpacing;

  

  
  
  
  
  
  
  
  virtual void PlatformSpecificSetOwner() OVERRIDE;

  virtual void PlatformSpecificShow(bool animate) OVERRIDE;
  virtual void PlatformSpecificOnCloseSoon() OVERRIDE;
  virtual void PlatformSpecificOnHeightsRecalculated() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void ForceCloseButtonToUseChromeTheme();

  GtkWidget* hbox() { return hbox_; }

  
  
  ui::GtkSignalRegistrar* signals() { return signals_.get(); }

  
  
  
  GtkWidget* CreateLabel(const std::string& text);

  
  GtkWidget* CreateLinkButton(const std::string& text);

  
  
  static GtkWidget* CreateMenuButton(const std::string& text);

  
  
  
  
  void AddLabelWithInlineLink(const base::string16& display_text,
                              const base::string16& link_text,
                              size_t link_offset,
                              GCallback callback);

  
  void ShowMenuWithModel(GtkWidget* sender,
                         MenuGtk::Delegate* delegate,
                         ui::MenuModel* model);

 private:
  void GetBackgroundColor(SkColor color, double* r, double* g, double* b);
  void UpdateBorderColor();

  CHROMEGTK_CALLBACK_0(InfoBarGtk, void, OnCloseButton);
  CHROMEGTK_CALLBACK_1(InfoBarGtk, gboolean, OnBackgroundExpose,
                       GdkEventExpose*);
  CHROMEGTK_CALLBACK_2(InfoBarGtk, void, OnChildSizeRequest, GtkWidget*,
                       GtkRequisition*);

  
  
  ui::OwnedWidgetGtk widget_;

  
  GtkWidget* bg_box_;

  
  GtkWidget* hbox_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  
  GtkThemeService* theme_service_;

  content::NotificationRegistrar registrar_;

  
  scoped_ptr<ui::GtkSignalRegistrar> signals_;

  
  
  scoped_ptr<MenuGtk> menu_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarGtk);
};

#endif  
