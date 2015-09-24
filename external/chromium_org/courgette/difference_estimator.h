// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef COURGETTE_DIFFERENCE_ESTIMATOR_H_
#define COURGETTE_DIFFERENCE_ESTIMATOR_H_

#include <vector>

#include "courgette/region.h"

namespace courgette {

class DifferenceEstimator {
 public:
  DifferenceEstimator();
  ~DifferenceEstimator();

  class Base;
  class Subject;

  
  
  
  Base* MakeBase(const Region& region);
  Subject* MakeSubject(const Region& region);

  
  
  
  size_t Measure(Base* base,  Subject* subject);

 private:
  std::vector<Base*> owned_bases_;
  std::vector<Subject*> owned_subjects_;
  DISALLOW_COPY_AND_ASSIGN(DifferenceEstimator);
};

}  

#endif  
