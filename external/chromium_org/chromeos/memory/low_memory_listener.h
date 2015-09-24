// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_MEMORY_LOW_MEMORY_LISTENER_H_
#define CHROMEOS_MEMORY_LOW_MEMORY_LISTENER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/memory/chromeos_memory_export.h"

namespace chromeos {

class LowMemoryListenerDelegate;
class LowMemoryListenerImpl;

class CHROMEOS_MEMORY_EXPORT LowMemoryListener {
 public:
  explicit LowMemoryListener(LowMemoryListenerDelegate* delegate);
  ~LowMemoryListener();

  void Start();
  void Stop();

 private:
  
  
  void OnMemoryLow();

  scoped_refptr<LowMemoryListenerImpl> observer_;
  LowMemoryListenerDelegate* delegate_;
  base::WeakPtrFactory<LowMemoryListener> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(LowMemoryListener);
};

}  

#endif  
