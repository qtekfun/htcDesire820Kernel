// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_FILE_PATH_H_
#define BASE_FILE_PATH_H_
#pragma once

#include <stddef.h>
#include <string>
#include <vector>

#include "base/base_api.h"
#include "base/compiler_specific.h"
#include "base/hash_tables.h"
#include "base/string16.h"
#include "base/string_piece.h"  
#include "build/build_config.h"

#if defined(OS_WIN)
#define FILE_PATH_USES_DRIVE_LETTERS
#define FILE_PATH_USES_WIN_SEPARATORS
#endif  

class Pickle;

class BASE_API FilePath {
 public:
#if defined(OS_POSIX)
  
  
  
  typedef std::string StringType;
#elif defined(OS_WIN)
  
  
  typedef std::wstring StringType;
#endif  

  typedef StringType::value_type CharType;

  
  
  
  
  static const CharType kSeparators[];

  
  static const CharType kCurrentDirectory[];

  
  static const CharType kParentDirectory[];

  
  static const CharType kExtensionSeparator;

  FilePath();
  FilePath(const FilePath& that);
  explicit FilePath(const StringType& path);
  ~FilePath();
  FilePath& operator=(const FilePath& that);

  bool operator==(const FilePath& that) const;

  bool operator!=(const FilePath& that) const;

  
  bool operator<(const FilePath& that) const {
    return path_ < that.path_;
  }

  const StringType& value() const { return path_; }

  bool empty() const { return path_.empty(); }

  void clear() { path_.clear(); }

  
  static bool IsSeparator(CharType character);

  
  
  
  void GetComponents(std::vector<FilePath::StringType>* components) const;

  
  
  
  
  
  bool IsParent(const FilePath& child) const;

  
  
  
  
  
  
  
  
  bool AppendRelativePath(const FilePath& child, FilePath* path) const;

  
  
  
  
  
  FilePath DirName() const;

  
  
  
  
  FilePath BaseName() const;

  
  
  
  
  
  
  
  
  StringType Extension() const;

  
  
  
  FilePath RemoveExtension() const;

  
  
  
  
  
  
  
  FilePath InsertBeforeExtension(const StringType& suffix) const;
  FilePath InsertBeforeExtensionASCII(const base::StringPiece& suffix) const;

  
  
  
  
  FilePath ReplaceExtension(const StringType& extension) const;

  
  
  bool MatchesExtension(const StringType& extension) const;

  
  
  
  
  
  
  FilePath Append(const StringType& component) const WARN_UNUSED_RESULT;
  FilePath Append(const FilePath& component) const WARN_UNUSED_RESULT;

  
  
  
  
  
  
  FilePath AppendASCII(const base::StringPiece& component)
      const WARN_UNUSED_RESULT;

  
  
  
  
  bool IsAbsolute() const;

  
  
  FilePath StripTrailingSeparators() const;

  
  
  bool ReferencesParent() const;

  
  
  
  
  string16 LossyDisplayName() const;

  
  
  
  std::string MaybeAsASCII() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static FilePath FromWStringHack(const std::wstring& wstring);

  
  static void WriteStringTypeToPickle(Pickle* pickle,
                                      const FilePath::StringType& path);
  static bool ReadStringTypeFromPickle(Pickle* pickle, void** iter,
                                       FilePath::StringType* path);

  void WriteToPickle(Pickle* pickle);
  bool ReadFromPickle(Pickle* pickle, void** iter);

#if defined(FILE_PATH_USES_WIN_SEPARATORS)
  
  FilePath NormalizeWindowsPathSeparators() const;
#endif

  
  
  
  
  // The methods are written as a static method so that they can also be used
  
  
  
  static int CompareIgnoreCase(const StringType& string1,
                               const StringType& string2);
  static bool CompareEqualIgnoreCase(const StringType& string1,
                                     const StringType& string2) {
    return CompareIgnoreCase(string1, string2) == 0;
  }
  static bool CompareLessIgnoreCase(const StringType& string1,
                                    const StringType& string2) {
    return CompareIgnoreCase(string1, string2) < 0;
  }

#if defined(OS_MACOSX)
  
  
  
  
  
  static StringType GetHFSDecomposedForm(const FilePath::StringType& string);

  
  
  
  
  static int HFSFastUnicodeCompare(const StringType& string1,
                                   const StringType& string2);
#endif

 private:
  
  
  
  
  
  void StripTrailingSeparatorsInternal();

  StringType path_;
};

#if defined(OS_POSIX)
#define FILE_PATH_LITERAL(x) x
#define PRFilePath "s"
#define PRFilePathLiteral "%s"
#elif defined(OS_WIN)
#define FILE_PATH_LITERAL(x) L ## x
#define PRFilePath "ls"
#define PRFilePathLiteral L"%ls"
#endif  

#if defined(COMPILER_GCC)
namespace __gnu_cxx {

template<>
struct hash<FilePath> {
  size_t operator()(const FilePath& f) const {
    return hash<FilePath::StringType>()(f.value());
  }
};

}  
#elif defined(COMPILER_MSVC)
namespace stdext {

inline size_t hash_value(const FilePath& f) {
  return hash_value(f.value());
}

}  
#endif  

#endif  
