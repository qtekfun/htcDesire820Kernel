// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_PLAYBACK_EXTENSION_H_
#define CHROME_RENDERER_PLAYBACK_EXTENSION_H_

namespace v8 {
class Extension;
}

namespace extensions_v8 {

class PlaybackExtension {
 public:
  static v8::Extension* Get();
};

}  

#endif  
