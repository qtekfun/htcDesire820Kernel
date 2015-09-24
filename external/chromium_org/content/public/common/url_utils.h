// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_URL_UTILS_H_
#define CONTENT_PUBLIC_COMMON_URL_UTILS_H_

#include <stddef.h>         

#include "build/build_config.h"
#include "content/common/content_export.h"

class GURL;

namespace content {

CONTENT_EXPORT const char* const* GetSavableSchemes();

CONTENT_EXPORT bool HasWebUIScheme(const GURL& url);

CONTENT_EXPORT bool IsSavableURL(const GURL& url);

#if defined(OS_ANDROID)
CONTENT_EXPORT void SetMaxURLChars(size_t max_chars);
#endif

CONTENT_EXPORT size_t GetMaxURLChars();

}  

#endif  
