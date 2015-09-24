// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_HOVER_CONTROLLER_GTK_H_
#define CHROME_BROWSER_UI_GTK_HOVER_CONTROLLER_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/base/animation/throb_animation.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"

class HoverControllerGtk : public ui::AnimationDelegate {
 public:
  virtual ~HoverControllerGtk();

  GtkWidget* button() { return button_; }

  
  
  
  void StartThrobbing(int cycles);

  
  
  static HoverControllerGtk* GetHoverControllerGtk(GtkWidget* button);

  
  static GtkWidget* CreateChromeButton();

  
  void Destroy();

 private:
  explicit HoverControllerGtk(GtkWidget* button);

  
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationEnded(const ui::Animation* animation);
  virtual void AnimationCanceled(const ui::Animation* animation);

  CHROMEGTK_CALLBACK_1(HoverControllerGtk, gboolean, OnEnter,
                       GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(HoverControllerGtk, gboolean, OnLeave,
                       GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(HoverControllerGtk, void, OnHierarchyChanged,
                       GtkWidget*);
  CHROMEGTK_CALLBACK_0(HoverControllerGtk, void, OnDestroy);

  ui::ThrobAnimation throb_animation_;
  ui::SlideAnimation hover_animation_;
  GtkWidget* button_;

  ui::GtkSignalRegistrar signals_;

  DISALLOW_COPY_AND_ASSIGN(HoverControllerGtk);
};

#endif  
