// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_WEBCRYPTO_WEBCRYPTO_UTIL_H_
#define CONTENT_RENDERER_WEBCRYPTO_WEBCRYPTO_UTIL_H_

#include <map>
#include <string>
#include <vector>
#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/platform/WebArrayBuffer.h"
#include "third_party/WebKit/public/platform/WebCryptoAlgorithm.h"

namespace content {

namespace webcrypto {

CONTENT_EXPORT const uint8* Uint8VectorStart(const std::vector<uint8>& data);

void ShrinkBuffer(blink::WebArrayBuffer* buffer, unsigned new_size);

blink::WebArrayBuffer CreateArrayBuffer(const uint8* data, unsigned data_size);

bool Base64DecodeUrlSafe(const std::string& input, std::string* output);

blink::WebCryptoAlgorithm GetInnerHashAlgorithm(
    const blink::WebCryptoAlgorithm& algorithm);

CONTENT_EXPORT blink::WebCryptoAlgorithm CreateAlgorithm(
    blink::WebCryptoAlgorithmId id);

blink::WebCryptoAlgorithm CreateHmacAlgorithmByHashOutputLen(
    unsigned short hash_output_length_bits);

CONTENT_EXPORT blink::WebCryptoAlgorithm CreateHmacAlgorithmByHashId(
    blink::WebCryptoAlgorithmId hash_id);

CONTENT_EXPORT blink::WebCryptoAlgorithm CreateHmacKeyGenAlgorithm(
    blink::WebCryptoAlgorithmId hash_id,
    unsigned key_length_bytes);

blink::WebCryptoAlgorithm CreateRsaSsaAlgorithm(
    blink::WebCryptoAlgorithmId hash_id);

blink::WebCryptoAlgorithm CreateRsaOaepAlgorithm(
    blink::WebCryptoAlgorithmId hash_id);

CONTENT_EXPORT blink::WebCryptoAlgorithm CreateRsaKeyGenAlgorithm(
    blink::WebCryptoAlgorithmId algorithm_id,
    unsigned modulus_length,
    const std::vector<uint8>& public_exponent);

CONTENT_EXPORT blink::WebCryptoAlgorithm CreateAesCbcAlgorithm(
    const std::vector<uint8>& iv);

blink::WebCryptoAlgorithm CreateAesGcmAlgorithm(
    const std::vector<uint8>& iv,
    const std::vector<uint8>& additional_data,
    uint8 tag_length_bytes);

CONTENT_EXPORT blink::WebCryptoAlgorithm CreateAesCbcKeyGenAlgorithm(
    unsigned short key_length_bits);

blink::WebCryptoAlgorithm CreateAesGcmKeyGenAlgorithm(
    unsigned short key_length_bits);

}  

}  

#endif  
