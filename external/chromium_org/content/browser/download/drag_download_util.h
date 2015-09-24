// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DRAG_DOWNLOAD_UTIL_H_
#define CONTENT_BROWSER_DOWNLOAD_DRAG_DOWNLOAD_UTIL_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "content/browser/download/drag_download_file.h"
#include "ui/base/dragdrop/download_file_interface.h"

class GURL;

namespace base {
class FilePath;
}

namespace net {
class FileStream;
}

namespace content {

bool ParseDownloadMetadata(const base::string16& metadata,
                           base::string16* mime_type,
                           base::FilePath* file_name,
                           GURL* url);

CONTENT_EXPORT net::FileStream* CreateFileStreamForDrop(
    base::FilePath* file_path, net::NetLog* net_log);

class PromiseFileFinalizer : public ui::DownloadFileObserver {
 public:
  explicit PromiseFileFinalizer(DragDownloadFile* drag_file_downloader);

  
  virtual void OnDownloadCompleted(const base::FilePath& file_path) OVERRIDE;
  virtual void OnDownloadAborted() OVERRIDE;

 protected:
  virtual ~PromiseFileFinalizer();

 private:
  void Cleanup();

  scoped_refptr<DragDownloadFile> drag_file_downloader_;

  DISALLOW_COPY_AND_ASSIGN(PromiseFileFinalizer);
};

}  

#endif  
