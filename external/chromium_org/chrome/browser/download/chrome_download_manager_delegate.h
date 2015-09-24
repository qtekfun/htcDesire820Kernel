// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_CHROME_DOWNLOAD_MANAGER_DELEGATE_H_
#define CHROME_BROWSER_DOWNLOAD_CHROME_DOWNLOAD_MANAGER_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/download/download_path_reservation_tracker.h"
#include "chrome/browser/download/download_target_determiner_delegate.h"
#include "chrome/browser/download/download_target_info.h"
#include "chrome/browser/safe_browsing/download_protection_service.h"
#include "content/public/browser/download_danger_type.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_manager_delegate.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class DownloadPrefs;
class Profile;

namespace content {
class DownloadManager;
}

namespace extensions {
class CrxInstaller;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

#if defined(COMPILER_GCC)
namespace BASE_HASH_NAMESPACE {
template<>
struct hash<extensions::CrxInstaller*> {
  std::size_t operator()(extensions::CrxInstaller* const& p) const {
    return reinterpret_cast<std::size_t>(p);
  }
};
}  
#endif

class ChromeDownloadManagerDelegate
    : public content::DownloadManagerDelegate,
      public content::NotificationObserver,
      public DownloadTargetDeterminerDelegate {
 public:
  explicit ChromeDownloadManagerDelegate(Profile* profile);
  virtual ~ChromeDownloadManagerDelegate();

  
  
  static void DisableSafeBrowsing(content::DownloadItem* item);

  void SetDownloadManager(content::DownloadManager* dm);

  
  
  content::DownloadIdCallback GetDownloadIdReceiverCallback();

  
  virtual void Shutdown() OVERRIDE;
  virtual void GetNextId(const content::DownloadIdCallback& callback) OVERRIDE;
  virtual bool DetermineDownloadTarget(
      content::DownloadItem* item,
      const content::DownloadTargetCallback& callback) OVERRIDE;
  virtual bool ShouldOpenFileBasedOnExtension(
      const base::FilePath& path) OVERRIDE;
  virtual bool ShouldCompleteDownload(
      content::DownloadItem* item,
      const base::Closure& complete_callback) OVERRIDE;
  virtual bool ShouldOpenDownload(
      content::DownloadItem* item,
      const content::DownloadOpenDelayedCallback& callback) OVERRIDE;
  virtual bool GenerateFileHash() OVERRIDE;
  virtual void GetSaveDir(content::BrowserContext* browser_context,
                          base::FilePath* website_save_dir,
                          base::FilePath* download_save_dir,
                          bool* skip_dir_check) OVERRIDE;
  virtual void ChooseSavePath(
      content::WebContents* web_contents,
      const base::FilePath& suggested_path,
      const base::FilePath::StringType& default_extension,
      bool can_save_as_complete,
      const content::SavePackagePathPickedCallback& callback) OVERRIDE;
  virtual void OpenDownload(content::DownloadItem* download) OVERRIDE;
  virtual void ShowDownloadInShell(content::DownloadItem* download) OVERRIDE;
  virtual void CheckForFileExistence(
      content::DownloadItem* download,
      const content::CheckForFileExistenceCallback& callback) OVERRIDE;
  virtual std::string ApplicationClientIdForFileScanning() const OVERRIDE;

  
  
  
  void OpenDownloadUsingPlatformHandler(content::DownloadItem* download);

  DownloadPrefs* download_prefs() { return download_prefs_.get(); }

 protected:
  
  
  content::DownloadManager* download_manager_;

  virtual safe_browsing::DownloadProtectionService*
      GetDownloadProtectionService();

  
  virtual void NotifyExtensions(
      content::DownloadItem* download,
      const base::FilePath& suggested_virtual_path,
      const NotifyExtensionsCallback& callback) OVERRIDE;
  virtual void ReserveVirtualPath(
      content::DownloadItem* download,
      const base::FilePath& virtual_path,
      bool create_directory,
      DownloadPathReservationTracker::FilenameConflictAction conflict_action,
      const ReservedPathCallback& callback) OVERRIDE;
  virtual void PromptUserForDownloadPath(
      content::DownloadItem* download,
      const base::FilePath& suggested_virtual_path,
      const FileSelectedCallback& callback) OVERRIDE;
  virtual void DetermineLocalPath(
      content::DownloadItem* download,
      const base::FilePath& virtual_path,
      const LocalPathCallback& callback) OVERRIDE;
  virtual void CheckDownloadUrl(
      content::DownloadItem* download,
      const base::FilePath& suggested_virtual_path,
      const CheckDownloadUrlCallback& callback) OVERRIDE;
  virtual void GetFileMimeType(
      const base::FilePath& path,
      const GetFileMimeTypeCallback& callback) OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<ChromeDownloadManagerDelegate>;

  typedef std::vector<content::DownloadIdCallback> IdCallbackVector;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void CheckClientDownloadDone(
      uint32 download_id,
      safe_browsing::DownloadProtectionService::DownloadCheckResult result);

  
  bool IsDownloadReadyForCompletion(
      content::DownloadItem* item,
      const base::Closure& internal_complete_callback);
  void ShouldCompleteDownloadInternal(
    uint32 download_id,
    const base::Closure& user_complete_callback);

  void SetNextId(uint32 id);

  void ReturnNextId(const content::DownloadIdCallback& callback);

  void OnDownloadTargetDetermined(
      int32 download_id,
      const content::DownloadTargetCallback& callback,
      scoped_ptr<DownloadTargetInfo> target_info);

  Profile* profile_;
  uint32 next_download_id_;
  IdCallbackVector id_callbacks_;
  scoped_ptr<DownloadPrefs> download_prefs_;

  
  typedef base::hash_map<extensions::CrxInstaller*,
      content::DownloadOpenDelayedCallback> CrxInstallerMap;
  CrxInstallerMap crx_installers_;

  content::NotificationRegistrar registrar_;

  base::WeakPtrFactory<ChromeDownloadManagerDelegate> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ChromeDownloadManagerDelegate);
};

#endif  
