// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_NATIVE_NATIVE_VIEW_HOST_WRAPPER_H_
#define UI_VIEWS_CONTROLS_NATIVE_NATIVE_VIEW_HOST_WRAPPER_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace views {

class NativeViewHost;

class VIEWS_EXPORT NativeViewHostWrapper {
 public:
  virtual ~NativeViewHostWrapper() {}

  
  
  
  virtual void NativeViewWillAttach() = 0;

  
  
  
  
  virtual void NativeViewDetaching(bool destroyed) = 0;

  
  
  virtual void AddedToWidget() = 0;

  
  
  virtual void RemovedFromWidget() = 0;

  
  virtual void InstallClip(int x, int y, int w, int h) = 0;

  
  virtual bool HasInstalledClip() = 0;

  
  virtual void UninstallClip() = 0;

  
  
  virtual void ShowWidget(int x, int y, int w, int h) = 0;

  
  
  virtual void HideWidget() = 0;

  
  virtual void SetFocus() = 0;

  
  
  virtual gfx::NativeViewAccessible GetNativeViewAccessible() = 0;

  
  
  static NativeViewHostWrapper* CreateWrapper(NativeViewHost* host);
};

}  

#endif  
