// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_NATIVE_CONTROL_H_
#define UI_VIEWS_CONTROLS_NATIVE_CONTROL_H_

#include <windows.h>

#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/views/view.h"

namespace views {

class NativeViewHost;
class NativeControlContainer;

class VIEWS_EXPORT NativeControl : public View {
 public:
   enum Alignment {
     LEADING = 0,
     CENTER,
     TRAILING };

  NativeControl();
  virtual ~NativeControl();

  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual void Layout();

  
  virtual void SetVisible(bool f);
  virtual void OnEnabledChanged();

  
  virtual void OnPaint(gfx::Canvas* canvas);

 protected:
  friend class NativeControlContainer;

  
  virtual HWND CreateNativeControl(HWND parent_container) = 0;

  
  virtual LRESULT OnNotify(int w_param, LPNMHDR l_param) = 0;

  
  virtual LRESULT OnCommand(UINT code, int id, HWND source) { return 0; }

  
  virtual void OnContextMenu(const POINT& location);

  
  virtual void OnFocus();

  
  virtual void OnDestroy() { }

  
  virtual HWND GetNativeControlHWND();

  
  
  
  void NativeControlDestroyed();

  
  virtual void VisibilityChanged(View* starting_from, bool is_visible);

  
  
  void SetFixedWidth(int width, Alignment alignment);
  void SetFixedHeight(int height, Alignment alignment);

  
  
  virtual bool OnKeyDown(ui::KeyboardCode virtual_key_code) { return false; }

  
  
  
  
  
  
  DWORD GetAdditionalExStyle() const;

  
  
  
  
  DWORD GetAdditionalRTLStyle() const;

  
  
  
  NativeViewHost* hwnd_view_;

  
  int fixed_width_;
  Alignment horizontal_alignment_;
  int fixed_height_;
  Alignment vertical_alignment_;

 private:

  void ValidateNativeControl();

  static LRESULT CALLBACK NativeControlWndProc(HWND window, UINT message,
                                               WPARAM w_param, LPARAM l_param);

  NativeControlContainer* container_;

  DISALLOW_COPY_AND_ASSIGN(NativeControl);
};

}  

#endif  
