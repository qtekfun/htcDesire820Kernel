// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VARIATIONS_ENTROPY_PROVIDER_H_
#define COMPONENTS_VARIATIONS_ENTROPY_PROVIDER_H_

#include <functional>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/metrics/field_trial.h"
#include "third_party/mt19937ar/mt19937ar.h"

namespace metrics {

namespace internal {

struct SeededRandGenerator : std::unary_function<uint32, uint32> {
  explicit SeededRandGenerator(uint32 seed);
  ~SeededRandGenerator();

  
  uint32 operator()(uint32 range);

  MersenneTwister mersenne_twister_;
};

void PermuteMappingUsingRandomizationSeed(uint32 randomization_seed,
                                          std::vector<uint16>* mapping);

}  

class SHA1EntropyProvider : public base::FieldTrial::EntropyProvider {
 public:
  
  
  
  explicit SHA1EntropyProvider(const std::string& entropy_source);
  virtual ~SHA1EntropyProvider();

  
  virtual double GetEntropyForTrial(const std::string& trial_name,
                                    uint32 randomization_seed) const OVERRIDE;

 private:
  std::string entropy_source_;

  DISALLOW_COPY_AND_ASSIGN(SHA1EntropyProvider);
};

class PermutedEntropyProvider : public base::FieldTrial::EntropyProvider {
 public:
  
  
  PermutedEntropyProvider(uint16 low_entropy_source,
                          size_t low_entropy_source_max);
  virtual ~PermutedEntropyProvider();

  
  virtual double GetEntropyForTrial(const std::string& trial_name,
                                    uint32 randomization_seed) const OVERRIDE;

 protected:
  
  
  virtual uint16 GetPermutedValue(uint32 randomization_seed) const;

 private:
  uint16 low_entropy_source_;
  size_t low_entropy_source_max_;

  DISALLOW_COPY_AND_ASSIGN(PermutedEntropyProvider);
};

}  

#endif  
