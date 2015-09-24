// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_GTK_INFO_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_INFO_BUBBLE_GTK_H_
#pragma once

#include <gtk/gtk.h>
#include <vector>

#include "base/basictypes.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

class GtkThemeService;
class InfoBubbleGtk;
namespace gfx {
class Rect;
}

class InfoBubbleGtkDelegate {
 public:
  
  
  virtual void InfoBubbleClosing(InfoBubbleGtk* info_bubble,
                                 bool closed_by_escape) = 0;

  
  

 protected:
  virtual ~InfoBubbleGtkDelegate() {}
};

class InfoBubbleGtk : public NotificationObserver {
 public:
  
  enum ArrowLocationGtk {
    
    ARROW_LOCATION_TOP_LEFT,
    ARROW_LOCATION_TOP_RIGHT,
  };

  
  
  
  
  
  
  
  
  static InfoBubbleGtk* Show(GtkWidget* anchor_widget,
                             const gfx::Rect* rect,
                             GtkWidget* content,
                             ArrowLocationGtk arrow_location,
                             bool match_system_theme,
                             bool grab_input,
                             GtkThemeService* provider,
                             InfoBubbleGtkDelegate* delegate);

  
  
  void Close();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  
  
  
  
  
  void HandlePointerAndKeyboardUngrabbedByContent();

 private:
  enum FrameType {
    FRAME_MASK,
    FRAME_STROKE,
  };

  explicit InfoBubbleGtk(GtkThemeService* provider, bool match_system_theme);
  virtual ~InfoBubbleGtk();

  
  void Init(GtkWidget* anchor_widget,
            const gfx::Rect* rect,
            GtkWidget* content,
            ArrowLocationGtk arrow_location,
            bool grab_input);

  
  
  static std::vector<GdkPoint> MakeFramePolygonPoints(
      ArrowLocationGtk arrow_location,
      int width,
      int height,
      FrameType type);

  
  
  
  
  
  static ArrowLocationGtk GetArrowLocation(
      ArrowLocationGtk preferred_location, int arrow_x, int width);

  
  
  
  bool UpdateArrowLocation(bool force_move_and_reshape);

  
  void UpdateWindowShape();

  
  
  
  void MoveWindow();

  
  void StackWindow();

  
  void set_delegate(InfoBubbleGtkDelegate* delegate) { delegate_ = delegate; }

  
  
  void GrabPointerAndKeyboard();

  CHROMEG_CALLBACK_3(InfoBubbleGtk, gboolean, OnGtkAccelerator, GtkAccelGroup*,
                     GObject*, guint, GdkModifierType);

  CHROMEGTK_CALLBACK_1(InfoBubbleGtk, gboolean, OnExpose, GdkEventExpose*);
  CHROMEGTK_CALLBACK_1(InfoBubbleGtk, void, OnSizeAllocate, GtkAllocation*);
  CHROMEGTK_CALLBACK_1(InfoBubbleGtk, gboolean, OnButtonPress, GdkEventButton*);
  CHROMEGTK_CALLBACK_0(InfoBubbleGtk, gboolean, OnDestroy);
  CHROMEGTK_CALLBACK_0(InfoBubbleGtk, void, OnHide);
  CHROMEGTK_CALLBACK_1(InfoBubbleGtk, gboolean, OnToplevelConfigure,
                       GdkEventConfigure*);
  CHROMEGTK_CALLBACK_1(InfoBubbleGtk, gboolean, OnToplevelUnmap, GdkEvent*);
  CHROMEGTK_CALLBACK_1(InfoBubbleGtk, void, OnAnchorAllocate, GtkAllocation*);

  
  InfoBubbleGtkDelegate* delegate_;

  
  
  GtkWidget* window_;

  
  GtkThemeService* theme_service_;

  
  GtkAccelGroup* accel_group_;

  
  
  
  
  GtkWindow* toplevel_window_;

  
  GtkWidget* anchor_widget_;

  
  
  gfx::Rect rect_;

  
  
  GdkRegion* mask_region_;

  
  
  ArrowLocationGtk preferred_arrow_location_;
  ArrowLocationGtk current_arrow_location_;

  
  
  
  bool match_system_theme_;

  
  
  
  bool grab_input_;

  bool closed_by_escape_;

  NotificationRegistrar registrar_;

  ui::GtkSignalRegistrar signals_;

  DISALLOW_COPY_AND_ASSIGN(InfoBubbleGtk);
};

#endif  
