// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGINS_PLUGIN_FINDER_H_
#define CHROME_BROWSER_PLUGINS_PLUGIN_FINDER_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "content/public/common/webplugininfo.h"

namespace base {
class DictionaryValue;
}

class GURL;
class PluginMetadata;
class PrefRegistrySimple;

#if defined(ENABLE_PLUGIN_INSTALLATION)
class PluginInstaller;
#endif

class PluginFinder {
 public:
  static void RegisterPrefs(PrefRegistrySimple* registry);

  static PluginFinder* GetInstance();

  
  void Init();

  void ReinitializePlugins(const base::DictionaryValue* json_metadata);

#if defined(ENABLE_PLUGIN_INSTALLATION)
  
  
  
  
  bool FindPlugin(const std::string& mime_type,
                  const std::string& language,
                  PluginInstaller** installer,
                  scoped_ptr<PluginMetadata>* plugin_metadata);

  
  
  
  bool FindPluginWithIdentifier(const std::string& identifier,
                                PluginInstaller** installer,
                                scoped_ptr<PluginMetadata>* plugin_metadata);
#endif

  
  base::string16 FindPluginNameWithIdentifier(const std::string& identifier);

  
  scoped_ptr<PluginMetadata> GetPluginMetadata(
      const content::WebPluginInfo& plugin);

 private:
  friend struct DefaultSingletonTraits<PluginFinder>;
  friend class Singleton<PluginFinder>;
  FRIEND_TEST_ALL_PREFIXES(PluginFinderTest, JsonSyntax);
  FRIEND_TEST_ALL_PREFIXES(PluginFinderTest, PluginGroups);

  PluginFinder();
  ~PluginFinder();

  
  
  static base::DictionaryValue* LoadBuiltInPluginList();

#if defined(ENABLE_PLUGIN_INSTALLATION)
  std::map<std::string, PluginInstaller*> installers_;
#endif

  std::map<std::string, PluginMetadata*> identifier_plugin_;

  
  
  
  int version_;

  
  
  base::Lock mutex_;

  DISALLOW_COPY_AND_ASSIGN(PluginFinder);
};

#endif  
