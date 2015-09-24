// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_VERSION_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_VERSION_HANDLER_H_

#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "content/public/common/webplugininfo.h"

class VersionHandler : public content::WebUIMessageHandler {
 public:
  VersionHandler();
  virtual ~VersionHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  
  
  virtual void HandleRequestVersionInfo(const ListValue* args);

 private:
  
  
  void OnGotFilePaths(base::string16* executable_path_data,
                      base::string16* profile_path_data);

  
  
  void OnGotPlugins(const std::vector<content::WebPluginInfo>& plugins);

  
  base::WeakPtrFactory<VersionHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(VersionHandler);
};

#endif  
