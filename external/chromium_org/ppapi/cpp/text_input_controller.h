// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_TEXT_INPUT_CONTROLLER_H_
#define PPAPI_CPP_TEXT_INPUT_CONTROLLER_H_

#include <string>

#include "ppapi/c/ppb_text_input_controller.h"
#include "ppapi/cpp/instance_handle.h"
#include "ppapi/cpp/var.h"


namespace pp {

class Rect;
class Instance;

class TextInputController {
 public:
  
  
  
  
  explicit TextInputController(const InstanceHandle& instance);

  
  ~TextInputController();

  
  
  
  
  void SetTextInputType(PP_TextInput_Type type);

  
  
  
  
  void UpdateCaretPosition(const Rect& caret);

  
  
  void CancelCompositionText();

  
  
  
  
  
  
  
  
  
  
  
  
  void UpdateSurroundingText(const Var& text,
                             uint32_t caret,
                             uint32_t anchor);

 private:
  InstanceHandle instance_;
};

}  

#endif  
