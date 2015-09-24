// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_SKIA_UTILS_BASE_H_
#define SKIA_EXT_SKIA_UTILS_BASE_H_

#include "base/pickle.h"
#include "third_party/skia/include/ports/SkFontConfigInterface.h"

namespace skia {

SK_API bool ReadSkString(const Pickle& pickle, PickleIterator* iter,
                         SkString* str);

SK_API bool ReadSkFontIdentity(const Pickle& pickle, PickleIterator* iter,
                               SkFontConfigInterface::FontIdentity* identity);

// Return true if str can be written into the request pickle.
SK_API bool WriteSkString(Pickle* pickle, const SkString& str);

// Return true if identity can be written into the request pickle.
SK_API bool WriteSkFontIdentity(Pickle* pickle,
                          const SkFontConfigInterface::FontIdentity& identity);

}  

#endif  

