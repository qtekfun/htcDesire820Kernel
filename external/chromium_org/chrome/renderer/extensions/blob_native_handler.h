// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_BLOB_NATIVE_HANDLER_H_
#define CHROME_RENDERER_EXTENSIONS_BLOB_NATIVE_HANDLER_H_

#include "chrome/renderer/extensions/chrome_v8_extension.h"

namespace extensions {

class BlobNativeHandler : public ObjectBackedNativeHandler {
 public:
  explicit BlobNativeHandler(ChromeV8Context* context);
};

}  

#endif  
