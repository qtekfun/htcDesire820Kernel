// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PAGE_RANGE_H_
#define PRINTING_PAGE_RANGE_H_

#include <vector>

#include "printing_export.h"

namespace printing {

struct PageRange;

typedef std::vector<PageRange> PageRanges;

struct PRINTING_EXPORT PageRange {
  int from;
  int to;

  bool operator==(const PageRange& rhs) const {
    return from == rhs.from && to == rhs.to;
  }

  
  static std::vector<int> GetPages(const PageRanges& ranges);
};

}  

#endif  
