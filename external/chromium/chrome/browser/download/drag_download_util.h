// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DRAG_DOWNLOAD_UTIL_H_
#define CHROME_BROWSER_DOWNLOAD_DRAG_DOWNLOAD_UTIL_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "chrome/browser/download/drag_download_file.h"
#include "ui/base/dragdrop/download_file_interface.h"

class FilePath;
class GURL;
namespace net {
class FileStream;
}

namespace drag_download_util {

bool ParseDownloadMetadata(const string16& metadata,
                           string16* mime_type,
                           FilePath* file_name,
                           GURL* url);

net::FileStream* CreateFileStreamForDrop(FilePath* file_path);

class PromiseFileFinalizer : public ui::DownloadFileObserver {
 public:
  explicit PromiseFileFinalizer(DragDownloadFile* drag_file_downloader);
  virtual ~PromiseFileFinalizer();

  
  virtual void OnDownloadCompleted(const FilePath& file_path);
  virtual void OnDownloadAborted();

 private:
  void Cleanup();

  scoped_refptr<DragDownloadFile> drag_file_downloader_;

  DISALLOW_COPY_AND_ASSIGN(PromiseFileFinalizer);
};

}  

#endif  
