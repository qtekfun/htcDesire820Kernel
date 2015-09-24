// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_RESOURCE_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_RESOURCE_H_
#pragma once

#include <string>

#include "base/file_path.h"

class ExtensionResource {
 public:
  ExtensionResource();

  ExtensionResource(const std::string& extension_id,
                    const FilePath& extension_root,
                    const FilePath& relative_path);

  ~ExtensionResource();

  
  
  
  const FilePath& GetFilePath() const;

  
  
  
  
  static FilePath GetFilePath(const FilePath& extension_root,
                              const FilePath& relative_path);

  
  const std::string& extension_id() const { return extension_id_; }
  const FilePath& extension_root() const { return extension_root_; }
  const FilePath& relative_path() const { return relative_path_; }

  bool empty() { return extension_root().empty(); }

  
  FilePath::StringType NormalizeSeperators(
      const FilePath::StringType& path) const;
  bool ComparePathWithDefault(const FilePath& path) const;

 private:
  
  std::string extension_id_;

  
  FilePath extension_root_;

  
  FilePath relative_path_;

  
  mutable FilePath full_resource_path_;
};

#endif  
