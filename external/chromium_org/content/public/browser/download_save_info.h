// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DOWNLOAD_SAVE_INFO_H_
#define CONTENT_PUBLIC_BROWSER_DOWNLOAD_SAVE_INFO_H_

#include "base/files/file_path.h"
#include "base/memory/linked_ptr.h"
#include "content/common/content_export.h"
#include "net/base/file_stream.h"

namespace content {

struct CONTENT_EXPORT DownloadSaveInfo {
  DownloadSaveInfo();
  ~DownloadSaveInfo();

  
  
  
  base::FilePath file_path;

  
  
  base::string16 suggested_name;

  
  scoped_ptr<net::FileStream> file_stream;

  
  int64 offset;

  
  std::string hash_state;

  
  
  
  
  
  bool prompt_for_save_location;

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadSaveInfo);
};

}  

#endif  
