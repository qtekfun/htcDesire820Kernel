// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_IME_XKEYBOARD_H_
#define CHROMEOS_IME_XKEYBOARD_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chromeos/chromeos_export.h"

namespace chromeos {
namespace input_method {

struct AutoRepeatRate {
  AutoRepeatRate() : initial_delay_in_ms(0), repeat_interval_in_ms(0) {}
  unsigned int initial_delay_in_ms;
  unsigned int repeat_interval_in_ms;
};

enum ModifierLockStatus {
  kDisableLock = 0,
  kEnableLock,
  kDontChange,
};

enum ModifierKey {
  kSearchKey = 0,  
  kControlKey,  
  kAltKey,  
  kVoidKey,
  kCapsLockKey,
  kEscapeKey,
  
  
  kNumModifierKeys,
};

class InputMethodUtil;

class CHROMEOS_EXPORT XKeyboard {
 public:
  virtual ~XKeyboard() {}

  
  
  virtual bool SetCurrentKeyboardLayoutByName(
      const std::string& layout_name) = 0;

  
  
  
  virtual bool ReapplyCurrentKeyboardLayout() = 0;

  
  
  
  
  
  
  
  
  
  virtual void ReapplyCurrentModifierLockStatus() = 0;

  
  
  virtual void SetLockedModifiers(ModifierLockStatus new_caps_lock_status,
                                  ModifierLockStatus new_num_lock_status) = 0;

  
  
  virtual void SetNumLockEnabled(bool enable_num_lock) = 0;

  
  
  virtual void SetCapsLockEnabled(bool enable_caps_lock) = 0;

  
  
  virtual bool NumLockIsEnabled() = 0;

  
  
  virtual bool CapsLockIsEnabled() = 0;

  
  
  
  
  
  virtual unsigned int GetNumLockMask() = 0;

  
  
  
  virtual void GetLockedModifiers(bool* out_caps_lock_enabled,
                                  bool* out_num_lock_enabled) = 0;

  
  
  
  static CHROMEOS_EXPORT bool SetAutoRepeatEnabled(bool enabled);

  
  
  
  
  static CHROMEOS_EXPORT bool SetAutoRepeatRate(const AutoRepeatRate& rate);

  
  
  static CHROMEOS_EXPORT bool GetAutoRepeatEnabledForTesting();

  
  
  static CHROMEOS_EXPORT bool GetAutoRepeatRateForTesting(
      AutoRepeatRate* out_rate);

  
  static CHROMEOS_EXPORT bool CheckLayoutNameForTesting(
      const std::string& layout_name);

  
  
  static XKeyboard* Create();
};

}  
}  

#endif  
