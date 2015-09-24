// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ENUMERATE_MODULES_MODEL_WIN_H_
#define CHROME_BROWSER_ENUMERATE_MODULES_MODEL_WIN_H_
#pragma once

#include <utility>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "base/string16.h"
#include "base/timer.h"
#include "content/browser/browser_thread.h"
#include "googleurl/src/gurl.h"

class EnumerateModulesModel;
class FilePath;
class ListValue;

class ModuleEnumerator : public base::RefCountedThreadSafe<ModuleEnumerator> {
 public:
  
  
  
  
  enum ModuleType {
    LOADED_MODULE               = 1 << 0,
    SHELL_EXTENSION             = 1 << 1,
    WINSOCK_MODULE_REGISTRATION = 1 << 2,
  };

  
  
  
  
  enum ModuleStatus {
    
    
    
    NOT_MATCHED,
    
    GOOD,
    
    SUSPECTED_BAD,
    
    CONFIRMED_BAD,
  };

  
  enum RecommendedAction {
    NONE          = 0,
    INVESTIGATING = 1 << 0,
    UNINSTALL     = 1 << 1,
    DISABLE       = 1 << 2,
    UPDATE        = 1 << 3,
    SEE_LINK      = 1 << 4,
  };

  
  struct Module {
    
    ModuleType type;
    
    ModuleStatus status;
    
    string16 location;
    
    string16 name;
    
    string16 product_name;
    
    string16 description;
    
    string16 version;
    
    string16 digital_signer;
    
    RecommendedAction recommended_action;
    
    int duplicate_count;
    
    
    bool normalized;
  };

  
  typedef std::vector<Module> ModulesVector;

  
  struct BlacklistEntry {
    const char* filename;
    const char* location;
    const char* desc_or_signer;
    const char* version_from;  
    const char* version_to;    
    RecommendedAction help_tip;
  };

  
  
  
  
  
  
  
  static void NormalizeModule(Module* module);

  
  static ModuleStatus Match(const Module& module,
                            const BlacklistEntry& blacklisted);

  explicit ModuleEnumerator(EnumerateModulesModel* observer);
  ~ModuleEnumerator();

  
  
  
  
  
  
  
  
  
  void ScanNow(ModulesVector* list, bool limited_mode);

 private:
  FRIEND_TEST_ALL_PREFIXES(EnumerateModulesTest, CollapsePath);

  
  static const BlacklistEntry kModuleBlacklist[];

  
  
  
  
  
  
  
  void ScanImpl();

  
  void EnumerateLoadedModules();

  
  void EnumerateShellExtensions();

  
  void EnumerateWinsockModules();

  
  
  void ReadShellExtensions(HKEY parent);

  
  
  void PopulateModuleInformation(Module* module);

  
  
  
  void AddToListWithoutDuplicating(const Module&);

  
  
  
  
  
  
  
  void PreparePathMappings();

  
  
  void CollapsePath(Module* module);

  
  
  void MatchAgainstBlacklist();

  
  
  void ReportBack();

  
  
  string16 GetSubjectNameFromDigitalSignature(const FilePath& filename);

  
  typedef std::vector< std::pair<string16, string16> > PathMapping;

  
  
  PathMapping path_mapping_;

  
  
  ModulesVector* enumerated_modules_;

  
  EnumerateModulesModel* observer_;

  
  bool limited_mode_;

  
  BrowserThread::ID callback_thread_id_;

  DISALLOW_COPY_AND_ASSIGN(ModuleEnumerator);
};

class EnumerateModulesModel {
 public:
  static EnumerateModulesModel* GetInstance();

  
  
  bool ShouldShowConflictWarning() const;

  
  void AcknowledgeConflictNotification();

  
  
  int suspected_bad_modules_detected() const {
    return suspected_bad_modules_detected_;
  }

  
  
  int confirmed_bad_modules_detected() const {
    return confirmed_bad_modules_detected_;
  }

  
  
  void set_limited_mode(bool limited_mode) {
    limited_mode_ = limited_mode;
  }

  
  
  void ScanNow();

  
  ListValue* GetModuleList() const;

 private:
  friend struct DefaultSingletonTraits<EnumerateModulesModel>;
  friend class ModuleEnumerator;

  EnumerateModulesModel();
  virtual ~EnumerateModulesModel();

  
  void DoneScanning();

  
  
  
  GURL ConstructHelpCenterUrl(const ModuleEnumerator::Module& module) const;

  
  
  ModuleEnumerator::ModulesVector enumerated_modules_;

  
  scoped_refptr<ModuleEnumerator> module_enumerator_;

  
  
  base::OneShotTimer<EnumerateModulesModel> check_modules_timer_;

  
  
  
  
  
  bool limited_mode_;

  
  bool scanning_;

  
  bool conflict_notification_acknowledged_;

  
  
  int confirmed_bad_modules_detected_;

  
  
  int suspected_bad_modules_detected_;

  DISALLOW_COPY_AND_ASSIGN(EnumerateModulesModel);
};

#endif  
