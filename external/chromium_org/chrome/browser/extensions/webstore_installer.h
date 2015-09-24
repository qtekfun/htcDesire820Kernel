// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_WEBSTORE_INSTALLER_H_
#define CHROME_BROWSER_EXTENSIONS_WEBSTORE_INSTALLER_H_

#include <list>
#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/supports_user_data.h"
#include "base/values.h"
#include "base/version.h"
#include "chrome/browser/extensions/extension_install_prompt.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/common/manifest_handlers/shared_module_info.h"
#include "net/base/net_errors.h"
#include "ui/gfx/image/image_skia.h"
#include "url/gurl.h"

class Profile;

namespace base {
class FilePath;
}

namespace content {
class NavigationController;
}

namespace extensions {

class Extension;
class Manifest;

class WebstoreInstaller :public content::NotificationObserver,
                         public content::DownloadItem::Observer,
                         public base::RefCountedThreadSafe<
  WebstoreInstaller, content::BrowserThread::DeleteOnUIThread> {
 public:
  enum InstallSource {
    
    
    INSTALL_SOURCE_INLINE,
    INSTALL_SOURCE_APP_LAUNCHER,
    INSTALL_SOURCE_OTHER
  };

  enum FailureReason {
    FAILURE_REASON_CANCELLED,
    FAILURE_REASON_DEPENDENCY_NOT_FOUND,
    FAILURE_REASON_DEPENDENCY_NOT_SHARED_MODULE,
    FAILURE_REASON_OTHER
  };

  enum ManifestCheckLevel {
    
    MANIFEST_CHECK_LEVEL_NONE,

    
    
    MANIFEST_CHECK_LEVEL_LOOSE,

    
    MANIFEST_CHECK_LEVEL_STRICT,
  };

  class Delegate {
   public:
    virtual void OnExtensionDownloadStarted(const std::string& id,
                                            content::DownloadItem* item);
    virtual void OnExtensionDownloadProgress(const std::string& id,
                                             content::DownloadItem* item);
    virtual void OnExtensionInstallSuccess(const std::string& id) = 0;
    virtual void OnExtensionInstallFailure(const std::string& id,
                                           const std::string& error,
                                           FailureReason reason) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  
  
  struct Approval : public base::SupportsUserData::Data {
    static scoped_ptr<Approval> CreateWithInstallPrompt(Profile* profile);

    
    static scoped_ptr<Approval> CreateForSharedModule(Profile* profile);

    
    
    
    
    
    static scoped_ptr<Approval> CreateWithNoInstallPrompt(
        Profile* profile,
        const std::string& extension_id,
        scoped_ptr<base::DictionaryValue> parsed_manifest,
        bool strict_manifest_check);

    virtual ~Approval();

    
    std::string extension_id;

    
    Profile* profile;

    
    scoped_ptr<Manifest> manifest;

    
    
    bool use_app_installed_bubble;

    
    bool skip_post_install_ui;

    
    
    
    
    bool skip_install_dialog;

    
    bool enable_launcher;

    
    
    ManifestCheckLevel manifest_check_level;

    
    ExtensionInstallPrompt::ShowDialogCallback show_dialog_callback;

    
    gfx::ImageSkia installing_icon;

    
    scoped_refptr<Extension> dummy_extension;

    
    scoped_ptr<Version> minimum_version;

    
    bool is_ephemeral;

   private:
    Approval();
  };

  
  
  static const Approval* GetAssociatedApproval(
      const content::DownloadItem& download);

  
  
  
  
  
  
  
  WebstoreInstaller(Profile* profile,
                    Delegate* delegate,
                    content::NavigationController* controller,
                    const std::string& id,
                    scoped_ptr<Approval> approval,
                    InstallSource source);

  
  void Start();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void InvalidateDelegate();

  
  
  static void SetDownloadDirectoryForTests(base::FilePath* directory);

 private:
  FRIEND_TEST_ALL_PREFIXES(WebstoreInstallerTest, PlatformParams);
  friend struct content::BrowserThread::DeleteOnThread<
   content::BrowserThread::UI>;
  friend class base::DeleteHelper<WebstoreInstaller>;
  virtual ~WebstoreInstaller();

  
  static GURL GetWebstoreInstallURL(const std::string& extension_id,
                                    InstallSource source);

  
  void OnDownloadStarted(content::DownloadItem* item, net::Error error);

  
  virtual void OnDownloadUpdated(content::DownloadItem* download) OVERRIDE;
  virtual void OnDownloadDestroyed(content::DownloadItem* download) OVERRIDE;

  
  void DownloadNextPendingModule();

  
  
  void DownloadCrx(const std::string& extension_id, InstallSource source);

  
  void StartDownload(const base::FilePath& file_path);

  
  
  void ReportFailure(const std::string& error, FailureReason reason);

  
  
  
  void ReportSuccess();

  
  void RecordInterrupt(const content::DownloadItem* download) const;

  content::NotificationRegistrar registrar_;
  Profile* profile_;
  Delegate* delegate_;
  content::NavigationController* controller_;
  std::string id_;
  InstallSource install_source_;
  
  
  content::DownloadItem* download_item_;
  scoped_ptr<Approval> approval_;
  GURL download_url_;

  
  std::list<SharedModuleInfo::ImportInfo> pending_modules_;
  
  
  int total_modules_;
  bool download_started_;
};

}  

#endif  
