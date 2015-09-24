// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_DRAGDROP_DOWNLOAD_FILE_INTERFACE_H_
#define UI_BASE_DRAGDROP_DOWNLOAD_FILE_INTERFACE_H_

#include "build/build_config.h"

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

#include "ui/base/ui_export.h"

#if defined(OS_WIN)
#include <objidl.h>
#endif

namespace base {
class FilePath;
}

namespace ui {


class UI_EXPORT DownloadFileObserver
    : public base::RefCountedThreadSafe<DownloadFileObserver> {
 public:
  virtual void OnDownloadCompleted(const base::FilePath& file_path) = 0;
  virtual void OnDownloadAborted() = 0;

 protected:
  friend class base::RefCountedThreadSafe<DownloadFileObserver>;
  virtual ~DownloadFileObserver() {}
};

class UI_EXPORT DownloadFileProvider
    : public base::RefCountedThreadSafe<DownloadFileProvider> {
 public:
  
  virtual void Start(DownloadFileObserver* observer) = 0;

  
  
  virtual bool Wait() = 0;

  
  virtual void Stop() = 0;

 protected:
  friend class base::RefCountedThreadSafe<DownloadFileProvider>;
  virtual ~DownloadFileProvider() {}
};

}  

#endif  
