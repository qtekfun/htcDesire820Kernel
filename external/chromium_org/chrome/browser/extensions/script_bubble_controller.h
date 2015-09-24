// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_SCRIPT_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_EXTENSIONS_SCRIPT_BUBBLE_CONTROLLER_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/extensions/tab_helper.h"
#include "content/public/browser/web_contents_observer.h"

class ExtensionService;

namespace extensions {

class ExtensionSystem;

class ScriptBubbleController
    : public TabHelper::ScriptExecutionObserver,
      public content::WebContentsObserver {
 public:
  ScriptBubbleController(content::WebContents* web_contents,
                         TabHelper* tab_helper);
  virtual ~ScriptBubbleController();

  
  const std::set<std::string>& extensions_running_scripts() {
    return extensions_running_scripts_;
  }

  
  virtual void OnScriptsExecuted(
      const content::WebContents* web_contents,
      const ExecutingScriptsMap& extension_ids,
      int32 page_id,
      const GURL& on_url) OVERRIDE;

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;

  void OnExtensionUnloaded(const std::string& extension_id);

 private:
  
  Profile* profile() const;

  
  
  ExtensionService* GetExtensionService() const;

  
  
  void UpdateScriptBubble();

  
  std::set<std::string> extensions_running_scripts_;

  DISALLOW_COPY_AND_ASSIGN(ScriptBubbleController);
};

}  

#endif  
