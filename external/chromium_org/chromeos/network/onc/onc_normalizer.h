// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_ONC_ONC_NORMALIZER_H_
#define CHROMEOS_NETWORK_ONC_ONC_NORMALIZER_H_

#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/network/onc/onc_mapper.h"

namespace chromeos {
namespace onc {

struct OncValueSignature;

class CHROMEOS_EXPORT Normalizer : public Mapper {
 public:
  explicit Normalizer(bool remove_recommended_fields);
  virtual ~Normalizer();

  
  
  
  
  
  
  scoped_ptr<base::DictionaryValue> NormalizeObject(
      const OncValueSignature* object_signature,
      const base::DictionaryValue& onc_object);

 private:
  
  virtual scoped_ptr<base::DictionaryValue> MapObject(
      const OncValueSignature& signature,
      const base::DictionaryValue& onc_object,
      bool* error) OVERRIDE;

  void NormalizeCertificate(base::DictionaryValue* cert);
  void NormalizeEAP(base::DictionaryValue* eap);
  void NormalizeEthernet(base::DictionaryValue* ethernet);
  void NormalizeIPsec(base::DictionaryValue* ipsec);
  void NormalizeNetworkConfiguration(base::DictionaryValue* network);
  void NormalizeOpenVPN(base::DictionaryValue* openvpn);
  void NormalizeProxySettings(base::DictionaryValue* proxy);
  void NormalizeVPN(base::DictionaryValue* vpn);
  void NormalizeWiFi(base::DictionaryValue* wifi);

  const bool remove_recommended_fields_;

  DISALLOW_COPY_AND_ASSIGN(Normalizer);
};

}  
}  

#endif  
