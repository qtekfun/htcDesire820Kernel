// Copyright 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_FILEPATH_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_FILEPATH_H_

#include <gtest/internal/gtest-string.h>

namespace testing {
namespace internal {


class FilePath {
 public:
  FilePath() : pathname_("") { }
  FilePath(const FilePath& rhs) : pathname_(rhs.pathname_) { }
  explicit FilePath(const char* pathname) : pathname_(pathname) { }
  explicit FilePath(const String& pathname) : pathname_(pathname) { }

  void Set(const FilePath& rhs) {
    pathname_ = rhs.pathname_;
  }

  String ToString() const { return pathname_; }
  const char* c_str() const { return pathname_.c_str(); }

  
  
  
  
  static FilePath MakeFileName(const FilePath& directory,
                               const FilePath& base_name,
                               int number,
                               const char* extension);

  
  
  
  
  
  
  
  
  static FilePath GenerateUniqueFileName(const FilePath& directory,
                                         const FilePath& base_name,
                                         const char* extension);

  
  
  
  FilePath RemoveTrailingPathSeparator() const;

  
  
  
  
  
  
  FilePath RemoveDirectoryName() const;

  
  
  
  
  
  
  FilePath RemoveFileName() const;

  
  
  
  
  FilePath RemoveExtension(const char* extension) const;

  
  
  
  
  bool CreateDirectoriesRecursively() const;

  
  
  
  
  bool CreateFolder() const;

  
  
  bool FileOrDirectoryExists() const;

  
  
  bool DirectoryExists() const;

  
  
  
  bool IsDirectory() const;

 private:
  String pathname_;

  
  FilePath& operator=(const FilePath& rhs);
};  

}  
}  

#endif  
