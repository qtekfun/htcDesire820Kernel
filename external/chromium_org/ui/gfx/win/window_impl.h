// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_WIN_WINDOW_IMPL_H_
#define UI_GFX_WIN_WINDOW_IMPL_H_

#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>
#include <atlcrack.h>

#include <string>

#include "base/logging.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

namespace gfx {

class MessageMapInterface {
 public:
  
  virtual BOOL ProcessWindowMessage(HWND window,
                                    UINT message,
                                    WPARAM w_param,
                                    LPARAM l_param,
                                    LRESULT& result,
                                    DWORD msg_map_id = 0) = 0;
};

class GFX_EXPORT WindowImpl : public MessageMapInterface {
 public:
  WindowImpl();
  virtual ~WindowImpl();

  
  void Init(HWND parent, const gfx::Rect& bounds);

  
  virtual HICON GetDefaultWindowIcon() const;

  
  HWND hwnd() const { return hwnd_; }

  
  
  void set_window_style(DWORD style) { window_style_ = style; }
  DWORD window_style() const { return window_style_; }

  
  void set_window_ex_style(DWORD style) { window_ex_style_ = style; }
  DWORD window_ex_style() const { return window_ex_style_; }

  
  void set_initial_class_style(UINT class_style) {
    
    DCHECK_EQ((class_style & CS_GLOBALCLASS), 0u);
    class_style_ = class_style;
  }
  UINT initial_class_style() const { return class_style_; }

 protected:
  
  virtual LRESULT OnWndProc(UINT message, WPARAM w_param, LPARAM l_param);

  
  
  
  
  void ClearUserData();

 private:
  friend class ClassRegistrar;

  
  static LRESULT CALLBACK WndProc(HWND window,
                                  UINT message,
                                  WPARAM w_param,
                                  LPARAM l_param);

  
  
  ATOM GetWindowClassAtom();

  
  static const wchar_t* const kBaseClassName;

  
  DWORD window_style_;

  
  DWORD window_ex_style_;

  
  UINT class_style_;

  
  HWND hwnd_;

  
  
  bool got_create_;
  bool got_valid_hwnd_;
  bool* destroyed_;

  DISALLOW_COPY_AND_ASSIGN(WindowImpl);
};

}  

#endif  
