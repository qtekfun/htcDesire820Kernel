// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_ANDROID_DOWNLOAD_CONTROLLER_ANDROID_H_
#define CONTENT_PUBLIC_BROWSER_ANDROID_DOWNLOAD_CONTROLLER_ANDROID_H_

#include "content/common/content_export.h"
#include "content/public/common/context_menu_params.h"

namespace content {
class DownloadItem;
class WebContents;

class CONTENT_EXPORT DownloadControllerAndroid {
 public:
  
  static DownloadControllerAndroid* Get();

  
  
  virtual void CreateGETDownload(int render_process_id, int render_view_id,
                                 int request_id) = 0;

  
  
  
  virtual void OnDownloadStarted(DownloadItem* download_item) = 0;

  
  virtual void StartContextMenuDownload(
      const ContextMenuParams& params, WebContents* web_contents,
      bool is_link) = 0;

  
  virtual void DangerousDownloadValidated(
      WebContents* web_contents, int download_id, bool accept) = 0;

 protected:
  virtual ~DownloadControllerAndroid() {};
};

}  

#endif  
