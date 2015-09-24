// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_DOWNLOAD_DOWNLOAD_ITEM_GTK_H_
#define CHROME_BROWSER_UI_GTK_DOWNLOAD_DOWNLOAD_ITEM_GTK_H_

#include <gtk/gtk.h>

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/download/download_item_model.h"
#include "chrome/browser/icon_manager.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"

class DownloadShelfContextMenuGtk;
class DownloadShelfGtk;
class GtkThemeService;
class NineBox;

namespace gfx {
class Image;
class SlideAnimation;
}

class DownloadItemGtk : public content::DownloadItem::Observer,
                        public gfx::AnimationDelegate,
                        public content::NotificationObserver {
 public:
  
  DownloadItemGtk(DownloadShelfGtk* parent_shelf,
                  content::DownloadItem* download_item);

  
  virtual ~DownloadItemGtk();

  
  virtual void OnDownloadUpdated(content::DownloadItem* download) OVERRIDE;
  virtual void OnDownloadDestroyed(content::DownloadItem* download) OVERRIDE;
  virtual void OnDownloadOpened(content::DownloadItem* download) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void OnLoadSmallIconComplete(gfx::Image* image);
  void OnLoadLargeIconComplete(gfx::Image* image);

  
  content::DownloadItem* download() { return download_model_.download(); }

 private:
  friend class DownloadShelfContextMenuGtk;

  
  
  void UpdateDownloadProgress();

  
  void StartDownloadProgress();

  
  void StopDownloadProgress();

  
  void LoadIcon();

  
  void UpdateTooltip();

  
  void UpdateNameLabel();

  
  void UpdateStatusLabel(const std::string& status_text);

  
  void UpdateDangerWarning();

  
  void UpdateDangerIcon();

  
  void ReenableHbox();

  static void InitNineBoxes();

  
  
  void ShowPopupMenu(GtkWidget* button, GdkEventButton* event);

  
  CHROMEGTK_CALLBACK_1(DownloadItemGtk, gboolean, OnHboxExpose,
                       GdkEventExpose*);

  
  CHROMEGTK_CALLBACK_1(DownloadItemGtk, gboolean, OnExpose, GdkEventExpose*);

  
  CHROMEGTK_CALLBACK_0(DownloadItemGtk, void, OnClick);

  
  
  CHROMEGTK_CALLBACK_1(DownloadItemGtk, gboolean, OnButtonPress,
                       GdkEventButton*);

  
  CHROMEGTK_CALLBACK_1(DownloadItemGtk, gboolean, OnProgressAreaExpose,
                       GdkEventExpose*);

  CHROMEGTK_CALLBACK_1(DownloadItemGtk, gboolean, OnMenuButtonPressEvent,
                       GdkEventButton*);

  
  CHROMEGTK_CALLBACK_1(DownloadItemGtk, gboolean, OnDangerousPromptExpose,
                       GdkEventExpose*);
  CHROMEGTK_CALLBACK_0(DownloadItemGtk, void, OnDangerousAccept);
  CHROMEGTK_CALLBACK_0(DownloadItemGtk, void, OnDangerousDecline);

  
  static NineBox* body_nine_box_normal_;
  static NineBox* body_nine_box_prelight_;
  static NineBox* body_nine_box_active_;

  
  static NineBox* menu_nine_box_normal_;
  static NineBox* menu_nine_box_prelight_;
  static NineBox* menu_nine_box_active_;

  
  static NineBox* dangerous_nine_box_;

  
  DownloadShelfGtk* parent_shelf_;

  
  ui::OwnedWidgetGtk hbox_;

  
  
  ui::OwnedWidgetGtk body_;

  
  GtkWidget* text_stack_;

  
  GtkWidget* name_label_;

  
  GtkWidget* status_label_;

  
  std::string status_text_;

  
  GtkWidget* menu_button_;

  
  
  GtkWidget* arrow_;

  
  
  bool menu_showing_;

  
  GtkThemeService* theme_service_;

  
  
  ui::OwnedWidgetGtk progress_area_;

  
  int progress_angle_;

  
  
  scoped_ptr<DownloadShelfContextMenuGtk> menu_;

  
  DownloadItemModel download_model_;

  
  GtkWidget* dangerous_prompt_;
  GtkWidget* dangerous_image_;
  GtkWidget* dangerous_label_;

  
  ui::OwnedWidgetGtk dangerous_hbox_;
  int dangerous_hbox_start_width_;
  int dangerous_hbox_full_width_;

  
  scoped_ptr<gfx::SlideAnimation> new_item_animation_;

  
  base::RepeatingTimer<DownloadItemGtk> progress_timer_;

  
  gfx::SlideAnimation complete_animation_;

  
  
  
  gfx::Image* icon_small_;
  gfx::Image* icon_large_;

  
  base::FilePath icon_filepath_;

  content::NotificationRegistrar registrar_;

  
  base::Time creation_time_;

  
  CancelableTaskTracker cancelable_task_tracker_;

  
  
  bool download_complete_;

  
  bool disabled_while_opening_;

  
  
  base::WeakPtrFactory<DownloadItemGtk> weak_ptr_factory_;
};

#endif  
