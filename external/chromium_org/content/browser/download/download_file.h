// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_FILE_H_
#define CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_FILE_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "content/common/content_export.h"
#include "content/public/browser/download_interrupt_reasons.h"

namespace content {

class DownloadManager;

class CONTENT_EXPORT DownloadFile {
 public:
  
  
  
  typedef base::Callback<void(DownloadInterruptReason reason)>
      InitializeCallback;

  
  
  
  
  typedef base::Callback<void(DownloadInterruptReason reason,
                              const base::FilePath& path)>
      RenameCompletionCallback;

  virtual ~DownloadFile() {}

  
  
  
  virtual void Initialize(const InitializeCallback& callback) = 0;

  
  
  
  virtual void RenameAndUniquify(const base::FilePath& full_path,
                                 const RenameCompletionCallback& callback) = 0;

  
  
  
  virtual void RenameAndAnnotate(const base::FilePath& full_path,
                                 const RenameCompletionCallback& callback) = 0;

  
  virtual void Detach() = 0;

  
  virtual void Cancel() = 0;

  virtual base::FilePath FullPath() const = 0;
  virtual bool InProgress() const = 0;
  virtual int64 CurrentSpeed() const = 0;

  
  
  virtual bool GetHash(std::string* hash) = 0;

  
  virtual std::string GetHashState() = 0;

  
  
  
  virtual void SetClientGuid(const std::string& guid) = 0;

  
  
  
  
  static int GetNumberOfDownloadFiles();

 protected:
  static int number_active_objects_;
};

}  

#endif  
