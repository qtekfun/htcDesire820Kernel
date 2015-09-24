// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_IME_IBUS_KEYMAP_H_
#define CHROMEOS_IME_IBUS_KEYMAP_H_

#include <string>
#include "base/basictypes.h"
#include "chromeos/chromeos_export.h"

namespace chromeos {
namespace input_method {

CHROMEOS_EXPORT std::string GetIBusKey(int keyval);

CHROMEOS_EXPORT std::string GetIBusKeyCode(uint16 keycode);

}  
}  

#endif  
