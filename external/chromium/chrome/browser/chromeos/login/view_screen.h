// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_VIEW_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_VIEW_SCREEN_H_
#pragma once

#include "base/message_loop.h"
#include "chrome/browser/chromeos/login/helper.h"
#include "chrome/browser/chromeos/login/wizard_screen.h"
#include "ui/gfx/size.h"

template <class V>
class ViewScreen : public WizardScreen {
 public:
  
  explicit ViewScreen(WizardScreenDelegate* delegate);

  
  ViewScreen(WizardScreenDelegate* delegate, int width, int height);
  virtual ~ViewScreen();

  
  virtual void Show();
  virtual void Hide();
  virtual gfx::Size GetScreenSize() const { return size_; }

  V* view() { return view_; }

 protected:
  
  virtual void CreateView();
  
  virtual V* AllocateView() = 0;

  
  virtual void Refresh() {}

 private:
  
  friend class AutomationProvider;

  V* view_;

  
  gfx::Size size_;

  DISALLOW_COPY_AND_ASSIGN(ViewScreen);
};

template <class V>
class DefaultViewScreen : public ViewScreen<V> {
 public:
  explicit DefaultViewScreen(WizardScreenDelegate* delegate)
        : ViewScreen<V>(delegate) {}
  DefaultViewScreen(WizardScreenDelegate* delegate, int width, int height)
      : ViewScreen<V>(delegate, width, height) {}
  V* AllocateView() {
    return new V(ViewScreen<V>::delegate()->GetObserver(this));
  }
};

template <class V>
ViewScreen<V>::ViewScreen(WizardScreenDelegate* delegate)
    : WizardScreen(delegate),
      view_(NULL),
      size_(chromeos::login::kWizardScreenWidth,
            chromeos::login::kWizardScreenHeight) {
}

template <class V>
ViewScreen<V>::ViewScreen(WizardScreenDelegate* delegate, int width, int height)
    : WizardScreen(delegate),
      view_(NULL),
      size_(width, height) {
}

template <class V>
ViewScreen<V>::~ViewScreen() {
  
  
  if (view_) {
    delete view_;
    view_ = NULL;
  }
}

template <class V>
void ViewScreen<V>::Show() {
  if (!view_) {
    CreateView();
  }
  view_->SetVisible(true);
  
  
  
  Refresh();
}

template <class V>
void ViewScreen<V>::Hide() {
  if (view_) {
    delegate()->GetWizardView()->RemoveChildView(view_);
    
    
    MessageLoop::current()->DeleteSoon(FROM_HERE, view_);
    view_ = NULL;
  }
}

template <class V>
void ViewScreen<V>::CreateView() {
  view_ = AllocateView();
  view_->set_parent_owned(false);  
  delegate()->GetWizardView()->AddChildView(view_);
  view_->Init();
  view_->SetVisible(false);
}

#endif  
