// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_SERVICE_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_SERVICE_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class ChromeDownloadManagerDelegate;
class DownloadHistory;
class DownloadUIController;
class ExtensionDownloadsEventRouter;
class Profile;

namespace content {
class DownloadManager;
}

class DownloadService : public BrowserContextKeyedService {
 public:
  explicit DownloadService(Profile* profile);
  virtual ~DownloadService();

  
  ChromeDownloadManagerDelegate* GetDownloadManagerDelegate();

  
  
  
  DownloadHistory* GetDownloadHistory();

#if !defined(OS_ANDROID)
  ExtensionDownloadsEventRouter* GetExtensionEventRouter() {
    return extension_event_router_.get();
  }
#endif

  
  bool HasCreatedDownloadManager();

  
  
  int NonMaliciousDownloadCount() const;

  
  static int NonMaliciousDownloadCountAllProfiles();

  
  static void CancelAllDownloads();

  
  
  
  void SetDownloadManagerDelegateForTesting(
      scoped_ptr<ChromeDownloadManagerDelegate> delegate);

  
  
  virtual void Shutdown() OVERRIDE;

  
  
  bool IsShelfEnabled();

 private:
  bool download_manager_created_;
  Profile* profile_;

  
  
  
  scoped_ptr<ChromeDownloadManagerDelegate> manager_delegate_;

  scoped_ptr<DownloadHistory> download_history_;

  
  
  
  scoped_ptr<DownloadUIController> download_ui_;

  
  
  
  
#if !defined(OS_ANDROID)
  
  
  
  
  
  
  scoped_ptr<ExtensionDownloadsEventRouter> extension_event_router_;
#endif

  DISALLOW_COPY_AND_ASSIGN(DownloadService);
};

#endif  
