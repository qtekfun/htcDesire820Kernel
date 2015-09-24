// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_COMMAND_H_
#define CHROME_COMMON_EXTENSIONS_COMMAND_H_

#include <map>
#include <string>

#include "base/strings/string16.h"
#include "ui/base/accelerators/accelerator.h"

namespace base {
class DictionaryValue;
}

namespace extensions {
class Extension;
}

namespace extensions {

class Command {
 public:
  Command();
  Command(const std::string& command_name,
          const base::string16& description,
          const std::string& accelerator,
          bool global);
  ~Command();

  
  static std::string CommandPlatform();

  
  
  static ui::Accelerator StringToAccelerator(const std::string& accelerator,
                                             const std::string& command_name);

  
  
  static std::string AcceleratorToString(const ui::Accelerator& accelerator);

  
  bool Parse(const base::DictionaryValue* command,
             const std::string& command_name,
             int index,
             base::string16* error);

  
  
  base::DictionaryValue* ToValue(
      const Extension* extension, bool active) const;

  
  const std::string& command_name() const { return command_name_; }
  const ui::Accelerator& accelerator() const { return accelerator_; }
  const base::string16& description() const { return description_; }
  bool global() const { return global_; }

  
  void set_accelerator(ui::Accelerator accelerator) {
    accelerator_ = accelerator;
  }
  void set_global(bool global) {
    global_ = global;
  }

 private:
  std::string command_name_;
  ui::Accelerator accelerator_;
  base::string16 description_;
  bool global_;
};

typedef std::map<std::string, Command> CommandMap;

}  

#endif  
