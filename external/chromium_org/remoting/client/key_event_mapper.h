// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_KEY_EVENT_MAPPER_H_
#define REMOTING_CLIENT_KEY_EVENT_MAPPER_H_

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "remoting/protocol/input_filter.h"

namespace remoting {

class KeyEventMapper : public protocol::InputFilter {
 public:
  KeyEventMapper();
  explicit KeyEventMapper(InputStub* input_stub);
  virtual ~KeyEventMapper();

  
  typedef base::Callback<void(const protocol::KeyEvent&)> KeyTrapCallback;

  
  void SetTrapCallback(KeyTrapCallback callback);

  
  
  void TrapKey(uint32 usb_keycode, bool trap_key);

  
  
  void RemapKey(uint32 in_usb_keycode, uint32 out_usb_keycode);

  
  virtual void InjectKeyEvent(const protocol::KeyEvent& event) OVERRIDE;

 private:
  std::map<uint32,uint32> mapped_keys;
  std::set<uint32> trapped_keys;
  KeyTrapCallback trap_callback;

  DISALLOW_COPY_AND_ASSIGN(KeyEventMapper);
};

} 

#endif 
