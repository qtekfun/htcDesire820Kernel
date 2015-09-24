// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SAFE_BROWSING_SCORER_H_
#define CHROME_RENDERER_SAFE_BROWSING_SCORER_H_

#include <string>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/strings/string_piece.h"
#include "chrome/common/safe_browsing/client_model.pb.h"

namespace safe_browsing {
class FeatureMap;

class Scorer {
 public:
  virtual ~Scorer();

  
  
  static Scorer* Create(const base::StringPiece& model_str);

  
  
  
  virtual double ComputeScore(const FeatureMap& features) const;

  
  int model_version() const;

  

  
  
  const base::hash_set<std::string>& page_terms() const;

  
  
  const base::hash_set<uint32>& page_words() const;

  
  size_t max_words_per_term() const;

  
  uint32 murmurhash3_seed() const;

 protected:
  
  
  Scorer();

 private:
  friend class PhishingScorerTest;

  
  
  
  
  
  double ComputeRuleScore(const ClientSideModel::Rule& rule,
                          const FeatureMap& features) const;

  ClientSideModel model_;
  base::hash_set<std::string> page_terms_;
  base::hash_set<uint32> page_words_;

  DISALLOW_COPY_AND_ASSIGN(Scorer);
};
}  

#endif  
