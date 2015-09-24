// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_APP_COMMANDS_H_
#define CHROME_INSTALLER_UTIL_APP_COMMANDS_H_

#include <windows.h>

#include <map>
#include <string>
#include <utility>

#include "base/basictypes.h"
#include "chrome/installer/util/app_command.h"

namespace base {
namespace win {
class RegKey;
}
}

namespace installer {

class AppCommands {
 public:
  typedef std::map<std::wstring, AppCommand> CommandMap;
  typedef std::pair<CommandMap::const_iterator, CommandMap::const_iterator>
      CommandMapRange;

  AppCommands();
  ~AppCommands();

  
  
  
  
  bool Initialize(const base::win::RegKey& key);

  
  AppCommands& CopyFrom(const AppCommands& other);

  
  void Clear();

  
  
  bool Get(const std::wstring& command_id, AppCommand* command) const;

  
  
  
  bool Set(const std::wstring& command_id, const AppCommand& command);

  
  
  bool Remove(const std::wstring& command_id);

  
  
  CommandMapRange GetIterators() const;

 protected:
  CommandMap commands_;

  DISALLOW_COPY_AND_ASSIGN(AppCommands);
};

}  

#endif  
