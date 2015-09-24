// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_DRAGDROP_DRAG_SOURCE_WIN_H_
#define UI_BASE_DRAGDROP_DRAG_SOURCE_WIN_H_

#include <objidl.h>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT DragSourceWin
    : public IDropSource,
      public base::RefCountedThreadSafe<DragSourceWin> {
 public:
  DragSourceWin();
  virtual ~DragSourceWin() {}

  
  
  
  void CancelDrag() {
    cancel_drag_ = true;
  }

  
  HRESULT __stdcall QueryContinueDrag(BOOL escape_pressed, DWORD key_state);
  HRESULT __stdcall GiveFeedback(DWORD effect);

  
  HRESULT __stdcall QueryInterface(const IID& iid, void** object);
  ULONG __stdcall AddRef();
  ULONG __stdcall Release();

 protected:
  virtual void OnDragSourceCancel() {}
  virtual void OnDragSourceDrop() {}
  virtual void OnDragSourceMove() {}

 private:
  
  bool cancel_drag_;

  DISALLOW_COPY_AND_ASSIGN(DragSourceWin);
};

}  

#endif  
