// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_UTIL_H_

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chromeos/ime/input_method_descriptor.h"

namespace chromeos {
namespace input_method {

class InputMethodDelegate;

enum InputMethodType {
  kKeyboardLayoutsOnly,
  kAllInputMethods,
};

class InputMethodUtil {
 public:
  
  
  
  InputMethodUtil(InputMethodDelegate* delegate,
                  scoped_ptr<InputMethodDescriptors> supported_input_methods);
  ~InputMethodUtil();

  // Converts a string sent from IBus IME engines, which is written in English,
  
  
  
  base::string16 TranslateString(const std::string& english_string) const;

  
  
  
  std::string GetLanguageCodeFromInputMethodId(
      const std::string& input_method_id) const;

  
  
  
  std::string GetInputMethodDisplayNameFromId(
      const std::string& input_method_id) const;

  base::string16 GetInputMethodShortName(
      const InputMethodDescriptor& input_method) const;
  base::string16 GetInputMethodMediumName(
      const InputMethodDescriptor& input_method) const;
  base::string16 GetInputMethodLongName(
      const InputMethodDescriptor& input_method) const;

  
  
  
  
  const InputMethodDescriptor* GetInputMethodDescriptorFromId(
      const std::string& input_method_id) const;

  
  
  
  
  
  
  
  bool GetInputMethodIdsFromLanguageCode(
      const std::string& language_code,
      InputMethodType type,
      std::vector<std::string>* out_input_method_ids) const;

  
  
  
  void GetFirstLoginInputMethodIds(
      const std::string& language_code,
      const InputMethodDescriptor& current_input_method,
      std::vector<std::string>* out_input_method_ids) const;

  
  
  void GetLanguageCodesFromInputMethodIds(
      const std::vector<std::string>& input_method_ids,
      std::vector<std::string>* out_language_codes) const;

  
  
  std::string GetHardwareInputMethodId() const;

  
  bool IsValidInputMethodId(const std::string& input_method_id) const;

  
  static bool IsKeyboardLayout(const std::string& input_method_id);

  
  void SetComponentExtensions(const InputMethodDescriptors& imes);

  
  
  
  static InputMethodDescriptor GetFallbackInputMethodDescriptor();

 protected:
  
  bool GetInputMethodIdsFromLanguageCodeInternal(
      const std::multimap<std::string, std::string>& language_code_to_ids,
      const std::string& normalized_language_code,
      InputMethodType type,
      std::vector<std::string>* out_input_method_ids) const;

  
  void ReloadInternalMaps();

  
  
  scoped_ptr<InputMethodDescriptors> supported_input_methods_;

  
  
  
  
  
  
  
  
  
  
  std::string GetKeyboardLayoutName(const std::string& input_method_id) const;

 private:
  bool TranslateStringInternal(const std::string& english_string,
                               base::string16 *out_string) const;

  
  typedef std::multimap<std::string, std::string> LanguageCodeToIdsMap;
  
  typedef std::map<
    std::string, InputMethodDescriptor> InputMethodIdToDescriptorMap;
  
  typedef std::map<std::string, InputMethodDescriptor> XkbIdToDescriptorMap;
  
  typedef std::map<std::string, InputMethodDescriptor> ComponentExtIMEMap;

  LanguageCodeToIdsMap language_code_to_ids_;
  std::map<std::string, std::string> id_to_language_code_;
  InputMethodIdToDescriptorMap id_to_descriptor_;
  XkbIdToDescriptorMap xkb_id_to_descriptor_;
  ComponentExtIMEMap component_extension_ime_id_to_descriptor_;

  typedef base::hash_map<std::string, int> HashType;
  HashType english_to_resource_id_;

  InputMethodDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodUtil);
};

}  
}  

#endif  
