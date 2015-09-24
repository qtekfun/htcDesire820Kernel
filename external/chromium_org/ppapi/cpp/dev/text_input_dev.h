// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_TEXT_INPUT_DEV_H_
#define PPAPI_CPP_DEV_TEXT_INPUT_DEV_H_

#include <string>

#include "ppapi/c/dev/ppb_text_input_dev.h"
#include "ppapi/cpp/instance_handle.h"

namespace pp {

class Rect;
class Instance;

class TextInput_Dev {
 public:
  explicit TextInput_Dev(Instance* instance);
  virtual ~TextInput_Dev();

  virtual void RequestSurroundingText(uint32_t desired_number_of_characters);

  void SetTextInputType(PP_TextInput_Type_Dev type);
  void UpdateCaretPosition(const Rect& caret, const Rect& bounding_box);
  void CancelCompositionText();
  void SelectionChanged();
  void UpdateSurroundingText(const std::string& text,
                             uint32_t caret, uint32_t anchor);

 private:
  InstanceHandle instance_;
};

}  

#endif  
