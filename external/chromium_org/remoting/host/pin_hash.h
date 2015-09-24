// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_PIN_HASH_H_
#define REMOTING_HOST_PIN_HASH_H_

#include <string>

namespace remoting {

std::string MakeHostPinHash(const std::string& host_id, const std::string& pin);

bool VerifyHostPinHash(const std::string& hash,
                       const std::string& host_id,
                       const std::string& pin);

}  

#endif  
