// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_STATUS_UPDATER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_STATUS_UPDATER_H_

#include <set>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"

class DownloadStatusUpdaterDelegate;

class DownloadStatusUpdater
    : public base::SupportsWeakPtr<DownloadStatusUpdater> {
 public:
  DownloadStatusUpdater();
  ~DownloadStatusUpdater();

  void AddDelegate(DownloadStatusUpdaterDelegate* delegate);
  void RemoveDelegate(DownloadStatusUpdaterDelegate* delegate);

  
  void Update();

 private:
  FRIEND_TEST_ALL_PREFIXES(DownloadStatusUpdaterTest, Basic);
  FRIEND_TEST_ALL_PREFIXES(DownloadStatusUpdaterTest, OneDelegate);
  FRIEND_TEST_ALL_PREFIXES(DownloadStatusUpdaterTest, MultipleDelegates);

  
  
  bool GetProgress(float* progress);

  
  int64 GetInProgressDownloadCount();

  typedef std::set<DownloadStatusUpdaterDelegate*> DelegateSet;
  DelegateSet delegates_;

  DISALLOW_COPY_AND_ASSIGN(DownloadStatusUpdater);
};

#endif  
