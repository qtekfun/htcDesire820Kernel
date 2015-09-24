// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DOWNLOAD_ITEM_H_
#define CONTENT_PUBLIC_BROWSER_DOWNLOAD_ITEM_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "base/supports_user_data.h"
#include "content/public/browser/download_danger_type.h"
#include "content/public/browser/download_interrupt_reasons.h"
#include "content/public/common/page_transition_types.h"

class GURL;

namespace base {
class FilePath;
class Time;
class TimeDelta;
}

namespace content {

class BrowserContext;
class DownloadManager;
class WebContents;

class CONTENT_EXPORT DownloadItem : public base::SupportsUserData {
 public:
  enum DownloadState {
    
    IN_PROGRESS = 0,

    
    COMPLETE,

    
    CANCELLED,

    
    INTERRUPTED,

    
    MAX_DOWNLOAD_STATE
  };

  
  enum TargetDisposition {
    TARGET_DISPOSITION_OVERWRITE, 
    TARGET_DISPOSITION_PROMPT     
                                  
                                  
  };

  
  typedef base::Callback<void(const base::FilePath&)> AcquireFileCallback;

  static const uint32 kInvalidId;

  static const char kEmptyFileHash[];

  
  
  class CONTENT_EXPORT Observer {
   public:
    virtual void OnDownloadUpdated(DownloadItem* download) {}
    virtual void OnDownloadOpened(DownloadItem* download) {}
    virtual void OnDownloadRemoved(DownloadItem* download) {}

    
    
    
    virtual void OnDownloadDestroyed(DownloadItem* download) {}

   protected:
    virtual ~Observer() {}
  };

  virtual ~DownloadItem() {}

  

  virtual void AddObserver(DownloadItem::Observer* observer) = 0;
  virtual void RemoveObserver(DownloadItem::Observer* observer) = 0;
  virtual void UpdateObservers() = 0;

  

  
  virtual void ValidateDangerousDownload() = 0;

  
  
  
  
  
  virtual void StealDangerousDownload(const AcquireFileCallback& callback) = 0;

  
  
  virtual void Pause() = 0;

  
  
  virtual void Resume() = 0;

  
  
  
  
  
  
  
  
  
  virtual void Cancel(bool user_cancel) = 0;

  
  
  
  virtual void Remove() = 0;

  
  
  virtual void OpenDownload() = 0;

  
  virtual void ShowDownloadInShell() = 0;

  

  virtual uint32 GetId() const = 0;
  virtual DownloadState GetState() const = 0;

  
  
  
  
  
  virtual DownloadInterruptReason GetLastReason() const = 0;

  virtual bool IsPaused() const = 0;
  virtual bool IsTemporary() const = 0;

  
  
  
  virtual bool CanResume() const = 0;

  
  
  
  virtual bool IsDone() const = 0;

  

  virtual const GURL& GetURL() const = 0;
  virtual const std::vector<GURL>& GetUrlChain() const = 0;
  virtual const GURL& GetOriginalUrl() const = 0;
  virtual const GURL& GetReferrerUrl() const = 0;
  virtual std::string GetSuggestedFilename() const = 0;
  virtual std::string GetContentDisposition() const = 0;
  virtual std::string GetMimeType() const = 0;
  virtual std::string GetOriginalMimeType() const = 0;
  virtual std::string GetRemoteAddress() const = 0;
  virtual bool HasUserGesture() const = 0;
  virtual PageTransition GetTransitionType() const = 0;
  virtual const std::string& GetLastModifiedTime() const = 0;
  virtual const std::string& GetETag() const = 0;
  virtual bool IsSavePackageDownload() const = 0;

  

  
  
  
  
  
  
  
  
  
  
  
  virtual const base::FilePath& GetFullPath() const = 0;

  
  
  
  
  virtual const base::FilePath& GetTargetFilePath() const = 0;

  
  
  virtual const base::FilePath& GetForcedFilePath() const = 0;

  
  
  
  virtual base::FilePath GetFileNameToReportUser() const = 0;

  virtual TargetDisposition GetTargetDisposition() const = 0;

  
  
  virtual const std::string& GetHash() const = 0;

  
  virtual const std::string& GetHashState() const = 0;

  
  
  virtual bool GetFileExternallyRemoved() const = 0;

  
  
  
  
  virtual void DeleteFile() = 0;

  // True if the file that will be written by the download is dangerous
  
  
  virtual bool IsDangerous() const = 0;

  
  virtual DownloadDangerType GetDangerType() const = 0;

  

  
  
  
  
  virtual bool TimeRemaining(base::TimeDelta* remaining) const = 0;

  
  virtual int64 CurrentSpeed() const = 0;

  
  
  virtual int PercentComplete() const = 0;

  
  virtual bool AllDataSaved() const = 0;

  virtual int64 GetTotalBytes() const = 0;
  virtual int64 GetReceivedBytes() const = 0;
  virtual base::Time GetStartTime() const = 0;
  virtual base::Time GetEndTime() const = 0;

  

  
  virtual bool CanShowInFolder() = 0;

  
  virtual bool CanOpenDownload() = 0;

  
  virtual bool ShouldOpenFileBasedOnExtension() = 0;

  
  virtual bool GetOpenWhenComplete() const = 0;

  
  virtual bool GetAutoOpened() = 0;

  
  virtual bool GetOpened() const = 0;

  

  virtual BrowserContext* GetBrowserContext() const = 0;
  virtual WebContents* GetWebContents() const = 0;

  
  
  

  
  
  
  virtual void OnContentCheckCompleted(DownloadDangerType danger_type) = 0;

  
  virtual void SetOpenWhenComplete(bool open) = 0;

  
  
  virtual void SetIsTemporary(bool temporary) = 0;

  
  virtual void SetOpened(bool opened) = 0;

  
  
  
  virtual void SetDisplayName(const base::FilePath& name) = 0;

  
  virtual std::string DebugString(bool verbose) const = 0;
};

}  

#endif  
