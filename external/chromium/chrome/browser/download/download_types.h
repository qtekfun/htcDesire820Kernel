// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_TYPES_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_TYPES_H_
#pragma once

#include <vector>

#include "base/file_path.h"
#include "base/memory/linked_ptr.h"
#include "base/synchronization/lock.h"
#include "net/base/file_stream.h"

namespace net {
class IOBuffer;
}

struct DownloadBuffer {
  DownloadBuffer();
  ~DownloadBuffer();

  base::Lock lock;
  typedef std::pair<net::IOBuffer*, int> Contents;
  std::vector<Contents> contents;
};

struct DownloadSaveInfo {
  DownloadSaveInfo();
  DownloadSaveInfo(const DownloadSaveInfo& info);
  ~DownloadSaveInfo();
  DownloadSaveInfo& operator=(const DownloadSaveInfo& info);

  FilePath file_path;
  linked_ptr<net::FileStream> file_stream;
};

#endif  
