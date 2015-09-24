// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_METRO_DRIVER_IME_TEXT_STORE_DELEGATE_H_
#define WIN8_METRO_DRIVER_IME_TEXT_STORE_DELEGATE_H_

#include <vector>

#include <windows.h>

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace metro_viewer {
struct UnderlineInfo;
}

namespace metro_driver {

class TextStoreDelegate {
 public:
  virtual ~TextStoreDelegate() {}

  
  
  virtual void OnCompositionChanged(
      const string16& text,
      int32 selection_start,
      int32 selection_end,
      const std::vector<metro_viewer::UnderlineInfo>& underlines) = 0;

  
  virtual void OnTextCommitted(const string16& text) = 0;

  
  
  virtual RECT GetCaretBounds() = 0;

  
  
  
  
  
  virtual bool GetCompositionCharacterBounds(uint32 index, RECT* rect) = 0;
};

}  

#endif  
