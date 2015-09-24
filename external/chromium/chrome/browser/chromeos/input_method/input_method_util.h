// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_UTIL_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/string16.h"
#include "chrome/browser/chromeos/cros/input_method_library.h"

namespace chromeos {
namespace input_method {

const struct ExtraLanguage {
  const char* language_code;
  const char* input_method_id;
} kExtraLanguages[] = {
  { "en-AU", "xkb:us::eng" },  
  { "id", "xkb:us::eng" }, 
  
  { "fil", "xkb:us::eng" },  
  
  
  { "es-419", "xkb:latam::spa" },
};

// Converts a string sent from IBus IME engines, which is written in English,
std::wstring GetString(
    const std::string& english_string, const std::string& input_method_id);
std::string GetStringUTF8(
    const std::string& english_string, const std::string& input_method_id);
string16 GetStringUTF16(
    const std::string& english_string, const std::string& input_method_id);

bool StringIsSupported(const std::string& english_string,
                       const std::string& input_method_id);

std::string NormalizeLanguageCode(const std::string& language_code);

bool IsKeyboardLayout(const std::string& input_method_id);

std::string GetLanguageCodeFromDescriptor(
    const InputMethodDescriptor& descriptor);

std::string GetKeyboardLayoutName(const std::string& input_method_id);

std::string GetKeyboardOverlayId(const std::string& input_method_name);

std::string GetLanguageCodeFromInputMethodId(
    const std::string& input_method_id);

std::string GetInputMethodDisplayNameFromId(const std::string& input_method_id);

const chromeos::InputMethodDescriptor* GetInputMethodDescriptorFromId(
    const std::string& input_method_id);

string16 GetLanguageDisplayNameFromCode(const std::string& language_code);

string16 GetLanguageNativeDisplayNameFromCode(const std::string& language_code);

void SortLanguageCodesByNames(std::vector<std::string>* language_codes);

enum InputMethodType {
  kKeyboardLayoutsOnly,
  kAllInputMethods,
};

bool GetInputMethodIdsFromLanguageCode(
    const std::string& language_code,
    InputMethodType type,
    std::vector<std::string>* out_input_method_ids);

void GetFirstLoginInputMethodIds(
    const std::string& language_code,
    const InputMethodDescriptor& current_input_method,
    std::vector<std::string>* out_input_method_ids);

void GetLanguageCodesFromInputMethodIds(
    const std::vector<std::string>& input_method_ids,
    std::vector<std::string>* out_language_codes);

void EnableInputMethods(const std::string& language_code, InputMethodType type,
                        const std::string& initial_input_method_id);

std::string GetHardwareInputMethodId();

InputMethodDescriptor GetFallbackInputMethodDescriptor();

void OnLocaleChanged();

bool GetInputMethodIdsFromLanguageCodeInternal(
    const std::multimap<std::string, std::string>& language_code_to_ids,
    const std::string& normalized_language_code,
    InputMethodType type,
    std::vector<std::string>* out_input_method_ids);

void ReloadInternalMaps();

}  
}  

#endif  
