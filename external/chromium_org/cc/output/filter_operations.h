// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_OUTPUT_FILTER_OPERATIONS_H_
#define CC_OUTPUT_FILTER_OPERATIONS_H_

#include <vector>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "cc/output/filter_operation.h"

namespace base {
class Value;
}

namespace cc {

class CC_EXPORT FilterOperations {
 public:
  FilterOperations();

  FilterOperations(const FilterOperations& other);

  ~FilterOperations();

  FilterOperations& operator=(const FilterOperations& other);

  bool operator==(const FilterOperations& other) const;

  bool operator!=(const FilterOperations& other) const {
    return !(*this == other);
  }

  void Append(const FilterOperation& filter);

  
  void Clear();

  bool IsEmpty() const;

  void GetOutsets(int* top, int* right, int* bottom, int* left) const;
  bool HasFilterThatMovesPixels() const;
  bool HasFilterThatAffectsOpacity() const;
  bool HasReferenceFilter() const;

  size_t size() const {
    return operations_.size();
  }

  const FilterOperation& at(size_t index) const {
    DCHECK_LT(index, size());
    return operations_[index];
  }

  
  
  
  
  
  
  
  
  
  
  FilterOperations Blend(const FilterOperations& from, double progress) const;


  scoped_ptr<base::Value> AsValue() const;

 private:
  std::vector<FilterOperation> operations_;
};

}  

#endif  
