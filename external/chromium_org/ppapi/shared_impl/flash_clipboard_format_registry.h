// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_FLASH_CLIPBOARD_FORMAT_REGISTRY_H_
#define PPAPI_SHARED_IMPL_FLASH_CLIPBOARD_FORMAT_REGISTRY_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "ppapi/c/private/ppb_flash_clipboard.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

class PPAPI_SHARED_EXPORT FlashClipboardFormatRegistry {
 public:
  FlashClipboardFormatRegistry();
  ~FlashClipboardFormatRegistry();

  
  
  
  
  
  uint32_t RegisterFormat(const std::string& format_name);

  
  
  
  void SetRegisteredFormat(const std::string& format_name, uint32_t format);

  
  bool IsFormatRegistered(uint32_t format) const;

  
  std::string GetFormatName(uint32_t format) const;

  
  
  
  uint32_t GetFormatID(const std::string& format_name) const;

  
  
  static bool IsValidPredefinedFormat(uint32_t format);

 private:
  
  typedef std::map<uint32_t, std::string> FormatMap;
  FormatMap custom_formats_;

  DISALLOW_COPY_AND_ASSIGN(FlashClipboardFormatRegistry);
};

}  

#endif  

