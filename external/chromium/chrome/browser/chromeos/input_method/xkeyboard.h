// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_XKEYBOARD_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_XKEYBOARD_H_
#pragma once

#include <map>
#include <string>
#include <vector>

namespace chromeos {
namespace input_method {

struct AutoRepeatRate {
  AutoRepeatRate() : initial_delay_in_ms(0), repeat_interval_in_ms(0) {}
  unsigned int initial_delay_in_ms;
  unsigned int repeat_interval_in_ms;
};

enum ModifierKey {
  kSearchKey = 0,  
  kLeftControlKey,  
  kLeftAltKey,  
  kVoidKey,
  kCapsLockKey,
  
  
  kNumModifierKeys,
};

struct ModifierKeyPair {
  ModifierKeyPair(ModifierKey in_original, ModifierKey in_replacement)
      : original(in_original), replacement(in_replacement) {}
  bool operator==(const ModifierKeyPair& rhs) const {
    
    return (rhs.original == original) && (rhs.replacement == replacement);
  }
  ModifierKey original;      
  ModifierKey replacement;   
};
typedef std::vector<ModifierKeyPair> ModifierMap;

bool SetCurrentKeyboardLayoutByName(const std::string& layout_name);

bool RemapModifierKeys(const ModifierMap& modifier_map);

bool SetAutoRepeatEnabled(bool enabled);

bool SetAutoRepeatRate(const AutoRepeatRate& rate);


inline std::string ModifierKeyToString(ModifierKey key) {
  switch (key) {
    case kSearchKey:
      return "search";
    case kLeftControlKey:
      return "leftcontrol";
    case kLeftAltKey:
      return "leftalt";
    case kVoidKey:
      return "disabled";
    case kCapsLockKey:
      return "capslock";
    case kNumModifierKeys:
      break;
  }
  return "";
}

std::string CreateFullXkbLayoutName(const std::string& layout_name,
                                    const ModifierMap& modifire_map);

bool CapsLockIsEnabled();

void SetCapsLockEnabled(bool enabled);

bool ContainsModifierKeyAsReplacement(
    const ModifierMap& modifier_map, ModifierKey key);

}  
}  

#endif  
