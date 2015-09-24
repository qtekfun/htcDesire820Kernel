// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_PAIRING_REGISTRY_DELEGATE_WIN_H_
#define REMOTING_PROTOCOL_PAIRING_REGISTRY_DELEGATE_WIN_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/win/registry.h"
#include "remoting/protocol/pairing_registry.h"

namespace base {
class ListValue;
}  

namespace remoting {

class PairingRegistryDelegateWin
    : public protocol::PairingRegistry::Delegate {
 public:
  PairingRegistryDelegateWin();
  virtual ~PairingRegistryDelegateWin();

  
  
  
  bool SetRootKeys(HKEY privileged, HKEY unprivileged);

  
  virtual scoped_ptr<base::ListValue> LoadAll() OVERRIDE;
  virtual bool DeleteAll() OVERRIDE;
  virtual protocol::PairingRegistry::Pairing Load(
      const std::string& client_id) OVERRIDE;
  virtual bool Save(const protocol::PairingRegistry::Pairing& pairing) OVERRIDE;
  virtual bool Delete(const std::string& client_id) OVERRIDE;

 private:
  base::win::RegKey privileged_;
  base::win::RegKey unprivileged_;

  DISALLOW_COPY_AND_ASSIGN(PairingRegistryDelegateWin);
};

}  

#endif  
