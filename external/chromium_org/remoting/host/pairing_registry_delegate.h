// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_PAIRING_REGISTRY_DELEGATE_H_
#define REMOTING_HOST_PAIRING_REGISTRY_DELEGATE_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/pairing_registry.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {
scoped_ptr<protocol::PairingRegistry::Delegate>
CreatePairingRegistryDelegate();

scoped_refptr<protocol::PairingRegistry> CreatePairingRegistry(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner);

}  

#endif  
