// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_INFOBARS_INFOBAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_INFOBARS_INFOBAR_GTK_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/tab_contents/infobar_delegate.h"
#include "chrome/browser/ui/gtk/infobars/infobar_arrow_model.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "chrome/browser/ui/gtk/slide_animator_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "third_party/skia/include/core/SkPaint.h"
#include "ui/base/gtk/gtk_signal.h"

class CustomDrawButton;
class GtkThemeService;
class InfoBarContainerGtk;
class InfoBarDelegate;

class InfoBar : public SlideAnimatorGtk::Delegate,
                public NotificationObserver,
                public InfoBarArrowModel::Observer {
 public:
  explicit InfoBar(InfoBarDelegate* delegate);
  virtual ~InfoBar();

  InfoBarDelegate* delegate() const { return delegate_; }

  
  GtkWidget* widget();

  
  
  void set_container(InfoBarContainerGtk* container) { container_ = container; }

  
  void AnimateOpen();

  
  void Open();

  
  
  void AnimateClose();

  
  
  
  void Close();

  
  bool IsAnimating();

  
  bool IsClosing();

  void SetThemeProvider(GtkThemeService* theme_provider);

  
  
  void ShowArrowFor(InfoBar* other, bool animate);

  
  virtual void PaintStateChanged();

  
  virtual void Closed();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  virtual void GetTopColor(InfoBarDelegate::Type type,
                           double* r, double* g, double *b);
  virtual void GetBottomColor(InfoBarDelegate::Type type,
                              double* r, double* g, double *b);

  
  static const int kInfoBarHeight;

 protected:
  
  static const int kEndOfLabelSpacing;
  
  static const int kButtonButtonSpacing;

  
  
  void RemoveInfoBar() const;

  
  
  
  
  void AddLabelWithInlineLink(const string16& display_text,
                              const string16& link_text,
                              size_t link_offset,
                              GCallback callback);

  
  scoped_ptr<SlideAnimatorGtk> slide_widget_;

  
  GtkWidget* bg_box_;

  
  GtkWidget* hbox_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  
  InfoBarContainerGtk* container_;

  
  InfoBarDelegate* delegate_;

  
  GtkThemeService* theme_service_;

  
  
  InfoBarArrowModel arrow_model_;

  NotificationRegistrar registrar_;

 private:
  CHROMEGTK_CALLBACK_0(InfoBar, void, OnCloseButton);
  CHROMEGTK_CALLBACK_1(InfoBar, gboolean, OnBackgroundExpose, GdkEventExpose*);

  void UpdateBorderColor();

  DISALLOW_COPY_AND_ASSIGN(InfoBar);
};

#endif  
