// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_SOURCE_DIR_H_
#define TOOLS_GN_SOURCE_DIR_H_

#include <string>

#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/strings/string_piece.h"

class SourceFile;

class SourceDir {
 public:
  enum SwapIn { SWAP_IN };

  SourceDir();
  explicit SourceDir(const base::StringPiece& p);
  
  
  SourceDir(SwapIn, std::string* s);
  ~SourceDir();

  
  
  
  
  
  
  
  
  SourceFile ResolveRelativeFile(
      const base::StringPiece& p,
      const base::StringPiece& source_root = base::StringPiece()) const;
  SourceDir ResolveRelativeDir(
      const base::StringPiece& p,
      const base::StringPiece& source_root = base::StringPiece()) const;

  
  
  base::FilePath Resolve(const base::FilePath& source_root) const;

  bool is_null() const { return value_.empty(); }
  const std::string& value() const { return value_; }

  
  
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

  void SwapValue(std::string* v);

  bool operator==(const SourceDir& other) const {
    return value_ == other.value_;
  }
  bool operator!=(const SourceDir& other) const {
    return !operator==(other);
  }
  bool operator<(const SourceDir& other) const {
    return value_ < other.value_;
  }

 private:
  friend class SourceFile;
  std::string value_;

  
};

namespace BASE_HASH_NAMESPACE {

#if defined(COMPILER_GCC)
template<> struct hash<SourceDir> {
  std::size_t operator()(const SourceDir& v) const {
    hash<std::string> h;
    return h(v.value());
  }
};
#elif defined(COMPILER_MSVC)
inline size_t hash_value(const SourceDir& v) {
  return hash_value(v.value());
}
#endif  

}  

#endif  
