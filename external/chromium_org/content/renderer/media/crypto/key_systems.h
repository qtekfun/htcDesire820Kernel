// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_CRYPTO_KEY_SYSTEMS_H_
#define CONTENT_RENDERER_MEDIA_CRYPTO_KEY_SYSTEMS_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"

namespace blink {
class WebString;
}

namespace content {

CONTENT_EXPORT bool IsConcreteSupportedKeySystem(
    const blink::WebString& key_system);

CONTENT_EXPORT bool IsSupportedKeySystemWithMediaMimeType(
    const std::string& mime_type,
    const std::vector<std::string>& codecs,
    const std::string& key_system);

CONTENT_EXPORT std::string KeySystemNameForUMA(
    const blink::WebString& key_system);
CONTENT_EXPORT std::string KeySystemNameForUMA(const std::string& key_system);

CONTENT_EXPORT bool CanUseAesDecryptor(const std::string& concrete_key_system);

#if defined(ENABLE_PEPPER_CDMS)
CONTENT_EXPORT std::string GetPepperType(
    const std::string& concrete_key_system);
#elif defined(OS_ANDROID)
CONTENT_EXPORT std::vector<uint8> GetUUID(
    const std::string& concrete_key_system);
#endif

}  

#endif  
