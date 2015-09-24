// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_QUOTA_QUOTA_RESERVATION_BUFFER_H_
#define WEBKIT_BROWSER_FILEAPI_QUOTA_QUOTA_RESERVATION_BUFFER_H_

#include <map>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "url/gurl.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/fileapi/file_system_types.h"

namespace fileapi {

class QuotaReservation;
class OpenFileHandle;
class OpenFileHandleContext;
class QuotaReservationManager;

class QuotaReservationBuffer : public base::RefCounted<QuotaReservationBuffer> {
 public:
  QuotaReservationBuffer(
      base::WeakPtr<QuotaReservationManager> reservation_manager,
      const GURL& origin,
      FileSystemType type);

  scoped_refptr<QuotaReservation> CreateReservation();
  scoped_ptr<OpenFileHandle> GetOpenFileHandle(
      QuotaReservation* reservation,
      const base::FilePath& platform_path);
  void CommitFileGrowth(int64 quota_consumption, int64 usage_delta);
  void DetachOpenFileHandleContext(OpenFileHandleContext* context);
  void PutReservationToBuffer(int64 size);

  QuotaReservationManager* reservation_manager() {
    return reservation_manager_.get();
  }

  const GURL& origin() const { return origin_; }
  FileSystemType type() const { return type_; }

 private:
  friend class base::RefCounted<QuotaReservationBuffer>;
  virtual ~QuotaReservationBuffer();

  static bool DecrementDirtyCount(
      base::WeakPtr<QuotaReservationManager> reservation_manager,
      const GURL& origin,
      FileSystemType type,
      base::PlatformFileError error);

  typedef std::map<base::FilePath, OpenFileHandleContext*>
      OpenFileHandleContextByPath;

  
  
  OpenFileHandleContextByPath open_files_;

  base::WeakPtr<QuotaReservationManager> reservation_manager_;

  GURL origin_;
  fileapi::FileSystemType type_;

  int64 reserved_quota_;

  base::SequenceChecker sequence_checker_;

  DISALLOW_COPY_AND_ASSIGN(QuotaReservationBuffer);
};

}  

#endif  
