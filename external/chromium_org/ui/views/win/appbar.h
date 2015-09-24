// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIN_APPBAR_H_
#define UI_VIEWS_WIN_APPBAR_H_

#include <map>

#include <windows.h>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/weak_ptr.h"

namespace views {

class Appbar {
 public:
  enum Edge {
    EDGE_TOP    = 1 << 0,
    EDGE_LEFT   = 1 << 1,
    EDGE_BOTTOM = 1 << 2,
    EDGE_RIGHT  = 1 << 3,
  };

  
  static Appbar* instance();

  
  
  
  
  
  int GetAutohideEdges(HMONITOR monitor, const base::Closure& callback);

 private:
  typedef std::map<HMONITOR, int> EdgeMap;

  Appbar();
  ~Appbar();

  
  
  void OnGotEdges(const base::Closure& callback,
                  HMONITOR monitor,
                  int returned_edges,
                  int* edges);

  EdgeMap edge_map_;

  base::WeakPtrFactory<Appbar> weak_factory_;

  
  
  bool in_callback_;

  DISALLOW_COPY_AND_ASSIGN(Appbar);
};

}  

#endif  

