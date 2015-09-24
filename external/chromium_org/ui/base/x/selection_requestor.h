// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_X_SELECTION_REQUESTOR_H_
#define UI_BASE_X_SELECTION_REQUESTOR_H_

#include <X11/Xlib.h>

#undef RootWindow

#include <list>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted_memory.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/x/x11_atom_cache.h"

namespace ui {
class SelectionData;

class UI_EXPORT SelectionRequestor {
 public:
  SelectionRequestor(Display* xdisplay,
                     ::Window xwindow,
                     ::Atom selection_name);
  ~SelectionRequestor();

  
  
  
  
  
  bool PerformBlockingConvertSelection(
      ::Atom target,
      scoped_refptr<base::RefCountedMemory>* out_data,
      size_t* out_data_bytes,
      size_t* out_data_items,
      ::Atom* out_type);

  
  
  SelectionData RequestAndWaitForTypes(const std::vector< ::Atom>& types);

  
  
  void OnSelectionNotify(const XSelectionEvent& event);

 private:
  
  Display* x_display_;
  ::Window x_window_;

  
  ::Atom selection_name_;

  
  struct PendingRequest {
    PendingRequest(Atom target, base::Closure quit_closure);
    ~PendingRequest();

    
    
    ::Atom target;

    
    base::Closure quit_closure;

    
    
    
    
    ::Atom returned_property;

    
    bool returned;
  };

  
  std::list<PendingRequest*> pending_requests_;

  X11AtomCache atom_cache_;

  DISALLOW_COPY_AND_ASSIGN(SelectionRequestor);
};

}  

#endif  
