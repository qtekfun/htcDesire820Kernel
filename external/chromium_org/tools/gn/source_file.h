// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_SOURCE_FILE_H_
#define TOOLS_GN_SOURCE_FILE_H_

#include <string>

#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/strings/string_piece.h"

class SourceDir;

class SourceFile {
 public:
  SourceFile();

  
  explicit SourceFile(const base::StringPiece& p);

  ~SourceFile();

  bool is_null() const { return value_.empty(); }
  const std::string& value() const { return value_; }

  
  std::string GetName() const;
  SourceDir GetDir() const;

  
  
  base::FilePath Resolve(const base::FilePath& source_root) const;

  
  
  bool is_source_absolute() const {
    return value_.size() >= 2 && value_[0] == '/' && value_[1] == '/';
  }

  
  
  bool is_system_absolute() const {
    return !is_source_absolute();
  }

  
  
  
  
  
  
  base::StringPiece SourceAbsoluteWithOneSlash() const {
    CHECK(is_source_absolute());
    return base::StringPiece(&value_[1], value_.size() - 1);
  }

  bool operator==(const SourceFile& other) const {
    return value_ == other.value_;
  }
  bool operator!=(const SourceFile& other) const {
    return !operator==(other);
  }
  bool operator<(const SourceFile& other) const {
    return value_ < other.value_;
  }

 private:
  friend class SourceDir;

  std::string value_;

  
};

namespace BASE_HASH_NAMESPACE {

#if defined(COMPILER_GCC)
template<> struct hash<SourceFile> {
  std::size_t operator()(const SourceFile& v) const {
    hash<std::string> h;
    return h(v.value());
  }
};
#elif defined(COMPILER_MSVC)
inline size_t hash_value(const SourceFile& v) {
  return hash_value(v.value());
}
#endif  

}  

#endif  
