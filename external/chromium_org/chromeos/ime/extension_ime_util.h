// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_IME_EXTENSION_IME_UTIL_H_
#define CHROMEOS_IME_EXTENSION_IME_UTIL_H_

#include <string>

#include "chromeos/chromeos_export.h"

namespace chromeos {

namespace extension_ime_util {

std::string CHROMEOS_EXPORT GetInputMethodID(const std::string& extension_id,
                                             const std::string& engine_id);

std::string CHROMEOS_EXPORT GetComponentInputMethodID(
    const std::string& extension_id,
    const std::string& engine_id);

std::string CHROMEOS_EXPORT GetExtensionIDFromInputMethodID(
    const std::string& input_method_id);

bool CHROMEOS_EXPORT IsExtensionIME(const std::string& input_method_id);

bool CHROMEOS_EXPORT IsComponentExtensionIME(
    const std::string& input_method_id);

bool CHROMEOS_EXPORT IsMemberOfExtension(const std::string& input_method_id,
                                         const std::string& extension_id);

}  

}  

#endif  
