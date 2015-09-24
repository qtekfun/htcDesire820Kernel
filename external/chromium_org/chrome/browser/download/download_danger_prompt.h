// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_DANGER_PROMPT_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_DANGER_PROMPT_H_

#include "base/callback_forward.h"

namespace content {
class DownloadItem;
class WebContents;
}

class DownloadDangerPrompt {
 public:
  
  enum Action {
    ACCEPT,
    CANCEL,
    DISMISS,
  };
  typedef base::Callback<void(Action)> OnDone;

  
  
  
  
  
  
  
  
  static DownloadDangerPrompt* Create(
      content::DownloadItem* item,
      content::WebContents* web_contents,
      bool show_context,
      const OnDone& done);

  
  
  virtual void InvokeActionForTesting(Action action) = 0;
};

#endif  
