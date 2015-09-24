// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_USER_SCRIPT_MASTER_H_
#define CHROME_BROWSER_EXTENSIONS_USER_SCRIPT_MASTER_H_

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/strings/string_piece.h"
#include "chrome/common/extensions/extension_messages.h"
#include "chrome/common/extensions/extension_set.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/common/user_script.h"

namespace content {
class RenderProcessHost;
}

class Profile;

typedef std::map<std::string, ExtensionSet::ExtensionPathAndDefaultLocale>
    ExtensionsInfo;

namespace extensions {

class UserScriptMaster : public base::RefCountedThreadSafe<UserScriptMaster>,
                         public content::NotificationObserver {
 public:
  explicit UserScriptMaster(Profile* profile);

  
  
  virtual void StartLoad();

  
  base::SharedMemory* GetSharedMemory() const {
    return shared_memory_.get();
  }

  
  void NewScriptsAvailable(base::SharedMemory* handle);

  
  bool ScriptsReady() const { return shared_memory_.get() != NULL; }

 protected:
  friend class base::RefCountedThreadSafe<UserScriptMaster>;

  virtual ~UserScriptMaster();

 public:
  
  
  
  
  
  
  class ScriptReloader
      : public base::RefCountedThreadSafe<UserScriptMaster::ScriptReloader> {
   public:
    
    static bool ParseMetadataHeader(const base::StringPiece& script_text,
                                    UserScript* script);

    explicit ScriptReloader(UserScriptMaster* master);

    
    
    void StartLoad(const UserScriptList& external_scripts,
                   const ExtensionsInfo& extension_info_);

    
    void DisownMaster() {
      master_ = NULL;
    }

   private:
    FRIEND_TEST_ALL_PREFIXES(UserScriptMasterTest, SkipBOMAtTheBeginning);
    FRIEND_TEST_ALL_PREFIXES(UserScriptMasterTest, LeaveBOMNotAtTheBeginning);
    friend class base::RefCountedThreadSafe<UserScriptMaster::ScriptReloader>;

    ~ScriptReloader();

    
    
    
    
    

    
    
    void NotifyMaster(base::SharedMemory* memory);

    
    
    
    
    void RunLoad(const UserScriptList& user_scripts);

    void LoadUserScripts(UserScriptList* user_scripts);

    
    
    SubstitutionMap* GetLocalizationMessages(std::string extension_id);

    
    
    UserScriptMaster* master_;

    
    ExtensionsInfo extensions_info_;

    
    
    content::BrowserThread::ID master_thread_id_;

    DISALLOW_COPY_AND_ASSIGN(ScriptReloader);
  };

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void SendUpdate(content::RenderProcessHost* process,
                  base::SharedMemory* shared_memory);

  
  content::NotificationRegistrar registrar_;

  
  scoped_refptr<ScriptReloader> script_reloader_;

  
  scoped_ptr<base::SharedMemory> shared_memory_;

  
  UserScriptList user_scripts_;

  
  ExtensionsInfo extensions_info_;

  
  
  bool extensions_service_ready_;

  
  
  
  bool pending_load_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(UserScriptMaster);
};

}  

#endif  
