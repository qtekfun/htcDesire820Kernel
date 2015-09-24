// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_METRICS_CACHING_PERMUTED_ENTROPY_PROVIDER_H_
#define CHROME_COMMON_METRICS_CACHING_PERMUTED_ENTROPY_PROVIDER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/threading/thread_checker.h"
#include "chrome/common/metrics/proto/permuted_entropy_cache.pb.h"
#include "components/variations/entropy_provider.h"

class PrefService;
class PrefRegistrySimple;

namespace metrics {

class CachingPermutedEntropyProvider : public PermutedEntropyProvider {
 public:
  
  
  
  CachingPermutedEntropyProvider(PrefService* local_state,
                                 uint16 low_entropy_source,
                                 size_t low_entropy_source_max);
  virtual ~CachingPermutedEntropyProvider();

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  
  static void ClearCache(PrefService* local_state);

 private:
  
  virtual uint16 GetPermutedValue(uint32 randomization_seed) const OVERRIDE;

  
  void ReadFromLocalState() const;

  
  void UpdateLocalState() const;

  
  void AddToCache(uint32 randomization_seed, uint16 value) const;

  
  
  bool FindValue(uint32 randomization_seed, uint16* value) const;

  base::ThreadChecker thread_checker_;
  PrefService* local_state_;
  mutable PermutedEntropyCache cache_;

  DISALLOW_COPY_AND_ASSIGN(CachingPermutedEntropyProvider);
};

}  

#endif  
