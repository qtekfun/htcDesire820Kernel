// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_COMPLETION_BLOCKER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_COMPLETION_BLOCKER_H_

#include "base/callback.h"
#include "base/supports_user_data.h"
#include "content/public/browser/download_item.h"

class DownloadCompletionBlocker : public base::SupportsUserData::Data {
 public:
  DownloadCompletionBlocker();
  virtual ~DownloadCompletionBlocker();

  bool is_complete() const { return is_complete_; }

  void set_callback(const base::Closure& callback) {
    if (!is_complete())
      callback_ = callback;
  }

  
  
  
  
  virtual void CompleteDownload();

 private:
  bool is_complete_;
  base::Closure callback_;

  DISALLOW_COPY_AND_ASSIGN(DownloadCompletionBlocker);
};

#endif  
