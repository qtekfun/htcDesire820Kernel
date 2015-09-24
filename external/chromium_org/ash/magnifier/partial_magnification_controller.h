// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_MAGNIFIER_PARTIAL_MAGNIFICATION_CONTROLLER_H_
#define ASH_MAGNIFIER_PARTIAL_MAGNIFICATION_CONTROLLER_H_

#include "ui/aura/window_observer.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/point.h"
#include "ui/views/widget/widget_observer.h"

namespace ash {

const float kDefaultPartialMagnifiedScale = 1.5f;
const float kNonPartialMagnifiedScale = 1.0f;

class PartialMagnificationController
  : public ui::EventHandler,
    public aura::WindowObserver,
    public views::WidgetObserver {
 public:
  PartialMagnificationController();
  virtual ~PartialMagnificationController();

  
  
  virtual void SetEnabled(bool enabled);

  bool is_enabled() const { return is_enabled_; }

  
  void SetScale(float scale);

  
  float GetScale() const { return scale_; }

 private:
  void OnMouseMove(const gfx::Point& location_in_root);

  
  
  
  
  
  void SwitchTargetRootWindow(aura::Window* new_root_window);

  
  aura::Window* GetCurrentRootWindow();

  
  bool IsPartialMagnified() const;

  
  void CreateMagnifierWindow();

  
  void CloseMagnifierWindow();

  
  void RemoveZoomWidgetObservers();

  
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;

  
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

  
  
  bool is_on_zooming_;

  bool is_enabled_;

  
  float scale_;
  gfx::Point origin_;

  views::Widget* zoom_widget_;

  DISALLOW_COPY_AND_ASSIGN(PartialMagnificationController);
};

}  

#endif  
