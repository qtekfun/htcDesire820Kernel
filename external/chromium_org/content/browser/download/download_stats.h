// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_STATS_H_
#define CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_STATS_H_

#include <string>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/public/browser/download_danger_type.h"
#include "content/public/browser/download_interrupt_reasons.h"

namespace base {
class FilePath;
class Time;
class TimeDelta;
class TimeTicks;
}

namespace content {

enum DownloadCountTypes {
  
  
  DOWNLOAD_COUNT_UNUSED_0 = 0,
  DOWNLOAD_COUNT_UNUSED_1,
  DOWNLOAD_COUNT_UNUSED_2,
  DOWNLOAD_COUNT_UNUSED_3,
  DOWNLOAD_COUNT_UNUSED_4,

  
  UNTHROTTLED_COUNT,

  
  COMPLETED_COUNT,

  
  CANCELLED_COUNT,

  
  START_COUNT,

  
  INTERRUPTED_COUNT,

  
  
  DOWNLOAD_COUNT_UNUSED_10,

  
  
  
  DOWNLOAD_COUNT_UNUSED_11,

  
  INTERRUPTED_AT_END_COUNT,

  
  
  
  
  
  APPEND_TO_DETACHED_FILE_COUNT,

  
  
  FILE_MISSING_AFTER_SUCCESSFUL_SCAN_COUNT,

  
  
  DOWNLOAD_COUNT_UNUSED_15,

  
  
  INTERRUPTED_WITHOUT_WEBCONTENTS,

  
  
  
  STRONG_VALIDATOR_AND_ACCEPTS_RANGES,

  DOWNLOAD_COUNT_TYPES_LAST_ENTRY
};

enum DownloadSource {
  
  
  
  INITIATED_BY_SAVE_PACKAGE_ON_NON_HTML = 0,

  
  
  INITIATED_BY_DRAG_N_DROP,

  
  
  INITIATED_BY_RENDERER,

  
  DOWNLOAD_SOURCE_UNUSED_3,

  
  
  INITIATED_BY_RESUMPTION,

  DOWNLOAD_SOURCE_LAST_ENTRY
};

enum DownloadDiscardReason {
  
  DOWNLOAD_DISCARD_DUE_TO_USER_ACTION,

  
  DOWNLOAD_DISCARD_DUE_TO_SHUTDOWN
};

void RecordDownloadCount(DownloadCountTypes type);

void RecordDownloadSource(DownloadSource source);

void RecordDownloadCompleted(const base::TimeTicks& start, int64 download_len);

void RecordDownloadInterrupted(DownloadInterruptReason reason,
                               int64 received,
                               int64 total);

void RecordMaliciousDownloadClassified(DownloadDangerType danger_type);

void RecordDangerousDownloadAccept(
    DownloadDangerType danger_type,
    const base::FilePath& file_path);

void RecordDangerousDownloadDiscard(
    DownloadDiscardReason reason,
    DownloadDangerType danger_type,
    const base::FilePath& file_path);

void RecordDownloadMimeType(const std::string& mime_type);

void RecordDownloadContentDisposition(const std::string& content_disposition);

void RecordDownloadWriteSize(size_t data_len);

void RecordDownloadWriteLoopCount(int count);

void RecordFileThreadReceiveBuffers(size_t num_buffers);

void RecordBandwidth(double actual_bandwidth, double potential_bandwidth);

void RecordOpen(const base::Time& end, bool first);

void RecordAcceptsRanges(const std::string& accepts_ranges,
                         int64 download_len,
                         bool has_strong_validator);

void RecordClearAllSize(int size);

void RecordOpensOutstanding(int size);

void RecordContiguousWriteTime(base::TimeDelta time_blocked);

void RecordNetworkBlockage(base::TimeDelta resource_handler_lifetime,
                           base::TimeDelta resource_handler_blocked_time);

void RecordFileBandwidth(size_t length,
                         base::TimeDelta disk_write_time,
                         base::TimeDelta elapsed_time);

enum SavePackageEvent {
  
  SAVE_PACKAGE_STARTED,

  
  SAVE_PACKAGE_CANCELLED,

  
  SAVE_PACKAGE_FINISHED,

  
  SAVE_PACKAGE_WRITE_TO_COMPLETED,

  
  SAVE_PACKAGE_WRITE_TO_FAILED,

  SAVE_PACKAGE_LAST_ENTRY
};

void RecordSavePackageEvent(SavePackageEvent event);

enum OriginStateOnResumption {
  ORIGIN_STATE_ON_RESUMPTION_ADDITIONAL_REDIRECTS = 1<<0,
  ORIGIN_STATE_ON_RESUMPTION_VALIDATORS_CHANGED = 1<<1,
  ORIGIN_STATE_ON_RESUMPTION_CONTENT_DISPOSITION_CHANGED = 1<<2,
  ORIGIN_STATE_ON_RESUMPTION_MAX = 1<<3
};

void RecordOriginStateOnResumption(bool is_partial,
                                   int state);

}  

#endif  
