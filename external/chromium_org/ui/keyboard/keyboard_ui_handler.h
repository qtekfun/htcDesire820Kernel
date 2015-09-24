// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_UI_HANDLER_H_
#define UI_KEYBOARD_KEYBOARD_UI_HANDLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace keyboard {

class KeyboardUIHandler : public content::WebUIMessageHandler {
 public:
  KeyboardUIHandler();
  virtual ~KeyboardUIHandler();

 private:
  
  virtual void RegisterMessages() OVERRIDE;

  
  void HandleInsertTextMessage(const base::ListValue* args);

  
  
  void HandleGetInputContextMessage(const base::ListValue* args);

  
  
  
  void HandleSendKeyEventMessage(const base::ListValue* args);

  
  void HandleHideKeyboard(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(KeyboardUIHandler);
};

}  

#endif  
