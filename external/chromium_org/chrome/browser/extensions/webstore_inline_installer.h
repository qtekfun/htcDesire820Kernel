// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_WEBSTORE_INLINE_INSTALLER_H_
#define CHROME_BROWSER_EXTENSIONS_WEBSTORE_INLINE_INSTALLER_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "content/public/browser/web_contents_observer.h"
#include "webstore_standalone_installer.h"

namespace content {
class WebContents;
}

namespace extensions {

class WebstoreInlineInstaller
    : public WebstoreStandaloneInstaller,
      public content::WebContentsObserver {
 public:
  typedef WebstoreStandaloneInstaller::Callback Callback;

  WebstoreInlineInstaller(content::WebContents* web_contents,
                          const std::string& webstore_item_id,
                          const GURL& requestor_url,
                          const Callback& callback);

 protected:
  friend class base::RefCountedThreadSafe<WebstoreInlineInstaller>;

  virtual ~WebstoreInlineInstaller();

  
  virtual bool CheckRequestorAlive() const OVERRIDE;
  virtual const GURL& GetRequestorURL() const OVERRIDE;
  virtual bool ShouldShowPostInstallUI() const OVERRIDE;
  virtual bool ShouldShowAppInstalledBubble() const OVERRIDE;
  virtual content::WebContents* GetWebContents() const OVERRIDE;
  virtual scoped_ptr<ExtensionInstallPrompt::Prompt>
      CreateInstallPrompt() const OVERRIDE;
  virtual bool CheckInlineInstallPermitted(
      const base::DictionaryValue& webstore_data,
      std::string* error) const OVERRIDE;
  virtual bool CheckRequestorPermitted(
      const base::DictionaryValue& webstore_data,
      std::string* error) const OVERRIDE;

 private:
  
  virtual void WebContentsDestroyed(
      content::WebContents* web_contents) OVERRIDE;

  
  
  static bool IsRequestorURLInVerifiedSite(const GURL& requestor_url,
                                           const std::string& verified_site);

  GURL requestor_url_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(WebstoreInlineInstaller);
};

}  

#endif  
