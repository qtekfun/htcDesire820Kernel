// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_REQUEST_HANDLE_H_
#define CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_REQUEST_HANDLE_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "content/browser/download/download_resource_handler.h"
#include "content/common/content_export.h"

namespace content {
class DownloadManager;
class WebContents;

class CONTENT_EXPORT DownloadRequestHandleInterface {
 public:
  virtual ~DownloadRequestHandleInterface() {}

  
  virtual WebContents* GetWebContents() const = 0;
  virtual DownloadManager* GetDownloadManager() const = 0;

  
  virtual void PauseRequest() const = 0;
  virtual void ResumeRequest() const = 0;

  
  virtual void CancelRequest() const = 0;

  
  virtual std::string DebugString() const = 0;
};


class CONTENT_EXPORT DownloadRequestHandle
    : public DownloadRequestHandleInterface {
 public:
  virtual ~DownloadRequestHandle();

  
  
  
  
  
  
  
  DownloadRequestHandle();

  
  DownloadRequestHandle(const base::WeakPtr<DownloadResourceHandler>& handler,
                        int child_id,
                        int render_view_id,
                        int request_id);

  
  virtual WebContents* GetWebContents() const OVERRIDE;
  virtual DownloadManager* GetDownloadManager() const OVERRIDE;
  virtual void PauseRequest() const OVERRIDE;
  virtual void ResumeRequest() const OVERRIDE;
  virtual void CancelRequest() const OVERRIDE;
  virtual std::string DebugString() const OVERRIDE;

 private:
  base::WeakPtr<DownloadResourceHandler> handler_;

  
  int child_id_;

  
  int render_view_id_;

  
  int request_id_;
};

}  

#endif  
