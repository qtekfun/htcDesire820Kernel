// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_PEPPER_QUOTA_RESERVATION_H_
#define CONTENT_BROWSER_RENDERER_HOST_PEPPER_QUOTA_RESERVATION_H_

#include <map>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/platform_file.h"
#include "content/common/content_export.h"
#include "ppapi/c/pp_stdint.h"  
#include "url/gurl.h"
#include "webkit/browser/fileapi/file_system_context.h"

namespace fileapi {
class FileSystemURL;
class OpenFileHandle;
class QuotaReservation;
}

namespace content {

struct QuotaReservationDeleter;

class CONTENT_EXPORT QuotaReservation
    : public base::RefCountedThreadSafe<QuotaReservation,
                                        QuotaReservationDeleter> {
 public:
  
  static scoped_refptr<QuotaReservation> Create(
      scoped_refptr<fileapi::FileSystemContext> file_system_context,
      const GURL& origin_url,
      fileapi::FileSystemType file_system_type);

  
  int64_t OpenFile(int32_t id, const fileapi::FileSystemURL& url);
  
  void CloseFile(int32_t id, int64_t max_written_offset);
  
  // ids with maximum written offsets is provided as input. The callback will
  
  typedef std::map<int32_t, int64_t> OffsetMap;
  typedef base::Callback<void(int64_t, const OffsetMap&)> ReserveQuotaCallback;
  void ReserveQuota(int64_t amount,
                    const OffsetMap& max_written_offsets,
                    const ReserveQuotaCallback& callback);
 private:
  friend class base::RefCountedThreadSafe<QuotaReservation,
                                          QuotaReservationDeleter>;
  friend class base::DeleteHelper<QuotaReservation>;
  friend struct QuotaReservationDeleter;
  friend class QuotaReservationTest;

  QuotaReservation(
      scoped_refptr<fileapi::FileSystemContext> file_system_context,
      const GURL& origin_url,
      fileapi::FileSystemType file_system_type);

  
  
  QuotaReservation(
      scoped_refptr<fileapi::QuotaReservation> quota_reservation,
      const GURL& origin_url,
      fileapi::FileSystemType file_system_type);

  ~QuotaReservation();

  void GotReservedQuota(const ReserveQuotaCallback& callback,
                        base::PlatformFileError error);

  void DeleteOnCorrectThread() const;

  scoped_refptr<fileapi::FileSystemContext> file_system_context_;
  scoped_refptr<fileapi::QuotaReservation> quota_reservation_;
  typedef std::map<int32_t, fileapi::OpenFileHandle*> FileMap;
  FileMap files_;

  DISALLOW_COPY_AND_ASSIGN(QuotaReservation);
};

struct QuotaReservationDeleter {
  static void Destruct(const QuotaReservation* quota_reservation) {
    quota_reservation->DeleteOnCorrectThread();
  }
};

}  

#endif  
