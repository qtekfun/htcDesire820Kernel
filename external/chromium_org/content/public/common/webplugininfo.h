// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_WEBPLUGININFO_H_
#define CONTENT_PUBLIC_COMMON_WEBPLUGININFO_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "content/common/content_export.h"

namespace base {
class Version;
}

namespace content {

struct CONTENT_EXPORT WebPluginMimeType {
  WebPluginMimeType();
  
  
  WebPluginMimeType(const std::string& m,
                    const std::string& f,
                    const std::string& d);
  ~WebPluginMimeType();

  
  std::string mime_type;

  
  std::vector<std::string> file_extensions;

  
  base::string16 description;

  
  std::vector<base::string16> additional_param_names;
  std::vector<base::string16> additional_param_values;
};

struct CONTENT_EXPORT WebPluginInfo {
  enum PluginType {
    PLUGIN_TYPE_NPAPI,
    PLUGIN_TYPE_PEPPER_IN_PROCESS,
    PLUGIN_TYPE_PEPPER_OUT_OF_PROCESS,
    PLUGIN_TYPE_PEPPER_UNSANDBOXED
  };

  WebPluginInfo();
  WebPluginInfo(const WebPluginInfo& rhs);
  ~WebPluginInfo();
  WebPluginInfo& operator=(const WebPluginInfo& rhs);

  
  WebPluginInfo(const base::string16& fake_name,
                const base::FilePath& fake_path,
                const base::string16& fake_version,
                const base::string16& fake_desc);

  bool is_pepper_plugin() const {
    return ((type == PLUGIN_TYPE_PEPPER_IN_PROCESS ) ||
          (type == PLUGIN_TYPE_PEPPER_OUT_OF_PROCESS) ||
          (type == PLUGIN_TYPE_PEPPER_UNSANDBOXED));
  }

  
  
  static void CreateVersionFromString(const base::string16& version_string,
                                      base::Version* parsed_version);

  
  base::string16 name;

  
  base::FilePath path;

  
  base::string16 version;

  
  base::string16 desc;

  
  std::vector<WebPluginMimeType> mime_types;

  
  int type;

  
  int32 pepper_permissions;
};

}  

#endif  
