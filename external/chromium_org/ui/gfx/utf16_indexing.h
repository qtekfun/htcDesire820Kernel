// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_UTF16_INDEXING_H_
#define UI_GFX_UTF16_INDEXING_H_

#include "base/strings/string16.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {

GFX_EXPORT bool IsValidCodePointIndex(const base::string16& s, size_t index);

GFX_EXPORT ptrdiff_t UTF16IndexToOffset(const base::string16& s,
                                        size_t base,
                                        size_t pos);
GFX_EXPORT size_t UTF16OffsetToIndex(const base::string16& s,
                                     size_t base,
                                     ptrdiff_t offset);

}  

#endif  
