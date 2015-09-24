// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_PLUGIN_LIST_H_
#define CONTENT_COMMON_PLUGIN_LIST_H_

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/lazy_instance.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "content/public/common/webplugininfo.h"

class GURL;

namespace content {

class CONTENT_EXPORT PluginList {
 public:

  
  static PluginList* Singleton();

  
  
  static bool DebugPluginLoading();

  
  
  static bool SupportsType(const WebPluginInfo& plugin,
                           const std::string& mime_type,
                           bool allow_wildcard);

  
  
  void DisablePluginsDiscovery();

  
  
  void RefreshPlugins();

  
  
  void AddExtraPluginPath(const base::FilePath& plugin_path);
  void RemoveExtraPluginPath(const base::FilePath& plugin_path);

  
  void AddExtraPluginDir(const base::FilePath& plugin_dir);

  
  void GetPluginDirectories(std::vector<base::FilePath>* plugin_dirs);

  
  
  
  
  
  void RegisterInternalPlugin(const WebPluginInfo& info,
                              bool add_at_beginning);

  
  
  void UnregisterInternalPlugin(const base::FilePath& path);

  
  void GetInternalPlugins(std::vector<WebPluginInfo>* plugins);

  
  
  
  bool ReadPluginInfo(const base::FilePath& filename,
                      WebPluginInfo* info);

  
  
  
  
  static bool ParseMimeTypes(
      const std::string& mime_types,
      const std::string& file_extensions,
      const base::string16& mime_type_descriptions,
      std::vector<WebPluginMimeType>* parsed_mime_types);

  
  void GetPlugins(std::vector<WebPluginInfo>* plugins,
                  bool include_npapi);

  
  
  bool GetPluginsNoRefresh(std::vector<WebPluginInfo>* plugins);

  
  
  
  
  
  
  
  
  
  
  
  
  void GetPluginInfoArray(const GURL& url,
                          const std::string& mime_type,
                          bool allow_wildcard,
                          bool* use_stale,
                          bool include_npapi,
                          std::vector<WebPluginInfo>* info,
                          std::vector<std::string>* actual_mime_types);

  
  
  bool LoadPluginIntoPluginList(const base::FilePath& filename,
                                std::vector<WebPluginInfo>* plugins,
                                WebPluginInfo* plugin_info);

  
  

  
  void GetPluginPathsToLoad(std::vector<base::FilePath>* plugin_paths,
                            bool include_npapi);

  
  void SetPlugins(const std::vector<WebPluginInfo>& plugins);

  void set_will_load_plugins_callback(const base::Closure& callback);

  virtual ~PluginList();

  
  
  
  static bool ReadWebPluginInfo(const base::FilePath& filename,
                                WebPluginInfo* info);

#if defined(OS_POSIX) && !defined(OS_MACOSX)
  
  
  static void ParseMIMEDescription(const std::string& description,
      std::vector<WebPluginMimeType>* mime_types);

  
  
  static void ExtractVersionString(const std::string& version,
                                   WebPluginInfo* info);
#endif

 private:
  enum LoadingState {
    LOADING_STATE_NEEDS_REFRESH,
    LOADING_STATE_REFRESHING,
    LOADING_STATE_UP_TO_DATE,
  };

  friend class PluginListTest;
  friend struct base::DefaultLazyInstanceTraits<PluginList>;

  PluginList();

  
  void LoadPlugins(bool include_npapi);

  
  
  void GetPluginsInDir(const base::FilePath& path,
                       std::vector<base::FilePath>* plugins);

  
  
  
  bool ShouldLoadPluginUsingPluginList(const WebPluginInfo& info,
                                       std::vector<WebPluginInfo>* plugins);

  
  
  
  
  bool SupportsExtension(const WebPluginInfo& plugin,
                         const std::string& extension,
                         std::string* actual_mime_type);

  
  
  void RemoveExtraPluginPathLocked(const base::FilePath& plugin_path);

  
  
  

#if defined(OS_WIN)
  
  
  void GetPluginPathsFromRegistry(std::vector<base::FilePath>* plugins);
#endif

  
  
  

  
  
  
  LoadingState loading_state_;

  
  std::vector<base::FilePath> extra_plugin_paths_;

  
  std::vector<base::FilePath> extra_plugin_dirs_;

  
  std::vector<WebPluginInfo> internal_plugins_;

  
  std::vector<WebPluginInfo> plugins_list_;

  
  base::Closure will_load_plugins_callback_;

  
  
  base::Lock lock_;

  
  
  bool plugins_discovery_disabled_;

  DISALLOW_COPY_AND_ASSIGN(PluginList);
};

}  

#endif  
