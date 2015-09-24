// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_PREFIX_SET_H_
#define CHROME_BROWSER_SAFE_BROWSING_PREFIX_SET_H_
#pragma once

#include <vector>

#include "chrome/browser/safe_browsing/safe_browsing_util.h"

class FilePath;

namespace safe_browsing {

class PrefixSet {
 public:
  explicit PrefixSet(const std::vector<SBPrefix>& sorted_prefixes);
  ~PrefixSet();

  
  bool Exists(SBPrefix prefix) const;

  
  static PrefixSet* LoadFile(const FilePath& filter_name);
  bool WriteFile(const FilePath& filter_name) const;

  
  
  void GetPrefixes(std::vector<SBPrefix>* prefixes) const;

  
  

  size_t IndexBinFor(size_t target_index) const;

  
  size_t GetSize() const;

  
  
  bool IsDeltaAt(size_t target_index) const;

  
  
  uint16 DeltaAt(size_t target_index) const;

  
  
  bool CheckChecksum() const;

 private:
  
  
  
  static const size_t kMaxRun = 100;

  
  
  PrefixSet(std::vector<std::pair<SBPrefix,size_t> > *index,
            std::vector<uint16> *deltas);

  
  
  
  
  std::vector<std::pair<SBPrefix,size_t> > index_;

  
  
  
  std::vector<uint16> deltas_;

  
  
  
  uint32 checksum_;

  DISALLOW_COPY_AND_ASSIGN(PrefixSet);
};

}  

#endif  
