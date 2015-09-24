// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_CREATE_INFO_H_
#define CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_CREATE_INFO_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/time/time.h"
#include "content/browser/download/download_file.h"
#include "content/browser/download/download_request_handle.h"
#include "content/common/content_export.h"
#include "content/public/browser/download_save_info.h"
#include "content/public/common/page_transition_types.h"
#include "net/base/net_log.h"
#include "url/gurl.h"

namespace content {

struct CONTENT_EXPORT DownloadCreateInfo {
  DownloadCreateInfo(const base::Time& start_time,
                     int64 total_bytes,
                     const net::BoundNetLog& bound_net_log,
                     bool has_user_gesture,
                     PageTransition transition_type,
                     scoped_ptr<DownloadSaveInfo> save_info);
  DownloadCreateInfo();
  ~DownloadCreateInfo();

  std::string DebugString() const;

  
  
  const GURL& url() const;

  
  std::vector<GURL> url_chain;

  
  GURL referrer_url;

  
  base::Time start_time;

  
  int64 total_bytes;

  
  uint32 download_id;

  
  bool has_user_gesture;

  PageTransition transition_type;

  
  std::string content_disposition;

  
  std::string mime_type;

  
  
  
  std::string original_mime_type;

  
  
  
  std::string last_modified;

  
  std::string etag;

  
  scoped_ptr<DownloadSaveInfo> save_info;

  
  
  std::string remote_address;

  
  DownloadRequestHandle request_handle;

  
  
  const net::BoundNetLog request_bound_net_log;

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadCreateInfo);
};

}  

#endif  
