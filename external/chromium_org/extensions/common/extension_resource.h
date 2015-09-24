// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_EXTENSION_RESOURCE_H_
#define EXTENSIONS_COMMON_EXTENSION_RESOURCE_H_

#include <string>

#include "base/files/file_path.h"

namespace extensions {

class ExtensionResource {
 public:
  
  
  enum SymlinkPolicy {
    SYMLINKS_MUST_RESOLVE_WITHIN_ROOT,
    FOLLOW_SYMLINKS_ANYWHERE,
  };

  ExtensionResource();

  ExtensionResource(const std::string& extension_id,
                    const base::FilePath& extension_root,
                    const base::FilePath& relative_path);

  ~ExtensionResource();

  
  
  
  void set_follow_symlinks_anywhere();

  
  
  
  const base::FilePath& GetFilePath() const;

  
  
  
  
  
  
  
  
  static base::FilePath GetFilePath(const base::FilePath& extension_root,
                                    const base::FilePath& relative_path,
                                    SymlinkPolicy symlink_policy);

  
  const std::string& extension_id() const { return extension_id_; }
  const base::FilePath& extension_root() const { return extension_root_; }
  const base::FilePath& relative_path() const { return relative_path_; }

  bool empty() const { return extension_root().empty(); }

  
  base::FilePath::StringType NormalizeSeperators(
      const base::FilePath::StringType& path) const;
  bool ComparePathWithDefault(const base::FilePath& path) const;

 private:
  
  std::string extension_id_;

  
  base::FilePath extension_root_;

  
  base::FilePath relative_path_;

  
  
  bool follow_symlinks_anywhere_;

  
  mutable base::FilePath full_resource_path_;
};

}  

#endif  
