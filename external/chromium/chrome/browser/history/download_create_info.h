// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_DOWNLOAD_CREATE_INFO_H_
#define CHROME_BROWSER_HISTORY_DOWNLOAD_CREATE_INFO_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/time.h"
#include "chrome/browser/download/download_file.h"
#include "googleurl/src/gurl.h"

struct DownloadCreateInfo {
  DownloadCreateInfo(const FilePath& path,
                     const GURL& url,
                     base::Time start_time,
                     int64 received_bytes,
                     int64 total_bytes,
                     int32 state,
                     int32 download_id,
                     bool has_user_gesture);
  DownloadCreateInfo();
  ~DownloadCreateInfo();

  
  bool IsDangerous();

  std::string DebugString() const;

  
  
  const GURL& url() const;

  
  FilePath path;
  
  std::vector<GURL> url_chain;
  GURL referrer_url;
  FilePath suggested_path;
  
  
  int path_uniquifier;
  base::Time start_time;
  int64 received_bytes;
  int64 total_bytes;
  int32 state;
  int32 download_id;
  bool has_user_gesture;
  int child_id;
  int render_view_id;
  int request_id;
  int64 db_handle;
  std::string content_disposition;
  std::string mime_type;
  
  
  
  std::string original_mime_type;

  
  
  
  
  bool prompt_user_for_save_location;
  
  bool is_dangerous_file;
  
  bool is_dangerous_url;
  
  FilePath original_name;
  
  bool is_extension_install;
  
  
  std::string referrer_charset;
  
  DownloadSaveInfo save_info;
};

#endif  
