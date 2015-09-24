// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_COMMANDS_COMMAND_SERVICE_H_
#define CHROME_BROWSER_EXTENSIONS_API_COMMANDS_COMMAND_SERVICE_H_

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/common/extensions/command.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_source.h"
#include "extensions/common/extension.h"

class Profile;

namespace base {
class DictionaryValue;
}

namespace ui {
class Accelerator;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace extensions {

class CommandService : public ProfileKeyedAPI,
                       public content::NotificationObserver {
 public:
  
  
  enum QueryType {
    ALL,
    ACTIVE_ONLY,
  };

  
  
  
  enum CommandScope {
    REGULAR,    
    GLOBAL,     
    ANY_SCOPE,  
  };

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  explicit CommandService(Profile* profile);
  virtual ~CommandService();

  
  static ProfileKeyedAPIFactory<CommandService>* GetFactoryInstance();

  
  static CommandService* Get(Profile* profile);

  
  
  
  static bool IsMediaKey(const ui::Accelerator& accelerator);

  
  
  
  
  
  
  bool GetBrowserActionCommand(const std::string& extension_id,
                               QueryType type,
                               extensions::Command* command,
                               bool* active);

  
  
  
  
  
  
  bool GetPageActionCommand(const std::string& extension_id,
                            QueryType type,
                            extensions::Command* command,
                            bool* active);

  
  
  
  
  
  
  bool GetScriptBadgeCommand(const std::string& extension_id,
                             QueryType type,
                             extensions::Command* command,
                             bool* active);

  
  
  
  
  
  bool GetNamedCommands(const std::string& extension_id,
                        QueryType type,
                        CommandScope scope,
                        extensions::CommandMap* command_map);

  
  
  
  
  
  // |accelerator| will be overwritten. If |global| is true, the command will
  
  
  bool AddKeybindingPref(const ui::Accelerator& accelerator,
                         std::string extension_id,
                         std::string command_name,
                         bool allow_overrides,
                         bool global);

  
  
  
  void RemoveKeybindingPrefs(const std::string& extension_id,
                             const std::string& command_name);

  
  
  
  void UpdateKeybindingPrefs(const std::string& extension_id,
                             const std::string& command_name,
                             const std::string& keystroke);

  
  
  
  bool SetScope(const std::string& extension_id,
                const std::string& command_name,
                bool global);

  
  
  
  Command FindCommandByName(const std::string& extension_id,
                            const std::string& command);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<CommandService>;

  
  static const char* service_name() {
    return "CommandService";
  }
  static const bool kServiceRedirectedInIncognito = true;

  
  enum ExtensionActionType {
    BROWSER_ACTION,
    PAGE_ACTION,
    SCRIPT_BADGE,
  };

  
  
  
  
  
  void AssignInitialKeybindings(const extensions::Extension* extension);

  bool GetExtensionActionCommand(const std::string& extension_id,
                                 QueryType query_type,
                                 extensions::Command* command,
                                 bool* active,
                                 ExtensionActionType action_type);

  
  content::NotificationRegistrar registrar_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(CommandService);
};

template <>
void ProfileKeyedAPIFactory<CommandService>::DeclareFactoryDependencies();

}  

#endif  
