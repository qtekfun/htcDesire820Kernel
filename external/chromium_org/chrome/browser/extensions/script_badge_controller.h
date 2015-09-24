// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_SCRIPT_BADGE_CONTROLLER_H_
#define CHROME_BROWSER_EXTENSIONS_SCRIPT_BADGE_CONTROLLER_H_

#include <map>
#include <set>
#include <string>

#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/location_bar_controller.h"
#include "chrome/browser/extensions/tab_helper.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"

class ExtensionAction;
class ExtensionService;
class GURL;

namespace base {
class ListValue;
}  

namespace IPC {
class Message;
}

namespace extensions {

class Extension;

class ScriptBadgeController
    : public LocationBarController,
      public TabHelper::ScriptExecutionObserver,
      public content::WebContentsObserver,
      public content::NotificationObserver {
 public:
  explicit ScriptBadgeController(content::WebContents* web_contents,
                                 TabHelper* tab_helper);
  virtual ~ScriptBadgeController();

  
  virtual std::vector<ExtensionAction*> GetCurrentActions() const OVERRIDE;
  virtual void GetAttentionFor(const std::string& extension_id) OVERRIDE;
  virtual Action OnClicked(const std::string& extension_id,
                           int mouse_button) OVERRIDE;
  virtual void NotifyChange() OVERRIDE;

  
  virtual void OnScriptsExecuted(
      const content::WebContents* web_contents,
      const ExecutingScriptsMap& extension_ids,
      int32 on_page_id,
      const GURL& on_url) OVERRIDE;

 private:
  
  Profile* profile() const;

  
  ExtensionService* GetExtensionService() const;

  
  int32 GetPageID();

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  ExtensionAction* AddExtensionToCurrentActions(
      const std::string& extension_id);

  
  
  
  bool MarkExtensionExecuting(const std::string& extension_id);

  
  
  std::set<std::string> extensions_in_current_actions_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ScriptBadgeController);
};

}  

#endif  
