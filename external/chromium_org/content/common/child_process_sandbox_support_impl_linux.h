// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_CHILD_PROCESS_SANDBOX_SUPPORT_IMPL_LINUX_H_
#define CONTENT_COMMON_CHILD_PROCESS_SANDBOX_SUPPORT_IMPL_LINUX_H_

#include "base/posix/global_descriptors.h"
#include "content/public/common/child_process_sandbox_support_linux.h"
#include "content/public/common/content_descriptors.h"

namespace blink {
struct WebFontFamily;
struct WebFontRenderStyle;
}

namespace content {

void GetFontFamilyForCharacter(const int32_t character,
                               const char* preferred_locale,
                               blink::WebFontFamily* family);

void GetRenderStyleForStrike(const char* family, int sizeAndStyle,
                             blink::WebFontRenderStyle* out);

inline int GetSandboxFD() {
  return kSandboxIPCChannel + base::GlobalDescriptors::kBaseDescriptor;
}

};  

#endif  
