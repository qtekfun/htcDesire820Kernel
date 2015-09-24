// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_CHANNEL_INFO_H_
#define CHROME_INSTALLER_UTIL_CHANNEL_INFO_H_

#include <string>

namespace base {
namespace win {
class RegKey;
}
}

namespace installer {

class ChannelInfo {
 public:

  
  
  
  
  
  bool Initialize(const base::win::RegKey& key);

  
  // Returns false if the value could not be written to the registry.
  bool Write(base::win::RegKey* key) const;

  const std::wstring& value() const { return value_; }
  void set_value(const std::wstring& value) { value_ = value; }
  bool Equals(const ChannelInfo& other) const {
    return value_ == other.value_;
  }

  
  
  
  
  bool GetChannelName(std::wstring* channel_name) const;

  
  bool IsChrome() const;

  
  
  bool SetChrome(bool value);

  
  bool IsChromeFrame() const;

  
  
  bool SetChromeFrame(bool value);

  
  bool IsAppLauncher() const;

  
  
  bool SetAppLauncher(bool value);

  
  bool IsMultiInstall() const;

  
  
  bool SetMultiInstall(bool value);

  
  bool IsReadyMode() const;

  
  
  bool SetReadyMode(bool value);

  
  
  
  bool SetStage(const wchar_t* stage);

  
  
  std::wstring GetStage() const;

  
  bool HasFullSuffix() const;

  
  
  bool SetFullSuffix(bool value);

  
  bool HasMultiFailSuffix() const;

  
  
  bool SetMultiFailSuffix(bool value);

  
  
  bool SetMigratingSuffix(bool value);

  
  bool HasMigratingSuffix() const;

  
  
  bool RemoveAllModifiersAndSuffixes();

 private:
  std::wstring value_;
};  

}  

#endif  
