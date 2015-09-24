// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_PLUGINS_PLUGIN_LIST_H_
#define WEBKIT_GLUE_PLUGINS_PLUGIN_LIST_H_


#include "webkit/plugins/npapi/plugin_list.h"

namespace NPAPI {

typedef webkit::npapi::PluginEntryPoints PluginEntryPoints;

class PluginList {
 public:
  static inline webkit::npapi::PluginList* Singleton() {
    return webkit::npapi::PluginList::Singleton();
  }
};

}  

#endif  
