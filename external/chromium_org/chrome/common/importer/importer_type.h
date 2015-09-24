// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_IMPORTER_IMPORTER_TYPE_H_
#define CHROME_COMMON_IMPORTER_IMPORTER_TYPE_H_

#include "build/build_config.h"

namespace importer {

enum ImporterType {
  TYPE_UNKNOWN         = -1,
#if defined(OS_WIN)
  TYPE_IE              = 0,
#endif
  
  TYPE_FIREFOX         = 2,
#if defined(OS_MACOSX)
  TYPE_SAFARI          = 3,
#endif
  
  TYPE_BOOKMARKS_FILE  = 5  
};

}  


#endif  
