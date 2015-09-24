// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_INVALIDATE_TYPE_H_
#define CONTENT_PUBLIC_BROWSER_INVALIDATE_TYPE_H_

namespace content {

enum InvalidateTypes {
  INVALIDATE_TYPE_URL           = 1 << 0,  
  INVALIDATE_TYPE_TAB           = 1 << 1,  
                                           
  INVALIDATE_TYPE_LOAD          = 1 << 2,  
  INVALIDATE_TYPE_PAGE_ACTIONS  = 1 << 3,  
  INVALIDATE_TYPE_TITLE         = 1 << 4,  
};

}

#endif  
