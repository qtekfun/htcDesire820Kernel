// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_BASE_CANCELATION_SIGNAL_H_
#define SYNC_INTERNAL_API_PUBLIC_BASE_CANCELATION_SIGNAL_H_

#include "base/synchronization/lock.h"
#include "sync/base/sync_export.h"

namespace syncer {

class CancelationObserver;

class SYNC_EXPORT_PRIVATE CancelationSignal {
 public:
  CancelationSignal();
  ~CancelationSignal();

  
  
  
  
  
  
  
  bool TryRegisterHandler(CancelationObserver* handler);

  
  void UnregisterHandler(CancelationObserver* handler);

  
  bool IsSignalled();

  
  
  
  void Signal();

 private:
  
  base::Lock signal_lock_;

  
  bool signalled_;

  
  CancelationObserver* handler_;
};

}  

#endif  
