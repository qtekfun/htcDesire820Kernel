// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_PATHS_H_
#define CC_TEST_PATHS_H_

namespace cc {

class CCPaths {
 public:
  enum {
    PATH_START = 5000,

    
    DIR_TEST_DATA,
    PATH_END
  };

  
  static void RegisterPathProvider();
};

}  

#endif  
