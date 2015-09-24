// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_NATIVE_CONTROL_WIN_H_
#define UI_VIEWS_CONTROLS_NATIVE_CONTROL_WIN_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/widget/child_window_message_processor.h"

namespace ui {
class ViewProp;
}

namespace views {

class NativeControlWin : public ChildWindowMessageProcessor,
                         public NativeViewHost {
 public:
  NativeControlWin();
  virtual ~NativeControlWin();

  
  
  
  virtual bool OnKeyDown(int vkey) { return false; }

  
  virtual bool ProcessMessage(UINT message,
                              WPARAM w_param,
                              LPARAM l_param,
                              LRESULT* result) OVERRIDE;

  
  virtual void OnEnabledChanged() OVERRIDE;

 protected:
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual void VisibilityChanged(View* starting_from, bool is_visible) OVERRIDE;
  virtual void OnFocus() OVERRIDE;

  
  
  virtual void ShowContextMenu(const gfx::Point& location);

  
  
  
  virtual void CreateNativeControl() = 0;

  
  
  
  virtual void NativeControlCreated(HWND native_control);

  
  
  
  
  
  
  DWORD GetAdditionalExStyle() const;

  
  
  
  
  DWORD GetAdditionalRTLStyle() const;

 private:
  typedef ScopedVector<ui::ViewProp> ViewProps;

  
  
  
  LRESULT GetControlColor(UINT message, HDC dc, HWND sender);

  
  static LRESULT CALLBACK NativeControlWndProc(HWND window,
                                               UINT message,
                                               WPARAM w_param,
                                               LPARAM l_param);

  
  WNDPROC original_wndproc_;

  ViewProps props_;

  DISALLOW_COPY_AND_ASSIGN(NativeControlWin);
};

}  

#endif  
