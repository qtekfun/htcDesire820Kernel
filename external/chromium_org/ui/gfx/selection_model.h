// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SELECTION_MODEL_H_
#define UI_GFX_SELECTION_MODEL_H_

#include <string>

#include "ui/gfx/gfx_export.h"
#include "ui/gfx/range/range.h"

namespace gfx {

enum VisualCursorDirection {
  CURSOR_LEFT,
  CURSOR_RIGHT
};
enum LogicalCursorDirection {
  CURSOR_BACKWARD,
  CURSOR_FORWARD
};

class GFX_EXPORT SelectionModel {
 public:
  // Create a default SelectionModel to be overwritten later.
  SelectionModel();
  
  
  
  
  SelectionModel(size_t position, LogicalCursorDirection affinity);
  
  
  SelectionModel(const Range& selection, LogicalCursorDirection affinity);

  const Range& selection() const { return selection_; }
  size_t caret_pos() const { return selection_.end(); }
  LogicalCursorDirection caret_affinity() const { return caret_affinity_; }

  bool operator==(const SelectionModel& sel) const;
  bool operator!=(const SelectionModel& sel) const { return !(*this == sel); }

  std::string ToString() const;

 private:
  friend class RenderText;

  
  
  
  void set_selection_start(size_t pos) { selection_.set_start(pos); }

  
  Range selection_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  LogicalCursorDirection caret_affinity_;
};

}  

#endif  
