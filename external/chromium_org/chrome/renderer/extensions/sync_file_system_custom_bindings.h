// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_SYNC_FILE_SYSTEM_CUSTOM_BINDINGS_H_
#define CHROME_RENDERER_EXTENSIONS_SYNC_FILE_SYSTEM_CUSTOM_BINDINGS_H_

#include "chrome/renderer/extensions/chrome_v8_extension.h"
#include "v8/include/v8.h"

namespace extensions {

class SyncFileSystemCustomBindings : public ChromeV8Extension {
 public:
  SyncFileSystemCustomBindings(Dispatcher* dispatcher,
                               ChromeV8Context* context);

 private:
  
  
  void GetSyncFileSystemObject(const v8::FunctionCallbackInfo<v8::Value>& args);

  DISALLOW_COPY_AND_ASSIGN(SyncFileSystemCustomBindings);
};

}  

#endif  
