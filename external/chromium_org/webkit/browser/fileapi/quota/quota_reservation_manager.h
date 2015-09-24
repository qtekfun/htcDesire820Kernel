// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_QUOTA_QUOTA_RESERVATION_MANAGER_H_
#define WEBKIT_BROWSER_FILEAPI_QUOTA_QUOTA_RESERVATION_MANAGER_H_

#include <map>
#include <utility>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "url/gurl.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/fileapi/file_system_types.h"

namespace fileapi {

class QuotaReservation;
class QuotaReservationBuffer;
class OpenFileHandle;
class OpenFileHandleContext;

class WEBKIT_STORAGE_BROWSER_EXPORT QuotaReservationManager {
 public:
  
  
  typedef base::Callback<bool(base::PlatformFileError error)>
      ReserveQuotaCallback;

  
  class WEBKIT_STORAGE_BROWSER_EXPORT QuotaBackend {
   public:
    QuotaBackend() {}
    virtual ~QuotaBackend() {}

    
    
    
    
    
    
    virtual void ReserveQuota(const GURL& origin,
                              FileSystemType type,
                              int64 delta,
                              const ReserveQuotaCallback& callback) = 0;

    
    virtual void ReleaseReservedQuota(const GURL& origin,
                                      FileSystemType type,
                                      int64 size) = 0;

    
    
    virtual void CommitQuotaUsage(const GURL& origin,
                                  FileSystemType type,
                                  int64 delta) = 0;

    virtual void IncrementDirtyCount(const GURL& origin,
                                    FileSystemType type) = 0;
    virtual void DecrementDirtyCount(const GURL& origin,
                                    FileSystemType type) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(QuotaBackend);
  };

  explicit QuotaReservationManager(scoped_ptr<QuotaBackend> backend);
  ~QuotaReservationManager();

  
  
  scoped_refptr<QuotaReservation> CreateReservation(
      const GURL& origin,
      FileSystemType type);

 private:
  typedef std::map<std::pair<GURL, FileSystemType>, QuotaReservationBuffer*>
      ReservationBufferByOriginAndType;

  friend class QuotaReservation;
  friend class QuotaReservationBuffer;
  friend class QuotaReservationManagerTest;

  void ReserveQuota(const GURL& origin,
                    FileSystemType type,
                    int64 delta,
                    const ReserveQuotaCallback& callback);

  void ReleaseReservedQuota(const GURL& origin,
                            FileSystemType type,
                            int64 size);

  void CommitQuotaUsage(const GURL& origin,
                        FileSystemType type,
                        int64 delta);

  void IncrementDirtyCount(const GURL& origin, FileSystemType type);
  void DecrementDirtyCount(const GURL& origin, FileSystemType type);

  scoped_refptr<QuotaReservationBuffer> GetReservationBuffer(
      const GURL& origin,
      FileSystemType type);
  void ReleaseReservationBuffer(QuotaReservationBuffer* reservation_pool);

  scoped_ptr<QuotaBackend> backend_;

  
  
  ReservationBufferByOriginAndType reservation_buffers_;

  base::SequenceChecker sequence_checker_;
  base::WeakPtrFactory<QuotaReservationManager> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(QuotaReservationManager);
};

}  

#endif  
