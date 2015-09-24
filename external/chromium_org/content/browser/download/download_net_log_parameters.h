// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_NET_LOG_PARAMETERS_H_
#define CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_NET_LOG_PARAMETERS_H_

#include <string>

#include "content/public/browser/download_item.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"

class GURL;

namespace base {
class FilePath;
}

namespace content {

enum DownloadType {
  SRC_ACTIVE_DOWNLOAD,
  SRC_HISTORY_IMPORT,
  SRC_SAVE_PAGE_AS
};

base::Value* ItemActivatedNetLogCallback(
    const DownloadItem* download_item,
    DownloadType download_type,
    const std::string* file_name,
    net::NetLog::LogLevel log_level);

base::Value* ItemCheckedNetLogCallback(
    DownloadDangerType danger_type,
    net::NetLog::LogLevel log_level);

base::Value* ItemRenamedNetLogCallback(const base::FilePath* old_filename,
                                       const base::FilePath* new_filename,
                                       net::NetLog::LogLevel log_level);

base::Value* ItemInterruptedNetLogCallback(DownloadInterruptReason reason,
                                           int64 bytes_so_far,
                                           const std::string* hash_state,
                                           net::NetLog::LogLevel log_level);

base::Value* ItemResumingNetLogCallback(bool user_initiated,
                                        DownloadInterruptReason reason,
                                        int64 bytes_so_far,
                                        const std::string* hash_state,
                                        net::NetLog::LogLevel log_level);

base::Value* ItemCompletingNetLogCallback(int64 bytes_so_far,
                                          const std::string* final_hash,
                                          net::NetLog::LogLevel log_level);

base::Value* ItemFinishedNetLogCallback(bool auto_opened,
                                        net::NetLog::LogLevel log_level);

base::Value* ItemCanceledNetLogCallback(int64 bytes_so_far,
                                        const std::string* hash_state,
                                        net::NetLog::LogLevel log_level);

base::Value* FileOpenedNetLogCallback(const base::FilePath* file_name,
                                      int64 start_offset,
                                      net::NetLog::LogLevel log_level);

base::Value* FileStreamDrainedNetLogCallback(size_t stream_size,
                                             size_t num_buffers,
                                             net::NetLog::LogLevel log_level);

base::Value* FileRenamedNetLogCallback(const base::FilePath* old_filename,
                                       const base::FilePath* new_filename,
                                       net::NetLog::LogLevel log_level);

base::Value* FileErrorNetLogCallback(const char* operation,
                                     net::Error net_error,
                                     net::NetLog::LogLevel log_level);

base::Value* FileInterruptedNetLogCallback(const char* operation,
                                           int os_error,
                                           DownloadInterruptReason reason,
                                           net::NetLog::LogLevel log_level);

}  

#endif  
