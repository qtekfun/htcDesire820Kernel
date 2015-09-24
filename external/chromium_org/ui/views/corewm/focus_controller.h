// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_FOCUS_CONTROLLER_H_
#define UI_VIEWS_COREWM_FOCUS_CONTROLLER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/scoped_observer.h"
#include "ui/aura/client/activation_client.h"
#include "ui/aura/client/focus_client.h"
#include "ui/aura/window_observer.h"
#include "ui/events/event_handler.h"
#include "ui/views/views_export.h"

namespace views {
namespace corewm {

class FocusRules;

class VIEWS_EXPORT FocusController : public aura::client::ActivationClient,
                                     public aura::client::FocusClient,
                                     public ui::EventHandler,
                                     public aura::WindowObserver {
 public:
  
  explicit FocusController(FocusRules* rules);
  virtual ~FocusController();

 private:
  
  virtual void AddObserver(
      aura::client::ActivationChangeObserver* observer) OVERRIDE;
  virtual void RemoveObserver(
      aura::client::ActivationChangeObserver* observer) OVERRIDE;
  virtual void ActivateWindow(aura::Window* window) OVERRIDE;
  virtual void DeactivateWindow(aura::Window* window) OVERRIDE;
  virtual aura::Window* GetActiveWindow() OVERRIDE;
  virtual aura::Window* GetActivatableWindow(aura::Window* window) OVERRIDE;
  virtual aura::Window* GetToplevelWindow(aura::Window* window) OVERRIDE;
  virtual bool OnWillFocusWindow(aura::Window* window,
    const ui::Event* event) OVERRIDE;
  virtual bool CanActivateWindow(aura::Window* window) const OVERRIDE;

  
  virtual void AddObserver(
      aura::client::FocusChangeObserver* observer) OVERRIDE;
  virtual void RemoveObserver(
      aura::client::FocusChangeObserver* observer) OVERRIDE;
  virtual void FocusWindow(aura::Window* window) OVERRIDE;
  virtual void ResetFocusWithinActiveWindow(aura::Window* window) OVERRIDE;
  virtual aura::Window* GetFocusedWindow() OVERRIDE;

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void OnWindowVisibilityChanged(aura::Window* window,
                                         bool visible) OVERRIDE;
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;
  virtual void OnWindowHierarchyChanging(
      const HierarchyChangeParams& params) OVERRIDE;
  virtual void OnWindowHierarchyChanged(
      const HierarchyChangeParams& params) OVERRIDE;

  
  
  void SetFocusedWindow(aura::Window* window);

  
  
  
  
  
  
  void SetActiveWindow(aura::Window* requested_window,
                       aura::Window* activatable_window);

  
  
  
  void WindowLostFocusFromDispositionChange(aura::Window* window,
                                            aura::Window* next);

  
  
  
  void WindowFocusedFromInputEvent(aura::Window* window);

  aura::Window* active_window_;
  aura::Window* focused_window_;

  bool updating_focus_;
  bool updating_activation_;

  scoped_ptr<FocusRules> rules_;

  ObserverList<aura::client::ActivationChangeObserver> activation_observers_;
  ObserverList<aura::client::FocusChangeObserver> focus_observers_;

  ScopedObserver<aura::Window, aura::WindowObserver> observer_manager_;

  DISALLOW_COPY_AND_ASSIGN(FocusController);
};

}  
}  

#endif  
