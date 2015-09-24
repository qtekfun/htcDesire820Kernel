// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_BUNDLE_INSTALLER_H_
#define CHROME_BROWSER_EXTENSIONS_BUNDLE_INSTALLER_H_

#include <string>
#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/extensions/extension_install_prompt.h"
#include "chrome/browser/extensions/webstore_install_helper.h"
#include "chrome/browser/extensions/webstore_installer.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/host_desktop.h"
#include "extensions/common/extension.h"

namespace base {
class DictionaryValue;
}  

namespace content {
class NavigationController;
}  

class Browser;
class Profile;

namespace extensions {

class BundleInstaller : public WebstoreInstallHelper::Delegate,
                        public ExtensionInstallPrompt::Delegate,
                        public WebstoreInstaller::Delegate,
                        public chrome::BrowserListObserver,
                        public base::RefCountedThreadSafe<BundleInstaller> {
 public:
  
  static void SetAutoApproveForTesting(bool approve);

  class Delegate {
   public:
    virtual void OnBundleInstallApproved() {}
    virtual void OnBundleInstallCanceled(bool user_initiated) {}
    virtual void OnBundleInstallCompleted() {}

   protected:
    virtual ~Delegate() {}
  };

  
  struct Item {
    
    
    enum State {
      STATE_PENDING,
      STATE_INSTALLED,
      STATE_FAILED
    };

    Item();

    
    base::string16 GetNameForDisplay();

    std::string id;
    std::string manifest;
    std::string localized_name;
    State state;
  };

  typedef std::vector<Item> ItemList;

  BundleInstaller(Browser* browser, const ItemList& items);

  
  bool approved() const { return approved_; }

  
  ItemList GetItemsWithState(Item::State state) const;

  
  
  
  
  void PromptForApproval(Delegate* delegate);

  
  
  
  
  
  
  void CompleteInstall(content::NavigationController* controller,
                       Delegate* delegate);

  
  
  
  
  
  
  
  base::string16 GetHeadingTextFor(Item::State state) const;

 private:
  friend class base::RefCountedThreadSafe<BundleInstaller>;

  typedef std::map<std::string, Item> ItemMap;
  typedef std::map<std::string, linked_ptr<base::DictionaryValue> > ManifestMap;

  virtual ~BundleInstaller();

  
  
  static void ShowInstalledBubble(const BundleInstaller* bundle,
                                  Browser* browser);

  
  void ParseManifests();

  
  void ReportApproved();

  
  void ReportCanceled(bool user_initiated);

  
  void ReportComplete();

  
  
  void ShowPromptIfDoneParsing();

  
  void ShowPrompt();

  
  void ShowInstalledBubbleIfDone();

  
  virtual void OnWebstoreParseSuccess(
      const std::string& id,
      const SkBitmap& icon,
      base::DictionaryValue* parsed_manifest) OVERRIDE;
  virtual void OnWebstoreParseFailure(
      const std::string& id,
      InstallHelperResultCode result_code,
      const std::string& error_message) OVERRIDE;

  
  virtual void InstallUIProceed() OVERRIDE;
  virtual void InstallUIAbort(bool user_initiated) OVERRIDE;

  
  virtual void OnExtensionInstallSuccess(const std::string& id) OVERRIDE;
  virtual void OnExtensionInstallFailure(
      const std::string& id,
      const std::string& error,
      WebstoreInstaller::FailureReason reason) OVERRIDE;

  
  virtual void OnBrowserAdded(Browser* browser) OVERRIDE;
  virtual void OnBrowserRemoved(Browser* browser) OVERRIDE;
  virtual void OnBrowserSetLastActive(Browser* browser) OVERRIDE;

  
  ExtensionList dummy_extensions_;

  
  ManifestMap parsed_manifests_;

  
  bool approved_;

  
  ItemMap items_;

  
  Browser* browser_;

  
  chrome::HostDesktopType host_desktop_type_;

  
  Profile* profile_;

  
  scoped_ptr<ExtensionInstallPrompt> install_ui_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(BundleInstaller);
};

}  

#endif  
