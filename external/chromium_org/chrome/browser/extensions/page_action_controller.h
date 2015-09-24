// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_PAGE_ACTION_CONTROLLER_H_
#define CHROME_BROWSER_EXTENSIONS_PAGE_ACTION_CONTROLLER_H_

#include <set>
#include <string>

#include "base/observer_list.h"
#include "chrome/browser/extensions/location_bar_controller.h"
#include "content/public/browser/web_contents_observer.h"

class ExtensionService;
class Profile;

namespace extensions {

class PageActionController : public LocationBarController,
                             public content::WebContentsObserver {
 public:
  explicit PageActionController(content::WebContents* web_contents);
  virtual ~PageActionController();

  
  virtual std::vector<ExtensionAction*> GetCurrentActions() const OVERRIDE;
  
  virtual void GetAttentionFor(const std::string& extension_id) OVERRIDE {}
  virtual Action OnClicked(const std::string& extension_id,
                           int mouse_button) OVERRIDE;
  virtual void NotifyChange() OVERRIDE;

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;

 private:
  
  Profile* profile() const;

  
  ExtensionService* GetExtensionService() const;

  DISALLOW_COPY_AND_ASSIGN(PageActionController);
};

}  

#endif  
