// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_FILE_IMPL_H_
#define CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_FILE_IMPL_H_

#include "content/browser/download/download_file.h"

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "content/browser/byte_stream.h"
#include "content/browser/download/base_file.h"
#include "content/browser/download/rate_estimator.h"
#include "content/public/browser/download_save_info.h"
#include "net/base/net_log.h"

namespace content {
class ByteStreamReader;
class DownloadDestinationObserver;
class DownloadManager;
class PowerSaveBlocker;
struct DownloadCreateInfo;

class CONTENT_EXPORT DownloadFileImpl : virtual public DownloadFile {
 public:
  
  
  
  
  
  
  
  
  DownloadFileImpl(
    scoped_ptr<DownloadSaveInfo> save_info,
    const base::FilePath& default_downloads_directory,
    const GURL& url,
    const GURL& referrer_url,
    bool calculate_hash,
    scoped_ptr<ByteStreamReader> stream,
    const net::BoundNetLog& bound_net_log,
    scoped_ptr<PowerSaveBlocker> power_save_blocker,
    base::WeakPtr<DownloadDestinationObserver> observer);

  virtual ~DownloadFileImpl();

  
  virtual void Initialize(const InitializeCallback& callback) OVERRIDE;
  virtual void RenameAndUniquify(
      const base::FilePath& full_path,
      const RenameCompletionCallback& callback) OVERRIDE;
  virtual void RenameAndAnnotate(
      const base::FilePath& full_path,
      const RenameCompletionCallback& callback) OVERRIDE;
  virtual void Detach() OVERRIDE;
  virtual void Cancel() OVERRIDE;
  virtual base::FilePath FullPath() const OVERRIDE;
  virtual bool InProgress() const OVERRIDE;
  virtual int64 CurrentSpeed() const OVERRIDE;
  virtual bool GetHash(std::string* hash) OVERRIDE;
  virtual std::string GetHashState() OVERRIDE;
  virtual void SetClientGuid(const std::string& guid) OVERRIDE;

 protected:
  
  virtual DownloadInterruptReason AppendDataToFile(
      const char* data, size_t data_len);

 private:
  
  void SendUpdate();

  
  
  void StreamActive();

  
  BaseFile file_;

  
  base::FilePath default_download_directory_;

  
  
  
  
  scoped_ptr<ByteStreamReader> stream_reader_;

  
  scoped_ptr<base::RepeatingTimer<DownloadFileImpl> > update_timer_;

  
  size_t bytes_seen_;
  base::TimeDelta disk_writes_time_;
  base::TimeTicks download_start_;
  RateEstimator rate_estimator_;

  net::BoundNetLog bound_net_log_;

  base::WeakPtr<DownloadDestinationObserver> observer_;

  base::WeakPtrFactory<DownloadFileImpl> weak_factory_;

  
  scoped_ptr<PowerSaveBlocker> power_save_blocker_;

  DISALLOW_COPY_AND_ASSIGN(DownloadFileImpl);
};

}  

#endif  
