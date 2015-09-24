// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_REGISTRY_SYNCER_WIN_H_
#define CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_REGISTRY_SYNCER_WIN_H_

#include "base/timer/timer.h"

namespace chrome_variations {

class VariationsRegistrySyncer {
 public:
  VariationsRegistrySyncer();
  ~VariationsRegistrySyncer();

  
  
  
  void RequestRegistrySync();

 private:
  
  void SyncWithRegistry();

  
  
  
  
  
  base::OneShotTimer<VariationsRegistrySyncer> timer_;

  DISALLOW_COPY_AND_ASSIGN(VariationsRegistrySyncer);
};

}  

#endif  
