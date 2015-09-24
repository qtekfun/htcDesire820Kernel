// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_RENDERER_PRINCIPALS_EXTENSION_BINDINGS_H_
#define CHROME_RENDERER_PRINCIPALS_EXTENSION_BINDINGS_H_

namespace v8 {
class Extension;
}

namespace extensions_v8 {

class PrincipalsExtension {
 public:
  static v8::Extension* Get();
};

}  

#endif  