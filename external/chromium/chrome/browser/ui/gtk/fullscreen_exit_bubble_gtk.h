// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_FULLSCREEN_EXIT_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_FULLSCREEN_EXIT_BUBBLE_GTK_H_
#pragma once

#include "base/timer.h"
#include "chrome/browser/ui/gtk/slide_animator_gtk.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"

typedef struct _GtkFloatingContainer GtkFloatingContainer;
typedef struct _GtkWidget GtkWidget;

class FullscreenExitBubbleGtk {
 public:
  
  explicit FullscreenExitBubbleGtk(GtkFloatingContainer* container);
  virtual ~FullscreenExitBubbleGtk();

  void InitWidgets();

 private:
  GtkWidget* widget() const {
    return slide_widget_->widget();
  }

  
  void Hide();

  CHROMEGTK_CALLBACK_1(FullscreenExitBubbleGtk, void, OnSetFloatingPosition,
                       GtkAllocation*);
  CHROMEGTK_CALLBACK_0(FullscreenExitBubbleGtk, void, OnLinkClicked);

  
  GtkFloatingContainer* container_;

  
  scoped_ptr<SlideAnimatorGtk> slide_widget_;

  
  base::OneShotTimer<FullscreenExitBubbleGtk> initial_delay_;

  ui::GtkSignalRegistrar signals_;
};

#endif  
