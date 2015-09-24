// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGIN_UPDATER_H_
#define CHROME_BROWSER_PLUGIN_UPDATER_H_
#pragma once

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/memory/singleton.h"
#include "content/common/notification_observer.h"

class DictionaryValue;
class ListValue;
class NotificationDetails;
class NotificationSource;
class Profile;

namespace webkit {
namespace npapi {
class PluginGroup;
struct WebPluginInfo;
}
}

class PluginUpdater : public NotificationObserver {
 public:
  
  
  static ListValue* GetPluginGroupsData();

  
  void EnablePluginGroup(bool enable, const string16& group_name);

  
  void EnablePlugin(bool enable, const FilePath::StringType& file_path);

  
  void UpdatePluginGroupsStateFromPrefs(Profile* profile);

  
  void UpdatePreferences(Profile* profile, int delay_ms);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  static PluginUpdater* GetInstance();

 private:
  PluginUpdater();
  virtual ~PluginUpdater() {}

  
  
  static void GetPreferencesDataOnFileThread(void* profile);

  
  static void OnUpdatePreferences(
      Profile* profile,
      const std::vector<webkit::npapi::WebPluginInfo>& plugins,
      const std::vector<webkit::npapi::PluginGroup>& groups);

  
  
  void NotifyPluginStatusChanged();

  
  static void OnNotifyPluginStatusChanged();

  static DictionaryValue* CreatePluginFileSummary(
      const webkit::npapi::WebPluginInfo& plugin);

  
  
  
  
  void UpdatePluginsStateFromPolicy(const ListValue* disabled_list,
                                    const ListValue* exceptions_list,
                                    const ListValue* enabled_list);

  void ListValueToStringSet(const ListValue* src, std::set<string16>* dest);

  
  friend struct DefaultSingletonTraits<PluginUpdater>;

  bool notify_pending_;

  DISALLOW_COPY_AND_ASSIGN(PluginUpdater);
};

#endif  
