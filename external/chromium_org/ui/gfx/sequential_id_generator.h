// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SEQUENTIAL_ID_GENERATOR_H_
#define UI_GFX_SEQUENTIAL_ID_GENERATOR_H_

#include <map>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "ui/gfx/gfx_export.h"

namespace ui {

class GFX_EXPORT SequentialIDGenerator {
 public:
   
  explicit SequentialIDGenerator(uint32 min_id);
  ~SequentialIDGenerator();

  
  
  
  uint32 GetGeneratedID(uint32 number);

  
  
  bool HasGeneratedIDFor(uint32 number) const;

  
  
  
  void ReleaseGeneratedID(uint32 id);

  
  
  void ReleaseNumber(uint32 number);

 private:
  typedef base::hash_map<uint32, uint32> IDMap;

  uint32 GetNextAvailableID();

  void UpdateNextAvailableIDAfterRelease(uint32 id);

  IDMap number_to_id_;
  IDMap id_to_number_;

  uint32 min_id_;
  uint32 min_available_id_;

  DISALLOW_COPY_AND_ASSIGN(SequentialIDGenerator);
};

}  

#endif  
