// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_METRO_DRIVER_IME_TEXT_SERVICE_H_
#define WIN8_METRO_DRIVER_IME_TEXT_SERVICE_H_

#include <Windows.h>

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace metro_viewer {
struct CharacterBounds;
}

namespace metro_driver {

class TextServiceDelegate;

class TextService {
 public:
  virtual ~TextService() {}

  
  virtual void CancelComposition() = 0;

  
  
  
  
  
  
  
  virtual void OnDocumentChanged(
      const std::vector<int32>& input_scopes,
      const std::vector<metro_viewer::CharacterBounds>& character_bounds) = 0;

  
  virtual void OnWindowActivated() = 0;
};

scoped_ptr<TextService>
CreateTextService(TextServiceDelegate* delegate, HWND window_handle);

}  

#endif  
