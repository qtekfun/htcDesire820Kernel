// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_COMMANDS_COMMANDS_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_API_COMMANDS_COMMANDS_HANDLER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/common/extensions/command.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest.h"
#include "extensions/common/manifest_handler.h"

namespace extensions {

struct CommandsInfo : public Extension::ManifestData {
  CommandsInfo();
  virtual ~CommandsInfo();

  
  
  
  
  scoped_ptr<Command> browser_action_command;
  scoped_ptr<Command> page_action_command;
  scoped_ptr<Command> script_badge_command;
  CommandMap named_commands;

  static const Command* GetBrowserActionCommand(const Extension* extension);
  static const Command* GetPageActionCommand(const Extension* extension);
  static const Command* GetScriptBadgeCommand(const Extension* extension);
  static const CommandMap* GetNamedCommands(const Extension* extension);
};

class CommandsHandler : public ManifestHandler {
 public:
  CommandsHandler();
  virtual ~CommandsHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;
  virtual bool AlwaysParseForType(Manifest::Type type) const OVERRIDE;

 private:
  
  
  
  void MaybeSetBrowserActionDefault(const Extension* extension,
                                    CommandsInfo* info);

  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(CommandsHandler);
};

}  

#endif  
