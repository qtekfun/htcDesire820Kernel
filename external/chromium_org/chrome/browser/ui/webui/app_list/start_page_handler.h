// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_APP_LIST_START_PAGE_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_APP_LIST_START_PAGE_HANDLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/app_list/recommended_apps_observer.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class ListValue;
}

namespace app_list {

class RecommendedApps;

class StartPageHandler : public content::WebUIMessageHandler,
                         public RecommendedAppsObserver {
 public:
  StartPageHandler();
  virtual ~StartPageHandler();

 private:
  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void OnRecommendedAppsChanged() OVERRIDE;

  
  void SendRecommendedApps();

  
  void HandleInitialize(const base::ListValue* args);
  void HandleLaunchApp(const base::ListValue* args);
  void HandleSpeechResult(const base::ListValue* args);
  void HandleSpeechSoundLevel(const base::ListValue* args);
  void HandleSpeechRecognition(const base::ListValue* args);

  RecommendedApps* recommended_apps_;  

  DISALLOW_COPY_AND_ASSIGN(StartPageHandler);
};

}  

#endif  
