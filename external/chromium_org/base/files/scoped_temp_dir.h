// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_SCOPED_TEMP_DIR_H_
#define BASE_FILES_SCOPED_TEMP_DIR_H_


#include "base/base_export.h"
#include "base/files/file_path.h"

namespace base {

class BASE_EXPORT ScopedTempDir {
 public:
  
  ScopedTempDir();

  
  ~ScopedTempDir();

  
  
  bool CreateUniqueTempDir() WARN_UNUSED_RESULT;

  
  bool CreateUniqueTempDirUnderPath(const FilePath& path) WARN_UNUSED_RESULT;

  
  
  bool Set(const FilePath& path) WARN_UNUSED_RESULT;

  
  bool Delete() WARN_UNUSED_RESULT;

  
  
  FilePath Take();

  const FilePath& path() const { return path_; }

  
  bool IsValid() const;

 private:
  FilePath path_;

  DISALLOW_COPY_AND_ASSIGN(ScopedTempDir);
};

}  

#endif  
