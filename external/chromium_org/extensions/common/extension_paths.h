// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_EXTENSION_PATHS_H_
#define EXTENSIONS_COMMON_EXTENSION_PATHS_H_


namespace extensions {

enum {
  PATH_START = 6000,

  
  DIR_TEST_DATA,

  PATH_END
};

void RegisterPathProvider();

}  

#endif  
