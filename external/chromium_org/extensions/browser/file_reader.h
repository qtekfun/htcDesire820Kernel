// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_FILE_READER_H_
#define EXTENSIONS_BROWSER_FILE_READER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "extensions/common/extension_resource.h"

namespace base {
class MessageLoop;
}

class FileReader : public base::RefCountedThreadSafe<FileReader> {
 public:
  
  typedef base::Callback<void(bool, const std::string&)> Callback;

  FileReader(const extensions::ExtensionResource& resource,
             const Callback& callback);

  
  
  void Start();

 private:
  friend class base::RefCountedThreadSafe<FileReader>;

  virtual ~FileReader();

  void ReadFileOnBackgroundThread();

  extensions::ExtensionResource resource_;
  Callback callback_;
  base::MessageLoop* origin_loop_;
};

#endif  
