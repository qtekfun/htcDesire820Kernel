// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_MEMORY_PRESSURE_LISTENER_H_
#define BASE_MEMORY_PRESSURE_LISTENER_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/callback.h"

namespace base {

class BASE_EXPORT MemoryPressureListener {
 public:
  enum MemoryPressureLevel {
#define DEFINE_MEMORY_PRESSURE_LEVEL(name, value) name = value,
#include "base/memory/memory_pressure_level_list.h"
#undef DEFINE_MEMORY_PRESSURE_LEVEL
  };

  typedef base::Callback<void(MemoryPressureLevel)> MemoryPressureCallback;

  explicit MemoryPressureListener(
      const MemoryPressureCallback& memory_pressure_callback);
  ~MemoryPressureListener();

  
  static void NotifyMemoryPressure(MemoryPressureLevel memory_pressure_level);

 private:
  void Notify(MemoryPressureLevel memory_pressure_level);

  MemoryPressureCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(MemoryPressureListener);
};

}  

#endif  
