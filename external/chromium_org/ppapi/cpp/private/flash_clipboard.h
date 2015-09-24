// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_FLASH_CLIPBOARD_H_
#define PPAPI_CPP_PRIVATE_FLASH_CLIPBOARD_H_

#include <string>
#include <vector>

#include "ppapi/c/private/ppb_flash_clipboard.h"
#include "ppapi/cpp/var.h"

namespace pp {

class InstanceHandle;

namespace flash {

class Clipboard {
 public:
  
  static bool IsAvailable();

  
  
  static uint32_t RegisterCustomFormat(const InstanceHandle& instance,
                                       const std::string& format_name);

  
  static bool IsFormatAvailable(const InstanceHandle& instance,
                                PP_Flash_Clipboard_Type clipboard_type,
                                uint32_t format);

  
  
  static bool ReadData(const InstanceHandle& instance,
                       PP_Flash_Clipboard_Type clipboard_type,
                       uint32_t clipboard_format,
                       Var* out);

  // Returns true on success in which case all of |data| will be written to
  // the clipboard. Otherwise nothing will be written.
  static bool WriteData(const InstanceHandle& instance,
                        PP_Flash_Clipboard_Type clipboard_type,
                        const std::vector<uint32_t>& formats,
                        const std::vector<Var>& data_items);
};

}  
}  

#endif  
