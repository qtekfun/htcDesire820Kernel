// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_TEST_EXTENSION_DIR_H_
#define CHROME_BROWSER_EXTENSIONS_TEST_EXTENSION_DIR_H_

#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "base/strings/string_piece.h"

namespace extensions {

class TestExtensionDir {
 public:
  TestExtensionDir();

  ~TestExtensionDir();

  
  
  void WriteManifest(base::StringPiece manifest);

  
  
  void WriteFile(const base::FilePath::StringType& filename,
                 base::StringPiece contents);

  
  
  base::FilePath Pack();

  
  base::FilePath unpacked_path() {
    return dir_.path();
  }

 private:
  
  base::ScopedTempDir dir_;

  
  base::ScopedTempDir crx_dir_;
};

}  

#endif  
