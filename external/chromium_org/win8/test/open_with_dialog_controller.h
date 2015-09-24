// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_TEST_OPEN_WITH_DIALOG_CONTROLLER_H_
#define WIN8_TEST_OPEN_WITH_DIALOG_CONTROLLER_H_

#include <windows.h>

#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"

namespace win8 {

class OpenWithDialogController {
 public:
  
  
  
  
  typedef base::Callback<void(HRESULT,
                              std::vector<string16>)> SetDefaultCallback;

  OpenWithDialogController();
  ~OpenWithDialogController();

  
  
  
  
  
  
  
  void Begin(HWND parent_window,
             const string16& url_protocol,
             const string16& program,
             const SetDefaultCallback& callback);

  
  
  
  
  HRESULT RunSynchronously(HWND parent_window,
                           const string16& url_protocol,
                           const string16& program,
                           std::vector<string16>* choices);

 private:
  class Context;

  base::WeakPtr<Context> context_;

  DISALLOW_COPY_AND_ASSIGN(OpenWithDialogController);
};

}  

#endif 
