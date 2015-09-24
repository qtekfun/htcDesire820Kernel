// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BUBBLE_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_BUBBLE_BUBBLE_GTK_H_

#include <gtk/gtk.h>

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

class BubbleGtk;
class GtkThemeService;

namespace gfx {
class Rect;
}

class BubbleDelegateGtk {
 public:
  
  
  virtual void BubbleClosing(BubbleGtk* bubble, bool closed_by_escape) = 0;

  
  

 protected:
  virtual ~BubbleDelegateGtk() {}
};

class BubbleGtk : public content::NotificationObserver {
 public:
  
  enum FrameStyle {
    ANCHOR_TOP_LEFT,
    ANCHOR_TOP_MIDDLE,
    ANCHOR_TOP_RIGHT,
    ANCHOR_BOTTOM_LEFT,
    ANCHOR_BOTTOM_MIDDLE,
    ANCHOR_BOTTOM_RIGHT,
    FLOAT_BELOW_RECT,  
    CENTER_OVER_RECT,  
    FIXED_TOP_LEFT,  
    FIXED_TOP_RIGHT,  
  };

  enum BubbleAttribute {
    NONE = 0,
    MATCH_SYSTEM_THEME = 1 << 0,  
    POPUP_WINDOW = 1 << 1,  
    GRAB_INPUT = 1 << 2,  
    NO_ACCELERATORS = 1 << 3, 
                              
  };

  
  
  
  
  
  
  
  
  
  static BubbleGtk* Show(GtkWidget* anchor_widget,
                         const gfx::Rect* rect,
                         GtkWidget* content,
                         FrameStyle frame_style,
                         int attribute_flags,
                         GtkThemeService* provider,
                         BubbleDelegateGtk* delegate);

  
  
  void Close();

  
  void SetPositionRelativeToAnchor(const gfx::Rect* rect);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  
  void StopGrabbingInput();

  GtkWindow* GetNativeWindow();

  GtkWidget* anchor_widget() { return anchor_widget_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(BubbleGtkTest, ArrowLocation);
  FRIEND_TEST_ALL_PREFIXES(BubbleGtkTest, NoArrow);

  enum FrameType {
    FRAME_MASK,
    FRAME_STROKE,
  };

  BubbleGtk(GtkThemeService* provider,
            FrameStyle frame_style,
            int attribute_flags);
  virtual ~BubbleGtk();

  
  void Init(GtkWidget* anchor_widget,
            const gfx::Rect* rect,
            GtkWidget* content,
            int attribute_flags);

  
  
  static std::vector<GdkPoint> MakeFramePolygonPoints(
      FrameStyle frame_style,
      int width,
      int height,
      FrameType type);

  
  
  
  
  
  static FrameStyle GetAllowedFrameStyle(FrameStyle preferred_location,
                                         int arrow_x,
                                         int arrow_y,
                                         int width,
                                         int height);

  
  
  
  bool UpdateFrameStyle(bool force_move_and_reshape);

  
  void UpdateWindowShape();

  
  
  
  void MoveWindow();

  
  void StackWindow();

  
  void set_delegate(BubbleDelegateGtk* delegate) { delegate_ = delegate; }

  
  
  void GrabPointerAndKeyboard();

  
  
  
  void UngrabPointerAndKeyboard();

  CHROMEG_CALLBACK_3(BubbleGtk, gboolean, OnGtkAccelerator, GtkAccelGroup*,
                     GObject*, guint, GdkModifierType);

  CHROMEGTK_CALLBACK_1(BubbleGtk, gboolean, OnExpose, GdkEventExpose*);
  CHROMEGTK_CALLBACK_1(BubbleGtk, void, OnSizeAllocate, GtkAllocation*);
  CHROMEGTK_CALLBACK_1(BubbleGtk, gboolean, OnButtonPress, GdkEventButton*);
  CHROMEGTK_CALLBACK_0(BubbleGtk, gboolean, OnDestroy);
  CHROMEGTK_CALLBACK_0(BubbleGtk, void, OnHide);
  CHROMEGTK_CALLBACK_1(BubbleGtk, gboolean, OnGrabBroken, GdkEventGrabBroken*);
  CHROMEGTK_CALLBACK_0(BubbleGtk, void, OnForeshadowWidgetHide);
  CHROMEGTK_CALLBACK_1(BubbleGtk, gboolean, OnToplevelConfigure,
                       GdkEventConfigure*);
  CHROMEGTK_CALLBACK_1(BubbleGtk, gboolean, OnToplevelUnmap, GdkEvent*);
  CHROMEGTK_CALLBACK_1(BubbleGtk, void, OnAnchorAllocate, GtkAllocation*);
  CHROMEGTK_CALLBACK_0(BubbleGtk, void, OnAnchorDestroy);

  
  BubbleDelegateGtk* delegate_;

  
  
  GtkWidget* window_;

  
  GtkThemeService* theme_service_;

  
  GtkAccelGroup* accel_group_;

  
  
  
  
  GtkWidget* toplevel_window_;

  
  GtkWidget* anchor_widget_;

  
  
  gfx::Rect rect_;

  
  
  GdkRegion* mask_region_;

  
  
  
  FrameStyle requested_frame_style_;

  
  FrameStyle actual_frame_style_;

  
  
  
  bool match_system_theme_;

  
  
  
  bool grab_input_;

  bool closed_by_escape_;

  content::NotificationRegistrar registrar_;

  ui::GtkSignalRegistrar signals_;

  base::WeakPtrFactory<BubbleGtk> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BubbleGtk);
};

#endif  
