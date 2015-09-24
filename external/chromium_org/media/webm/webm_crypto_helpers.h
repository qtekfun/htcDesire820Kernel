// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_WEBM_WEBM_CRYPTO_HELPERS_H_
#define MEDIA_WEBM_WEBM_CRYPTO_HELPERS_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "media/base/decoder_buffer.h"

namespace media {

const char kWebMEncryptInitDataType[] = "video/webm";

scoped_ptr<DecryptConfig> WebMCreateDecryptConfig(
    const uint8* data, int data_size,
    const uint8* key_id, int key_id_size);

}  

#endif  
