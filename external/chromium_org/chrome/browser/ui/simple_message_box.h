// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SIMPLE_MESSAGE_BOX_H_
#define CHROME_BROWSER_UI_SIMPLE_MESSAGE_BOX_H_

#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"

namespace chrome {

enum MessageBoxResult {
  MESSAGE_BOX_RESULT_NO = 0,  
  MESSAGE_BOX_RESULT_YES = 1, 
};

enum MessageBoxType {
  MESSAGE_BOX_TYPE_INFORMATION,  
  MESSAGE_BOX_TYPE_WARNING,      
  MESSAGE_BOX_TYPE_QUESTION,     
  MESSAGE_BOX_TYPE_OK_CANCEL,    
                                 
};

MessageBoxResult ShowMessageBox(gfx::NativeWindow parent,
                                const base::string16& title,
                                const base::string16& message,
                                MessageBoxType type);

MessageBoxResult ShowMessageBoxWithButtonText(gfx::NativeWindow parent,
                                              const base::string16& title,
                                              const base::string16& message,
                                              const base::string16& yes_text,
                                              const base::string16& no_text);

}  

#endif  
