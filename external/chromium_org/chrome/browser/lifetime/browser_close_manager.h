// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_LIFETIME_BROWSER_CLOSE_MANAGER_H_
#define CHROME_BROWSER_LIFETIME_BROWSER_CLOSE_MANAGER_H_

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"

class Browser;

class BrowserCloseManager : public base::RefCounted<BrowserCloseManager> {
 public:
  BrowserCloseManager();

  
  void StartClosingBrowsers();

 protected:
  friend class base::RefCounted<BrowserCloseManager>;

  virtual ~BrowserCloseManager();

  virtual void ConfirmCloseWithPendingDownloads(
      int download_count,
      const base::Callback<void(bool)>& callback);

 private:
  
  void CancelBrowserClose();

  
  
  void TryToCloseBrowsers();

  
  void OnBrowserReportCloseable(bool proceed);

  
  void CloseBrowsers();

  
  
  
  
  
  
  void CheckForDownloadsInProgress();

  
  void OnReportDownloadsCancellable(bool proceed);

  
  
  Browser* current_browser_;

  DISALLOW_COPY_AND_ASSIGN(BrowserCloseManager);
};

#endif  
