// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_HOVER_CONTROLLER_GTK_H_
#define CHROME_BROWSER_UI_GTK_HOVER_CONTROLLER_GTK_H_

#include <gtk/gtk.h>

#include "base/compiler_specific.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/gfx/animation/throb_animation.h"

class HoverControllerGtk : public gfx::AnimationDelegate {
 public:
  virtual ~HoverControllerGtk();

  GtkWidget* button() { return button_; }

  
  
  
  void StartThrobbing(int cycles);

  
  
  static HoverControllerGtk* GetHoverControllerGtk(GtkWidget* button);

  
  static GtkWidget* CreateChromeButton();

  
  void Destroy();

 private:
  explicit HoverControllerGtk(GtkWidget* button);

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;

  CHROMEGTK_CALLBACK_1(HoverControllerGtk, gboolean, OnEnter,
                       GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(HoverControllerGtk, gboolean, OnLeave,
                       GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(HoverControllerGtk, void, OnHierarchyChanged,
                       GtkWidget*);
  CHROMEGTK_CALLBACK_0(HoverControllerGtk, void, OnDestroy);

  gfx::ThrobAnimation throb_animation_;
  gfx::SlideAnimation hover_animation_;
  GtkWidget* button_;

  ui::GtkSignalRegistrar signals_;

  DISALLOW_COPY_AND_ASSIGN(HoverControllerGtk);
};

#endif  
