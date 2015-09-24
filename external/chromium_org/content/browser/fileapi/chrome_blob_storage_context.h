// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FILEAPI_CHROME_BLOB_STORAGE_CONTEXT_H_
#define CONTENT_BROWSER_FILEAPI_CHROME_BLOB_STORAGE_CONTEXT_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "content/common/content_export.h"

namespace webkit_blob {
class BlobStorageContext;
}

namespace content {
class BrowserContext;
struct ChromeBlobStorageContextDeleter;

class CONTENT_EXPORT ChromeBlobStorageContext
    : public base::RefCountedThreadSafe<ChromeBlobStorageContext,
                                        ChromeBlobStorageContextDeleter> {
 public:
  ChromeBlobStorageContext();

  static ChromeBlobStorageContext* GetFor(
      BrowserContext* browser_context);

  void InitializeOnIOThread();

  webkit_blob::BlobStorageContext* context() const {
    return context_.get();
  }

 protected:
  virtual ~ChromeBlobStorageContext();

 private:
  friend class base::DeleteHelper<ChromeBlobStorageContext>;
  friend class base::RefCountedThreadSafe<ChromeBlobStorageContext,
                                          ChromeBlobStorageContextDeleter>;
  friend struct ChromeBlobStorageContextDeleter;

  void DeleteOnCorrectThread() const;

  scoped_ptr<webkit_blob::BlobStorageContext> context_;
};

struct ChromeBlobStorageContextDeleter {
  static void Destruct(const ChromeBlobStorageContext* context) {
    context->DeleteOnCorrectThread();
  }
};

}  

#endif  
