// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_WEBSTORE_STANDALONE_INSTALLER_H_
#define CHROME_BROWSER_EXTENSIONS_WEBSTORE_STANDALONE_INSTALLER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_install_prompt.h"
#include "chrome/browser/extensions/webstore_data_fetcher_delegate.h"
#include "chrome/browser/extensions/webstore_install_helper.h"
#include "chrome/browser/extensions/webstore_installer.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "third_party/skia/include/core/SkBitmap.h"

class GURL;

namespace base {
class DictionaryValue;
}

namespace net {
class URLFetcher;
}

namespace extensions {
class Extension;
class WebstoreDataFetcher;


class WebstoreStandaloneInstaller
    : public base::RefCountedThreadSafe<WebstoreStandaloneInstaller>,
      public ExtensionInstallPrompt::Delegate,
      public WebstoreDataFetcherDelegate,
      public WebstoreInstaller::Delegate,
      public WebstoreInstallHelper::Delegate {
 public:
  
  
  
  typedef base::Callback<void(bool success, const std::string& error)> Callback;

  WebstoreStandaloneInstaller(const std::string& webstore_item_id,
                              Profile* profile,
                              const Callback& callback);
  void BeginInstall();

 protected:
  virtual ~WebstoreStandaloneInstaller();

  void AbortInstall();
  virtual void CompleteInstall(const std::string& error);

  

  
  
  
  virtual bool CheckRequestorAlive() const = 0;

  
  
  virtual const GURL& GetRequestorURL() const = 0;

  
  
  virtual bool ShouldShowPostInstallUI() const = 0;

  
  virtual bool ShouldShowAppInstalledBubble() const = 0;

  
  
  
  
  
  virtual content::WebContents* GetWebContents() const = 0;

  
  
  virtual scoped_ptr<ExtensionInstallPrompt::Prompt>
      CreateInstallPrompt() const = 0;

  
  
  
  virtual bool CheckInlineInstallPermitted(
      const base::DictionaryValue& webstore_data,
      std::string* error) const = 0;

  
  
  
  virtual bool CheckRequestorPermitted(
      const base::DictionaryValue& webstore_data,
      std::string* error) const = 0;

  
  
  virtual bool CheckInstallValid(
      const base::DictionaryValue& manifest,
      std::string* error);

  
  
  virtual scoped_ptr<ExtensionInstallPrompt> CreateInstallUI();

  
  virtual scoped_ptr<WebstoreInstaller::Approval> CreateApproval() const;

  
  bool show_user_count() const { return show_user_count_; }
  const std::string& localized_user_count() const {
    return localized_user_count_;
  }
  double average_rating() const { return average_rating_; }
  int rating_count() const { return rating_count_; }
  void set_install_source(WebstoreInstaller::InstallSource source) {
    install_source_ = source;
  }
  WebstoreInstaller::InstallSource install_source() const {
    return install_source_;
  }
  Profile* profile() const { return profile_; }
  const std::string& id() const { return id_; }
  const DictionaryValue* manifest() const { return manifest_.get(); }

 private:
  friend class base::RefCountedThreadSafe<WebstoreStandaloneInstaller>;
  FRIEND_TEST_ALL_PREFIXES(WebstoreStandaloneInstallerTest, DomainVerification);

  
  
  
  
  
  
  
  
  
  
  
  

  
  virtual void OnWebstoreRequestFailure() OVERRIDE;

  virtual void OnWebstoreResponseParseSuccess(
      scoped_ptr<base::DictionaryValue> webstore_data) OVERRIDE;

  virtual void OnWebstoreResponseParseFailure(
      const std::string& error) OVERRIDE;

  
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

  void ShowInstallUI();

  
  std::string id_;
  Callback callback_;
  Profile* profile_;
  WebstoreInstaller::InstallSource install_source_;

  
  scoped_ptr<ExtensionInstallPrompt> install_ui_;
  scoped_ptr<ExtensionInstallPrompt::Prompt> install_prompt_;

  
  scoped_ptr<WebstoreDataFetcher> webstore_data_fetcher_;

  
  std::string localized_name_;
  std::string localized_description_;
  bool show_user_count_;
  std::string localized_user_count_;
  double average_rating_;
  int rating_count_;
  scoped_ptr<DictionaryValue> webstore_data_;
  scoped_ptr<DictionaryValue> manifest_;
  SkBitmap icon_;

  
  
  scoped_refptr<Extension> localized_extension_for_display_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(WebstoreStandaloneInstaller);
};

}  

#endif  
