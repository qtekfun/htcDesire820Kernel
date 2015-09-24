// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_QUOTA_QUOTA_RESERVATION_H_
#define WEBKIT_BROWSER_FILEAPI_QUOTA_QUOTA_RESERVATION_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "webkit/browser/fileapi/quota/quota_reservation_manager.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/fileapi/file_system_types.h"

class GURL;

namespace fileapi {

class QuotaReservationBuffer;
class OpenFileHandle;

class WEBKIT_STORAGE_BROWSER_EXPORT QuotaReservation
    : public base::RefCounted<QuotaReservation> {
 public:
  typedef base::Callback<void(base::PlatformFileError error)> StatusCallback;

  
  
  
  void RefreshReservation(int64 size, const StatusCallback& callback);

  
  
  scoped_ptr<OpenFileHandle> GetOpenFileHandle(
      const base::FilePath& platform_path);

  
  
  
  
  
  
  void OnClientCrash();

  
  
  void ConsumeReservation(int64 size);

  
  int64 remaining_quota() const { return remaining_quota_; }

  QuotaReservationManager* reservation_manager();
  const GURL& origin() const;
  FileSystemType type() const;

 private:
  friend class QuotaReservationBuffer;

  
  explicit QuotaReservation(QuotaReservationBuffer* reservation_buffer);

  friend class base::RefCounted<QuotaReservation>;
  virtual ~QuotaReservation();

  static bool AdaptDidUpdateReservedQuota(
      const base::WeakPtr<QuotaReservation>& reservation,
      int64 new_reserved_size,
      const StatusCallback& callback,
      base::PlatformFileError error);
  void DidUpdateReservedQuota(int64 new_reserved_size,
                              const StatusCallback& callback,
                              base::PlatformFileError error);

  bool running_refresh_request_;
  int64 remaining_quota_;

  scoped_refptr<QuotaReservationBuffer> reservation_buffer_;

  base::SequenceChecker sequence_checker_;
  base::WeakPtrFactory<QuotaReservation> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(QuotaReservation);
};

}  

#endif  
