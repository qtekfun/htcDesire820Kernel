// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_CONSTRAINED_WINDOW_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_CONSTRAINED_WINDOW_VIEWS_H_
#pragma once

#include "base/compiler_specific.h"
#include "content/browser/tab_contents/constrained_window.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

class ConstrainedTabContentsWindowDelegate;
class ConstrainedWindowAnimation;
class ConstrainedWindowFrameView;
namespace views {
class NativeWindow;
class NonClientFrameView;
class Window;
class WindowDelegate;
}

class NativeConstrainedWindowDelegate {
 public:
  virtual ~NativeConstrainedWindowDelegate() {}

  
  
  virtual void OnNativeConstrainedWindowDestroyed() = 0;

  
  virtual void OnNativeConstrainedWindowMouseActivate() = 0;

  
  
  virtual views::NonClientFrameView* CreateFrameViewForWindow() = 0;
};

class NativeConstrainedWindow {
 public:
  virtual ~NativeConstrainedWindow() {}

  
  
  
  static NativeConstrainedWindow* CreateNativeConstrainedWindow(
      NativeConstrainedWindowDelegate* delegate,
      views::WindowDelegate* window_delegate);

  virtual void InitNativeConstrainedWindow(gfx::NativeView parent) = 0;

  virtual views::NativeWindow* AsNativeWindow() = 0;
};

class ConstrainedWindowViews : public ConstrainedWindow,
                               public NativeConstrainedWindowDelegate {
 public:
  ConstrainedWindowViews(TabContents* owner,
                         views::WindowDelegate* window_delegate);
  virtual ~ConstrainedWindowViews();

  
  TabContents* owner() const { return owner_; }

  views::Window* GetWindow();

  
  virtual void ShowConstrainedWindow() OVERRIDE;
  virtual void CloseConstrainedWindow() OVERRIDE;
  virtual void FocusConstrainedWindow() OVERRIDE;

 private:
  
  virtual void OnNativeConstrainedWindowDestroyed() OVERRIDE;
  virtual void OnNativeConstrainedWindowMouseActivate() OVERRIDE;
  virtual views::NonClientFrameView* CreateFrameViewForWindow() OVERRIDE;

  
  TabContents* owner_;

  NativeConstrainedWindow* native_constrained_window_;

  DISALLOW_COPY_AND_ASSIGN(ConstrainedWindowViews);
};

#endif  
