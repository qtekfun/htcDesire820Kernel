// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_CLIPBOARD_H_
#define REMOTING_HOST_CLIPBOARD_H_

#include <string>

#include "base/callback.h"

namespace remoting {

namespace protocol {
class ClipboardEvent;
class ClipboardStub;
}  

class Clipboard {
 public:
  virtual ~Clipboard() {}

  
  virtual void Start(scoped_ptr<protocol::ClipboardStub> client_clipboard) = 0;

  
  virtual void Stop() = 0;

  
  virtual void InjectClipboardEvent(const protocol::ClipboardEvent& event) = 0;

  static scoped_ptr<Clipboard> Create();
};

}  

#endif  
