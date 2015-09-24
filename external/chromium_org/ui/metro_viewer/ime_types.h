// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_METRO_VIEWER_IME_TYPES_H_
#define UI_METRO_VIEWER_IME_TYPES_H_

#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace metro_viewer {

struct UnderlineInfo {
  UnderlineInfo();
  int32 start_offset;
  int32 end_offset;
  bool thick;
};

struct Composition {
  Composition();
  ~Composition();
  string16 text;
  int32 selection_start;
  int32 selection_end;
  std::vector<UnderlineInfo> underlines;
};

struct CharacterBounds {
  CharacterBounds();
  int32 left;
  int32 top;
  int32 right;
  int32 bottom;
};

}  

#endif  
