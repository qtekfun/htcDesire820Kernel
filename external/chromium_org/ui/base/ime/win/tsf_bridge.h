// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_WIN_TSF_BRIDGE_H_
#define UI_BASE_IME_WIN_TSF_BRIDGE_H_

#include <Windows.h>
#include <msctf.h>

#include "base/basictypes.h"
#include "base/win/scoped_comptr.h"
#include "ui/base/ui_export.h"

namespace ui {
class TextInputClient;

class UI_EXPORT TSFBridge {
 public:
  virtual ~TSFBridge();

  
  
  static TSFBridge* GetInstance();

  
  
  static bool Initialize();

  
  
  
  static TSFBridge* ReplaceForTesting(TSFBridge* bridge);

  
  static void Shutdown();

  
  
  
  virtual void OnTextInputTypeChanged(const TextInputClient* client) = 0;

  
  virtual void OnTextLayoutChanged() = 0;

  
  
  
  
  virtual bool CancelComposition() = 0;

  
  
  
  
  virtual bool ConfirmComposition() = 0;

  
  
  virtual void SetFocusedClient(HWND focused_window,
                                TextInputClient* client) = 0;

  
  
  virtual void RemoveFocusedClient(TextInputClient* client) = 0;

  
  virtual base::win::ScopedComPtr<ITfThreadMgr> GetThreadManager() = 0;

  
  virtual TextInputClient* GetFocusedTextInputClient() const = 0;

 protected:
  
  TSFBridge();

 private:
  
  static void Finalize(void* data);

  DISALLOW_COPY_AND_ASSIGN(TSFBridge);
};

}  

#endif  
