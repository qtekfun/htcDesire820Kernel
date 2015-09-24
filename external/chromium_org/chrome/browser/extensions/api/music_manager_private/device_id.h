// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_MUSIC_MANAGER_PRIVATE_DEVICE_ID_H_
#define CHROME_BROWSER_EXTENSIONS_API_MUSIC_MANAGER_PRIVATE_DEVICE_ID_H_

#include <string>

#include "base/bind.h"

namespace extensions {
namespace api {

class DeviceId {
 public:
  typedef base::Callback<void(const std::string&)> IdCallback;

  
  
  
  
  
  
  
  
  
  
  
  
  static void GetDeviceId(const std::string& extension_id,
                          const IdCallback& callback);

 private:
  
  static void GetRawDeviceId(const IdCallback& callback);

  
  
  
  
  static bool IsValidMacAddress(const void* bytes, size_t size);
};

}  
}  

#endif  
