// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PRINTED_PAGES_SOURCE_H_
#define PRINTING_PRINTED_PAGES_SOURCE_H_

#include "base/strings/string16.h"

namespace printing {

class PrintedPagesSource {
 public:
  
  virtual base::string16 RenderSourceName() = 0;

 protected:
  virtual ~PrintedPagesSource() {}
};

}  

#endif  
