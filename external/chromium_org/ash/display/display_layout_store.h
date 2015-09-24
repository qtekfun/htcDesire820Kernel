// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_DISPLAY_LAYOUT_STORE_H_
#define ASH_DISPLAY_DISPLAY_LAYOUT_STORE_H_

#include <map>

#include "ash/ash_export.h"
#include "ash/display/display_layout.h"

namespace ash {
namespace internal {

class ASH_EXPORT DisplayLayoutStore {
 public:
  DisplayLayoutStore();
  ~DisplayLayoutStore();

  const DisplayLayout& default_display_layout() const {
    return default_display_layout_;
  }
  void SetDefaultDisplayLayout(const DisplayLayout& layout);

  
  void RegisterLayoutForDisplayIdPair(int64 id1,
                                      int64 id2,
                                      const DisplayLayout& layout);

  
  
  DisplayLayout GetRegisteredDisplayLayout(const DisplayIdPair& pair);

  
  
  
  DisplayLayout ComputeDisplayLayoutForDisplayIdPair(
      const DisplayIdPair& display_pair);

  
  
  
  void UpdateMirrorStatus(const DisplayIdPair& display_pair,
                          bool mirrored);

  
  
  
  void UpdatePrimaryDisplayId(const DisplayIdPair& display_pair,
                              int64 display_id);

 private:
  
  DisplayLayout CreateDisplayLayout(const DisplayIdPair& display_pair);

  
  DisplayLayout default_display_layout_;

  
  std::map<DisplayIdPair, DisplayLayout> paired_layouts_;

  DISALLOW_COPY_AND_ASSIGN(DisplayLayoutStore);
};

}  
}  

#endif  
