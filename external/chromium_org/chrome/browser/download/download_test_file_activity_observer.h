// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_TEST_FILE_ACTIVITY_OBSERVER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_TEST_FILE_ACTIVITY_OBSERVER_H_

#include "base/memory/weak_ptr.h"

class Profile;

class DownloadTestFileActivityObserver {
 public:
  
  
  explicit DownloadTestFileActivityObserver(Profile* profile);
  ~DownloadTestFileActivityObserver();

  
  
  
  
  void EnableFileChooser(bool enable);

  
  
  bool TestAndResetDidShowFileChooser();

 private:
  class MockDownloadManagerDelegate;

  base::WeakPtr<MockDownloadManagerDelegate> test_delegate_;
};

#endif  
