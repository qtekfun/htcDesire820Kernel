// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_DISPLAY_LAYOUT_H_
#define ASH_DISPLAY_DISPLAY_LAYOUT_H_

#include <map>
#include <string>

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"

namespace base {
class Value;
template <typename T> class JSONValueConverter;
}

namespace ash {

typedef std::pair<int64, int64> DisplayIdPair;

struct ASH_EXPORT DisplayLayout {
  
  enum Position {
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
  };

  
  
  
  static DisplayLayout FromInts(int position, int offsets);

  DisplayLayout();
  DisplayLayout(Position position, int offset);

  
  DisplayLayout Invert() const WARN_UNUSED_RESULT;

  
  static bool ConvertFromValue(const base::Value& value, DisplayLayout* layout);
  static bool ConvertToValue(const DisplayLayout& layout, base::Value* value);

  
  
  static void RegisterJSONConverter(
      base::JSONValueConverter<DisplayLayout>* converter);

  Position position;

  
  
  int offset;

  
  bool mirrored;

  
  int64 primary_id;

  
  std::string ToString() const;
};

}  

#endif
