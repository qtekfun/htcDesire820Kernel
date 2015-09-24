// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEB_MODAL_MODAL_DIALOG_HOST_H_
#define COMPONENTS_WEB_MODAL_MODAL_DIALOG_HOST_H_

#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Point;
class Size;
}  

namespace web_modal {

class ModalDialogHostObserver {
 public:
  virtual ~ModalDialogHostObserver();

  virtual void OnPositionRequiresUpdate() = 0;
  virtual void OnHostDestroying() = 0;
};

class ModalDialogHost {
 public:
  virtual ~ModalDialogHost();

  
  virtual gfx::NativeView GetHostView() const = 0;
  
  virtual gfx::Point GetDialogPosition(const gfx::Size& size) = 0;

  
  virtual void AddObserver(ModalDialogHostObserver* observer) = 0;
  virtual void RemoveObserver(ModalDialogHostObserver* observer) = 0;
};

}  

#endif  
