// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_SCOPED_OLE_INITIALIZER_H_
#define UI_BASE_WIN_SCOPED_OLE_INITIALIZER_H_

#include <ole2.h>

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT ScopedOleInitializer {
 public:
  ScopedOleInitializer();
  ~ScopedOleInitializer();

 private:
#ifndef NDEBUG
  
  
  
  
  DWORD thread_id_;
#endif
  HRESULT hr_;

  DISALLOW_COPY_AND_ASSIGN(ScopedOleInitializer);
};

}  

#endif  
