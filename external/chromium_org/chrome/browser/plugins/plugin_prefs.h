// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGINS_PLUGIN_PREFS_H_
#define CHROME_BROWSER_PLUGINS_PLUGIN_PREFS_H_

#include <map>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/prefs/pref_service.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/plugins/plugin_finder.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service.h"

class Profile;

namespace base {
class ListValue;
}

namespace content {
struct WebPluginInfo;
}

class PluginPrefs : public RefcountedBrowserContextKeyedService {
 public:
  enum PolicyStatus {
    NO_POLICY = 0,  
    POLICY_ENABLED,
    POLICY_DISABLED,
  };

  
  static scoped_refptr<PluginPrefs> GetForProfile(Profile* profile);

  
  
  
  static scoped_refptr<PluginPrefs> GetForTestingProfile(Profile* profile);

  
  PluginPrefs();

  
  
  
  void SetPrefs(PrefService* prefs);

  
  void EnablePluginGroup(bool enable, const base::string16& group_name);

  
  
  
  
  
  void EnablePlugin(bool enable, const base::FilePath& file_path,
                    const base::Callback<void(bool)>& callback);

  
  
  PolicyStatus PolicyStatusForPlugin(const base::string16& name) const;

  
  bool IsPluginEnabled(const content::WebPluginInfo& plugin) const;

  void set_profile(Profile* profile) { profile_ = profile; }

  
  virtual void ShutdownOnUIThread() OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<PluginPrefs>;
  friend class PluginPrefsTest;

  
  
  
  class PluginState {
   public:
    PluginState();
    ~PluginState();

    
    
    bool Get(const base::FilePath& plugin, bool* enabled) const;
    void Set(const base::FilePath& plugin, bool enabled);

   private:
    base::FilePath ConvertMapKey(const base::FilePath& plugin) const;

    std::map<base::FilePath, bool> state_;
  };

  virtual ~PluginPrefs();

  
  
  void UpdatePatternsAndNotify(std::set<base::string16>* patterns,
                               const std::string& pref_name);

  
  void SetPolicyEnforcedPluginPatterns(
      const std::set<base::string16>& disabled_patterns,
      const std::set<base::string16>& disabled_exception_patterns,
      const std::set<base::string16>& enabled_patterns);

  
  void EnablePluginGroupInternal(
      bool enabled,
      const base::string16& group_name,
      const std::vector<content::WebPluginInfo>& plugins);
  void EnablePluginInternal(
      bool enabled,
      const base::FilePath& path,
      PluginFinder* plugin_finder,
      const base::Callback<void(bool)>& callback,
      const std::vector<content::WebPluginInfo>& plugins);

  
  void OnUpdatePreferences(const std::vector<content::WebPluginInfo>& plugins);

  
  void NotifyPluginStatusChanged();

  static void ListValueToStringSet(const base::ListValue* src,
                                   std::set<base::string16>* dest);

  
  static bool IsStringMatchedInSet(const base::string16& name,
                                   const std::set<base::string16>& pattern_set);

  
  mutable base::Lock lock_;

  PluginState plugin_state_;
  std::map<base::string16, bool> plugin_group_state_;

  std::set<base::string16> policy_disabled_plugin_patterns_;
  std::set<base::string16> policy_disabled_plugin_exception_patterns_;
  std::set<base::string16> policy_enabled_plugin_patterns_;

  
  Profile* profile_;

  
  PrefService* prefs_;

  PrefChangeRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(PluginPrefs);
};

#endif  
