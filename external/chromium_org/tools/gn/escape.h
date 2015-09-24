// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_ESCAPE_H_
#define TOOLS_GN_ESCAPE_H_

#include <iosfwd>

#include "base/strings/string_piece.h"

enum EscapingMode {
  
  ESCAPE_NONE,

  
  ESCAPE_NINJA = 1,

  
  ESCAPE_SHELL = 2,

  
  ESCAPE_NINJA_SHELL = ESCAPE_NINJA | ESCAPE_SHELL,

  ESCAPE_JSON = 4,
};

struct EscapeOptions {
  EscapeOptions()
      : mode(ESCAPE_NONE),
        convert_slashes(false),
        inhibit_quoting(false) {
  }

  EscapingMode mode;

  
  bool convert_slashes;

  
  
  
  
  
  
  bool inhibit_quoting;
};

// (if inhibit_quoting was set) quoted will be written to it. This value should
// be initialized to false by the caller and will be written to only if it's
std::string EscapeString(const base::StringPiece& str,
                         const EscapeOptions& options,
                         bool* needed_quoting);

void EscapeStringToStream(std::ostream& out,
                          const base::StringPiece& str,
                          const EscapeOptions& options);

#endif  
