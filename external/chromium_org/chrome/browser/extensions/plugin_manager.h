// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_PLUGIN_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_PLUGIN_MANAGER_H_

#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/common/extensions/manifest_handlers/nacl_modules_handler.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class GURL;
class Profile;

namespace extensions {

class PluginManager : public ProfileKeyedAPI,
                      public content::NotificationObserver {
 public:
  explicit PluginManager(Profile* profile);
  virtual ~PluginManager();

  
  static ProfileKeyedAPIFactory<PluginManager>* GetFactoryInstance();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<PluginManager>;

  
  
  
  
  
  
  void RegisterNaClModule(const NaClModuleInfo& info);
  void UnregisterNaClModule(const NaClModuleInfo& info);

  
  
  void UpdatePluginListWithNaClModules();

  extensions::NaClModuleInfo::List::iterator FindNaClModule(const GURL& url);

  
  static const char* service_name() { return "PluginManager"; }
  static const bool kServiceIsNULLWhileTesting = true;

  extensions::NaClModuleInfo::List nacl_module_list_;

  content::NotificationRegistrar registrar_;

  Profile* profile_;
};

}  

#endif  
