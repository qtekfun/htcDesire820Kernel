// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_METRO_DRIVER_IME_INPUT_SOURCE_H_
#define WIN8_METRO_DRIVER_IME_INPUT_SOURCE_H_

#include <Windows.h>

#include "base/memory/scoped_ptr.h"

namespace metro_driver {

class InputSourceObserver;

class InputSource {
 public:
  virtual ~InputSource() {}
  
  static scoped_ptr<InputSource> Create();

  
  
  virtual bool GetActiveSource(LANGID* langid, bool* is_ime) = 0;

  
  
  virtual void AddObserver(InputSourceObserver* observer) = 0;
  virtual void RemoveObserver(InputSourceObserver* observer) = 0;
};

}  

#endif  
