// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_X_SELECTION_OWNER_H_
#define UI_BASE_X_SELECTION_OWNER_H_

#include <X11/Xlib.h>

#undef RootWindow

#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "ui/base/ui_export.h"
#include "ui/base/x/selection_utils.h"
#include "ui/gfx/x/x11_atom_cache.h"

namespace ui {

class UI_EXPORT SelectionOwner {
 public:
  SelectionOwner(Display* xdisplay,
                 ::Window xwindow,
                 ::Atom selection_name);
  ~SelectionOwner();

  
  const SelectionFormatMap& selection_format_map() { return format_map_; }

  
  void RetrieveTargets(std::vector<Atom>* targets);

  
  
  void TakeOwnershipOfSelection(const SelectionFormatMap& data);

  
  void Clear();

  
  void OnSelectionRequest(const XSelectionRequestEvent& event);
  void OnSelectionClear(const XSelectionClearEvent& event);
  
  

 private:
  
  Display* x_display_;
  ::Window x_window_;

  
  ::Atom selection_name_;

  
  SelectionFormatMap format_map_;

  X11AtomCache atom_cache_;

  DISALLOW_COPY_AND_ASSIGN(SelectionOwner);
};

}  

#endif  
