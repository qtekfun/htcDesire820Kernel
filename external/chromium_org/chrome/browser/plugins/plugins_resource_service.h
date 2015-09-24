// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGINS_PLUGINS_RESOURCE_SERVICE_H_
#define CHROME_BROWSER_PLUGINS_PLUGINS_RESOURCE_SERVICE_H_

#include "chrome/browser/web_resource/web_resource_service.h"

class PrefService;
class PrefRegistrySimple;

class PluginsResourceService : public WebResourceService {
 public:
  explicit PluginsResourceService(PrefService* local_state);

  void Init();

  static void RegisterPrefs(PrefRegistrySimple* registry);

 private:
  virtual ~PluginsResourceService();

  
  virtual void Unpack(const base::DictionaryValue& parsed_json) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(PluginsResourceService);
};

#endif  
