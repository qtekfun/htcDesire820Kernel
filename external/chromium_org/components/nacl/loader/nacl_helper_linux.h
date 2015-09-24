// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_LOADER_NACL_HELPER_LINUX_H_
#define COMPONENTS_NACL_LOADER_NACL_HELPER_LINUX_H_

namespace nacl {


#define kNaClMaxIPCMessageLength 2048

#define kNaClHelperStartupAck "NACLHELPER_OK"

enum NaClZygoteIPCCommand {
  kNaClForkRequest,
  kNaClGetTerminationStatusRequest,
};


#define kNaClZygoteDescriptor 3

} 

#endif  
