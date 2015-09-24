// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_DRAGDROP_DROP_TARGET_WIN_H_
#define UI_BASE_DRAGDROP_DROP_TARGET_WIN_H_

#include <objidl.h>

#include "base/memory/ref_counted.h"
#include "ui/base/ui_export.h"

struct IDropTargetHelper;

namespace ui {

class UI_EXPORT DropTargetWin : public IDropTarget {
 public:
  
  explicit DropTargetWin(HWND hwnd);
  virtual ~DropTargetWin();

  
  HRESULT __stdcall DragEnter(IDataObject* data_object,
                              DWORD key_state,
                              POINTL cursor_position,
                              DWORD* effect);
  HRESULT __stdcall DragOver(DWORD key_state,
                             POINTL cursor_position,
                             DWORD* effect);
  HRESULT __stdcall DragLeave();
  HRESULT __stdcall Drop(IDataObject* data_object,
                         DWORD key_state,
                         POINTL cursor_position,
                         DWORD* effect);

  
  HRESULT __stdcall QueryInterface(const IID& iid, void** object);
  ULONG __stdcall AddRef();
  ULONG __stdcall Release();

 protected:
  
  HWND GetHWND() { return hwnd_; }

  
  
  
  
  virtual DWORD OnDragEnter(IDataObject* data_object,
                            DWORD key_state,
                            POINT cursor_position,
                            DWORD effect);

  
  
  
  
  virtual DWORD OnDragOver(IDataObject* data_object,
                           DWORD key_state,
                           POINT cursor_position,
                           DWORD effect);

  
  
  virtual void OnDragLeave(IDataObject* data_object);

  
  
  virtual DWORD OnDrop(IDataObject* data_object,
                       DWORD key_state,
                       POINT cursor_position,
                       DWORD effect);

 private:
  
  
  static IDropTargetHelper* DropHelper();

  
  scoped_refptr<IDataObject> current_data_object_;

  
  
  
  
  
  
  
  
  static IDropTargetHelper* cached_drop_target_helper_;

  
  
  HWND hwnd_;

  LONG ref_count_;

  DISALLOW_COPY_AND_ASSIGN(DropTargetWin);
};

}  

#endif  
