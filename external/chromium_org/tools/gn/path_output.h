// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_PATH_OUTPUT_H_
#define TOOLS_GN_PATH_OUTPUT_H_

#include <iosfwd>
#include <string>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "tools/gn/escape.h"
#include "tools/gn/source_dir.h"

class OutputFile;
class SourceFile;

namespace base {
class FilePath;
}

class PathOutput {
 public:
  
  
  
  
  enum DirSlashEnding {
    DIR_INCLUDE_LAST_SLASH,
    DIR_NO_LAST_SLASH,
  };

  PathOutput(const SourceDir& current_dir,
             EscapingMode escaping,
             bool convert_slashes);
  ~PathOutput();

  
  EscapingMode escaping_mode() const { return options_.mode; }

  const SourceDir& current_dir() const { return current_dir_; }

  
  
  
  
  bool convert_slashes_to_system() const { return options_.convert_slashes; }

  
  
  
  
  
  bool inhibit_quoting() const { return options_.inhibit_quoting; }
  void set_inhibit_quoting(bool iq) { options_.inhibit_quoting = iq; }

  void WriteFile(std::ostream& out, const SourceFile& file) const;
  void WriteFile(std::ostream& out, const OutputFile& file) const;
  void WriteFile(std::ostream& out, const base::FilePath& file) const;
  void WriteDir(std::ostream& out,
                const SourceDir& dir,
                DirSlashEnding slash_ending) const;

  
  
  void WritePathStr(std::ostream& out, const base::StringPiece& str) const;

 private:
  
  
  void WriteSourceRelativeString(std::ostream& out,
                                 const base::StringPiece& str) const;

  SourceDir current_dir_;

  
  std::string inverse_current_dir_;

  
  
  EscapeOptions options_;
};

#endif  
