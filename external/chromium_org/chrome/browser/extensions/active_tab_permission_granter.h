// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_ACTIVE_TAB_PERMISSION_GRANTER_H_
#define CHROME_BROWSER_EXTENSIONS_ACTIVE_TAB_PERMISSION_GRANTER_H_

#include <set>
#include <string>

#include "chrome/common/extensions/extension_set.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "extensions/common/url_pattern_set.h"

class Profile;

namespace content {
class WebContents;
}

namespace extensions {

class Extension;

class ActiveTabPermissionGranter : public content::WebContentsObserver,
                                   public content::NotificationObserver {
 public:
  ActiveTabPermissionGranter(content::WebContents* web_contents,
                             int tab_id,
                             Profile* profile);
  virtual ~ActiveTabPermissionGranter();

  
  
  void GrantIfRequested(const Extension* extension);

 private:
  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void WebContentsDestroyed(content::WebContents* web_contents)
      OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void ClearActiveExtensionsAndNotify();

  
  int tab_id_;

  
  
  ExtensionSet granted_extensions_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ActiveTabPermissionGranter);
};

}  

#endif  
