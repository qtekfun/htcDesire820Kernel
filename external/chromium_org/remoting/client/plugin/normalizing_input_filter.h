// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_PLUGIN_NORMALIZING_INPUT_FILTER_H_
#define REMOTING_CLIENT_PLUGIN_NORMALIZING_INPUT_FILTER_H_

#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/input_filter.h"

namespace remoting {

scoped_ptr<protocol::InputFilter> CreateNormalizingInputFilter(
    protocol::InputStub* input_stub);

}  

#endif  
