// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_DOWNLOAD_DOWNLOAD_ITEM_GTK_H_
#define CHROME_BROWSER_UI_GTK_DOWNLOAD_DOWNLOAD_ITEM_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "chrome/browser/download/download_item.h"
#include "chrome/browser/icon_manager.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/base/gtk/gtk_signal.h"

class BaseDownloadItemModel;
class DownloadShelfContextMenuGtk;
class DownloadShelfGtk;
class GtkThemeService;
class NineBox;
class SkBitmap;

namespace gfx {
class Image;
}

namespace ui {
class SlideAnimation;
}

class DownloadItemGtk : public DownloadItem::Observer,
                        public ui::AnimationDelegate,
                        public NotificationObserver {
 public:
  
  DownloadItemGtk(DownloadShelfGtk* parent_shelf,
                  BaseDownloadItemModel* download_item_model);

  
  ~DownloadItemGtk();

  
  virtual void OnDownloadUpdated(DownloadItem* download);
  virtual void OnDownloadOpened(DownloadItem* download) { }

  
  virtual void AnimationProgressed(const ui::Animation* animation);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void OnLoadSmallIconComplete(IconManager::Handle handle,
                               gfx::Image* image);
  void OnLoadLargeIconComplete(IconManager::Handle handle,
                               gfx::Image* image);

  
  DownloadItem* get_download();

 private:
  friend class DownloadShelfContextMenuGtk;

  
  bool IsDangerous();

  
  
  void UpdateDownloadProgress();

  
  void StartDownloadProgress();

  
  void StopDownloadProgress();

  
  void LoadIcon();

  
  void UpdateTooltip();

  
  void UpdateNameLabel();

  
  void UpdateStatusLabel(const std::string& status_text);

  
  void UpdateDangerWarning();

  
  void UpdateDangerIcon();

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

  
  OwnedWidgetGtk hbox_;

  
  
  OwnedWidgetGtk body_;

  
  GtkWidget* name_label_;

  
  GtkWidget* status_label_;

  
  std::string status_text_;

  
  GtkWidget* menu_button_;

  
  
  GtkWidget* arrow_;

  
  
  bool menu_showing_;

  
  GtkThemeService* theme_service_;

  
  
  OwnedWidgetGtk progress_area_;

  
  int progress_angle_;

  
  
  scoped_ptr<DownloadShelfContextMenuGtk> menu_;

  
  scoped_ptr<BaseDownloadItemModel> download_model_;

  
  GtkWidget* dangerous_prompt_;
  GtkWidget* dangerous_image_;
  GtkWidget* dangerous_label_;

  
  OwnedWidgetGtk dangerous_hbox_;
  int dangerous_hbox_start_width_;
  int dangerous_hbox_full_width_;

  
  scoped_ptr<ui::SlideAnimation> new_item_animation_;

  
  base::RepeatingTimer<DownloadItemGtk> progress_timer_;

  
  ui::SlideAnimation complete_animation_;

  
  
  
  gfx::Image* icon_small_;
  gfx::Image* icon_large_;

  
  FilePath icon_filepath_;

  NotificationRegistrar registrar_;

  
  base::Time creation_time_;

  
  CancelableRequestConsumerT<int, 0> icon_consumer_;

  
  
  bool download_complete_;
};

#endif  
