// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_CREATOR_FILTER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_CREATOR_FILTER_H_

#include "base/memory/ref_counted.h"

namespace base {
class FilePath;
}

namespace extensions {

class ExtensionCreatorFilter
      : public base::RefCounted<ExtensionCreatorFilter> {
 public:
  ExtensionCreatorFilter() {}

  
  
  bool ShouldPackageFile(const base::FilePath& file_path);

 private:
  friend class base::RefCounted<ExtensionCreatorFilter>;
  ~ExtensionCreatorFilter() {}
  DISALLOW_COPY_AND_ASSIGN(ExtensionCreatorFilter);
};

}  

#endif  
