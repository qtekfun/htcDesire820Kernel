// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_FONT_LIST_H_
#define CONTENT_COMMON_FONT_LIST_H_

#include "base/memory/scoped_ptr.h"

namespace base {
class ListValue;
}

namespace content {

extern const char kFontListSequenceToken[];

scoped_ptr<base::ListValue> GetFontList_SlowBlocking();

}  

#endif  
