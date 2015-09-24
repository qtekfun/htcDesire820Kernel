// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_CLIPBOARD_FILTER_H_
#define REMOTING_PROTOCOL_CLIPBOARD_FILTER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "remoting/protocol/clipboard_stub.h"

namespace remoting {
namespace protocol {

class ClipboardFilter : public ClipboardStub {
 public:
  ClipboardFilter();
  explicit ClipboardFilter(ClipboardStub* clipboard_stub);
  virtual ~ClipboardFilter();

  
  void set_clipboard_stub(ClipboardStub* clipboard_stub);

  
  void set_enabled(bool enabled) { enabled_ = enabled; }
  bool enabled() const { return enabled_; }

  
  virtual void InjectClipboardEvent(const ClipboardEvent& event) OVERRIDE;

 private:
  ClipboardStub* clipboard_stub_;
  bool enabled_;

  DISALLOW_COPY_AND_ASSIGN(ClipboardFilter);
};

}  
}  

#endif  
