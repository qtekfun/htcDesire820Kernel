// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_FILE_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_FILE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/download/base_file.h"
#include "chrome/browser/download/download_types.h"

struct DownloadCreateInfo;
class DownloadManager;
class ResourceDispatcherHost;

class DownloadFile : public BaseFile {
 public:
  DownloadFile(const DownloadCreateInfo* info,
               DownloadManager* download_manager);
  virtual ~DownloadFile();

  
  void CancelDownloadRequest(ResourceDispatcherHost* rdh);

  int id() const { return id_; }
  DownloadManager* GetDownloadManager();

  virtual std::string DebugString() const;

 private:
  
  
  int id_;

  
  int child_id_;

  
  int request_id_;

  
  scoped_refptr<DownloadManager> download_manager_;

  DISALLOW_COPY_AND_ASSIGN(DownloadFile);
};

#endif  
