// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_CHANGE_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_CHANGE_H_

#include <set>

#include "base/files/file_path.h"

namespace drive {

class FileChange;

typedef std::set<FileChange> FileChangeSet;

class FileChange {
 public:
  enum Type {
    DELETED,
    ADDED,
    CHANGED,
  };

  
  
  FileChange(const base::FilePath& path, Type type);
  ~FileChange();

  
  static FileChangeSet CreateSingleSet(const base::FilePath& path, Type type);

  bool operator==(const FileChange &file_change) const {
    return path_ == file_change.path() && type_ == file_change.type();
  }

  bool operator<(const FileChange &file_change) const {
    return (path_ < file_change.path()) ||
           (path_ == file_change.path() && type_ < file_change.type());
  }

  const base::FilePath& path() const { return path_; }

  Type type() const { return type_; }

 private:
  const base::FilePath path_;
  const Type type_;
};

}  

#endif  
