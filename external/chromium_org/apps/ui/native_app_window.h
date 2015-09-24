// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_UI_NATIVE_APP_WINDOW_H_
#define APPS_UI_NATIVE_APP_WINDOW_H_

#include "apps/shell_window.h"
#include "components/web_modal/web_contents_modal_dialog_host.h"
#include "ui/base/base_window.h"
#include "ui/gfx/insets.h"

namespace apps {

class NativeAppWindow : public ui::BaseWindow,
                        public web_modal::WebContentsModalDialogHost {
 public:
  
  
  virtual void SetFullscreen(int fullscreen_types) = 0;

  
  virtual bool IsFullscreenOrPending() const = 0;

  
  virtual bool IsDetached() const = 0;

  
  virtual void UpdateWindowIcon() = 0;

  
  virtual void UpdateWindowTitle() = 0;

  
  virtual void UpdateDraggableRegions(
      const std::vector<extensions::DraggableRegion>& regions) = 0;

  
  virtual SkRegion* GetDraggableRegion() = 0;

  
  
  virtual void UpdateShape(scoped_ptr<SkRegion> region) = 0;

  
  
  virtual void HandleKeyboardEvent(
      const content::NativeWebKeyboardEvent& event) = 0;

  
  
  virtual bool IsFrameless() const = 0;

  
  
  virtual gfx::Insets GetFrameInsets() const = 0;

  
  
  
  virtual void ShowWithApp() = 0;
  virtual void HideWithApp() = 0;

  
  
  virtual void UpdateWindowMinMaxSize() = 0;

  virtual ~NativeAppWindow() {}
};

}  

#endif  
