// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_DOWNLOAD_ROW_H_
#define CHROME_BROWSER_HISTORY_DOWNLOAD_ROW_H_

#include <vector>

#include "base/files/file_path.h"
#include "base/time/time.h"
#include "content/public/browser/download_danger_type.h"
#include "content/public/browser/download_interrupt_reasons.h"
#include "content/public/browser/download_item.h"
#include "url/gurl.h"

namespace history {

struct DownloadRow {
  DownloadRow();
  DownloadRow(
      const base::FilePath& current_path,
      const base::FilePath& target_path,
      const std::vector<GURL>& url_chain,
      const GURL& referrer,
      const base::Time& start,
      const base::Time& end,
      const std::string& etag,
      const std::string& last_modified,
      int64 received,
      int64 total,
      content::DownloadItem::DownloadState download_state,
      content::DownloadDangerType danger_type,
      content::DownloadInterruptReason interrupt_reason,
      uint32 id,
      bool download_opened,
      const std::string& ext_id,
      const std::string& ext_name);
  ~DownloadRow();

  
  
  base::FilePath current_path;

  
  base::FilePath target_path;

  
  
  
  
  std::vector<GURL> url_chain;

  
  GURL referrer_url;

  
  base::Time start_time;

  
  base::Time end_time;

  
  std::string etag;

  
  std::string last_modified;

  
  int64 received_bytes;

  
  
  int64 total_bytes;

  
  content::DownloadItem::DownloadState state;

  
  content::DownloadDangerType danger_type;

  
  
  content::DownloadInterruptReason interrupt_reason;

  
  uint32 id;

  
  bool opened;

  
  std::string by_ext_id;
  std::string by_ext_name;
};

}  

#endif  
