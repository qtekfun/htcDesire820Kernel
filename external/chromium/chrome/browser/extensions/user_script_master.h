// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_USER_SCRIPT_MASTER_H_
#define CHROME_BROWSER_EXTENSIONS_USER_SCRIPT_MASTER_H_
#pragma once

#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/shared_memory.h"
#include "chrome/common/extensions/user_script.h"
#include "content/browser/browser_thread.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

namespace base {
class StringPiece;
}

class Profile;

class UserScriptMaster : public base::RefCountedThreadSafe<UserScriptMaster>,
                         public NotificationObserver {
 public:
  
  
  explicit UserScriptMaster(const FilePath& script_dir, Profile* profile);

  
  
  virtual void StartScan();

  
  base::SharedMemory* GetSharedMemory() const {
    return shared_memory_.get();
  }

  
  void NewScriptsAvailable(base::SharedMemory* handle);

  
  bool ScriptsReady() const { return shared_memory_.get() != NULL; }

  
  FilePath user_script_dir() const { return user_script_dir_; }

 protected:
  friend class base::RefCountedThreadSafe<UserScriptMaster>;

  virtual ~UserScriptMaster();

 private:
  FRIEND_TEST_ALL_PREFIXES(UserScriptMasterTest, Parse1);
  FRIEND_TEST_ALL_PREFIXES(UserScriptMasterTest, Parse2);
  FRIEND_TEST_ALL_PREFIXES(UserScriptMasterTest, Parse3);
  FRIEND_TEST_ALL_PREFIXES(UserScriptMasterTest, Parse4);
  FRIEND_TEST_ALL_PREFIXES(UserScriptMasterTest, Parse5);
  FRIEND_TEST_ALL_PREFIXES(UserScriptMasterTest, Parse6);

 public:
  
  
  
  
  
  
  class ScriptReloader
      : public base::RefCountedThreadSafe<UserScriptMaster::ScriptReloader> {
   public:
    
    static bool ParseMetadataHeader(const base::StringPiece& script_text,
                                    UserScript* script);

    static void LoadScriptsFromDirectory(const FilePath& script_dir,
                                         UserScriptList* result);

    explicit ScriptReloader(UserScriptMaster* master);

    
    
    void StartScan(const FilePath& script_dir,
                   const UserScriptList& external_scripts);

    
    void DisownMaster() {
      master_ = NULL;
    }

   private:
    friend class base::RefCountedThreadSafe<UserScriptMaster::ScriptReloader>;

    ~ScriptReloader() {}

    
    
    
    
    
    

    
    
    void NotifyMaster(base::SharedMemory* memory);

    
    
    
    
    void RunScan(const FilePath script_dir, UserScriptList lone_scripts);

    
    
    UserScriptMaster* master_;

    
    
    BrowserThread::ID master_thread_id_;

    DISALLOW_COPY_AND_ASSIGN(ScriptReloader);
  };

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  NotificationRegistrar registrar_;

  
  FilePath user_script_dir_;

  
  scoped_refptr<ScriptReloader> script_reloader_;

  
  scoped_ptr<base::SharedMemory> shared_memory_;

  
  UserScriptList lone_scripts_;

  
  
  bool extensions_service_ready_;

  
  
  
  bool pending_scan_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(UserScriptMaster);
};

#endif  
