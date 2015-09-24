// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_SAVE_PAGE_TYPE_H_
#define CONTENT_PUBLIC_BROWSER_SAVE_PAGE_TYPE_H_

namespace content {

enum SavePageType {
  
  SAVE_PAGE_TYPE_UNKNOWN = -1,
  
  SAVE_PAGE_TYPE_AS_ONLY_HTML = 0,
  
  SAVE_PAGE_TYPE_AS_COMPLETE_HTML = 1,
  
  SAVE_PAGE_TYPE_AS_MHTML = 2,

  
  SAVE_PAGE_TYPE_MAX,
};

}

#endif  
