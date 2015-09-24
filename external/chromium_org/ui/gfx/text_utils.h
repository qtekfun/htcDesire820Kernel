// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_TEXT_UTILS_H_
#define UI_GFX_TEXT_UTILS_H_

#include "base/strings/string16.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {

class FontList;

GFX_EXPORT base::string16 RemoveAcceleratorChar(const base::string16& s,
                                                base::char16 accelerator_char,
                                                int* accelerated_char_pos,
                                                int* accelerated_char_span);

GFX_EXPORT int GetStringWidth(const base::string16& text,
                              const FontList& font_list);

GFX_EXPORT float GetStringWidthF(const base::string16& text,
                                 const FontList& font_list);

}  

#endif  
