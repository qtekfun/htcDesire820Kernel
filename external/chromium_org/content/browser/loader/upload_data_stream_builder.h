// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_UPLOAD_DATA_STREAM_BUILDER_H_
#define CONTENT_BROWSER_LOADER_UPLOAD_DATA_STREAM_BUILDER_H_

#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"

namespace base {
class TaskRunner;
}

namespace fileapi {
class FileSystemContext;
}

namespace net {
class UploadDataStream;
}

namespace webkit_blob {
class BlobStorageContext;
}

namespace webkit_glue {
class ResourceRequestBody;
}

namespace content {

class CONTENT_EXPORT UploadDataStreamBuilder {
 public:
  
  
  
  
  
  
  
  
  
  
  static scoped_ptr<net::UploadDataStream> Build(
      webkit_glue::ResourceRequestBody* body,
      webkit_blob::BlobStorageContext* blob_context,
      fileapi::FileSystemContext* file_system_context,
      base::TaskRunner* file_task_runner);
};

}  

#endif  