// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_FILESYSTEM_UTILS_H_
#define TOOLS_GN_FILESYSTEM_UTILS_H_

#include <string>

#include "base/files/file_path.h"
#include "base/strings/string_piece.h"
#include "tools/gn/settings.h"
#include "tools/gn/target.h"

class Err;
class Location;
class Value;

enum SourceFileType {
  SOURCE_UNKNOWN,
  SOURCE_ASM,
  SOURCE_C,
  SOURCE_CC,
  SOURCE_H,
  SOURCE_M,
  SOURCE_MM,
  SOURCE_S,
  SOURCE_RC,
};

SourceFileType GetSourceFileType(const SourceFile& file,
                                 Settings::TargetOS os);

const char* GetExtensionForOutputType(Target::OutputType type,
                                      Settings::TargetOS os);

std::string FilePathToUTF8(const base::FilePath::StringType& str);
inline std::string FilePathToUTF8(const base::FilePath& path) {
  return FilePathToUTF8(path.value());
}
base::FilePath UTF8ToFilePath(const base::StringPiece& sp);


size_t FindExtensionOffset(const std::string& path);

base::StringPiece FindExtension(const std::string* path);


size_t FindFilenameOffset(const std::string& path);

base::StringPiece FindFilename(const std::string* path);

base::StringPiece FindFilenameNoExtension(const std::string* path);

void RemoveFilename(std::string* path);

bool EndsWithSlash(const std::string& s);


base::StringPiece FindDir(const std::string* path);

bool EnsureStringIsInOutputDir(const SourceDir& dir,
                               const std::string& str,
                               const Value& originating,
                               Err* err);


bool IsPathAbsolute(const base::StringPiece& path);

bool MakeAbsolutePathRelativeIfPossible(const base::StringPiece& source_root,
                                        const base::StringPiece& path,
                                        std::string* dest);

std::string InvertDir(const SourceDir& dir);

void NormalizePath(std::string* path);

void ConvertPathToSystem(std::string* path);
std::string PathToSystem(const std::string& path);

std::string RebaseSourceAbsolutePath(const std::string& input,
                                     const SourceDir& dest_dir);

std::string DirectoryWithNoLastSlash(const SourceDir& dir);


SourceDir GetToolchainOutputDir(const Settings* settings);
SourceDir GetToolchainGenDir(const Settings* settings);
SourceDir GetOutputDirForSourceDir(const Settings* settings,
                                   const SourceDir& source_dir);
SourceDir GetGenDirForSourceDir(const Settings* settings,
                                const SourceDir& source_dir);
SourceDir GetTargetOutputDir(const Target* target);
SourceDir GetTargetGenDir(const Target* target);
SourceDir GetCurrentOutputDir(const Scope* scope);
SourceDir GetCurrentGenDir(const Scope* scope);

#endif  
