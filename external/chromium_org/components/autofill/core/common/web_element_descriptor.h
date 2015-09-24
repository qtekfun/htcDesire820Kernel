// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_COMMON_WEB_ELEMENT_DESCRIPTOR_H_
#define COMPONENTS_AUTOFILL_CORE_COMMON_WEB_ELEMENT_DESCRIPTOR_H_

#include <string>

namespace autofill {

struct WebElementDescriptor {
  enum RetrievalMethod {
    CSS_SELECTOR,
    ID,
    NONE,
  };

  WebElementDescriptor();

  
  std::string descriptor;

  
  RetrievalMethod retrieval_method;
};

}  

#endif  
