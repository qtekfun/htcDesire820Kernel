// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGINS_PLUGIN_STATUS_PREF_SETTER_H_
#define CHROME_BROWSER_PLUGINS_PLUGIN_STATUS_PREF_SETTER_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_member.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class PluginPrefs;
class PrefService;
class Profile;

namespace content {
struct WebPluginInfo;
}

class PluginStatusPrefSetter : public content::NotificationObserver {
 public:
  PluginStatusPrefSetter();
  virtual ~PluginStatusPrefSetter();

  
  
  
  
  void Init(Profile* profile,
            const BooleanPrefMember::NamedChangeCallback& observer);

  bool IsClearPluginLSODataEnabled() const {
    return clear_plugin_lso_data_enabled_.GetValue();
  }

  bool IsPepperFlashSettingsEnabled() const {
    return pepper_flash_settings_enabled_.GetValue();
  }

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  void StartUpdate();
  void GotPlugins(scoped_refptr<PluginPrefs> plugin_prefs,
                  const std::vector<content::WebPluginInfo>& plugins);

  content::NotificationRegistrar registrar_;
  
  Profile* profile_;

  
  BooleanPrefMember clear_plugin_lso_data_enabled_;
  
  BooleanPrefMember pepper_flash_settings_enabled_;

  base::WeakPtrFactory<PluginStatusPrefSetter> factory_;

  DISALLOW_COPY_AND_ASSIGN(PluginStatusPrefSetter);
};

#endif  
