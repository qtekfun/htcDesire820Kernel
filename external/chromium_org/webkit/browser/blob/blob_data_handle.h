// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_BLOB_BLOB_DATA_HANDLE_H_
#define WEBKIT_BROWSER_BLOB_BLOB_DATA_HANDLE_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/supports_user_data.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace base {
class SequencedTaskRunner;
}

namespace webkit_blob {

class BlobData;
class BlobStorageContext;

class WEBKIT_STORAGE_BROWSER_EXPORT BlobDataHandle
    : public base::SupportsUserData::Data {
 public:
  virtual ~BlobDataHandle();  
  BlobData* data() const;  

 private:
  friend class BlobStorageContext;
  BlobDataHandle(BlobData* blob_data, BlobStorageContext* context,
                 base::SequencedTaskRunner* task_runner);

  static void DeleteHelper(
      base::WeakPtr<BlobStorageContext> context,
      BlobData* blob_data);

  BlobData* blob_data_;  
  base::WeakPtr<BlobStorageContext> context_;
  scoped_refptr<base::SequencedTaskRunner> io_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(BlobDataHandle);
};

}  

#endif  
