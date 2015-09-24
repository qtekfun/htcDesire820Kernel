// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_VLOG_H_
#define BASE_VLOG_H_

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/strings/string_piece.h"

namespace logging {

class BASE_EXPORT VlogInfo {
 public:
  static const int kDefaultVlogLevel;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  VlogInfo(const std::string& v_switch,
           const std::string& vmodule_switch,
           int* min_log_level);
  ~VlogInfo();

  
  
  int GetVlogLevel(const base::StringPiece& file) const;

 private:
  void SetMaxVlogLevel(int level);
  int GetMaxVlogLevel() const;

  
  
  struct VmodulePattern;
  std::vector<VmodulePattern> vmodule_levels_;
  int* min_log_level_;

  DISALLOW_COPY_AND_ASSIGN(VlogInfo);
};

BASE_EXPORT bool MatchVlogPattern(const base::StringPiece& string,
                                  const base::StringPiece& vlog_pattern);

}  

#endif  
