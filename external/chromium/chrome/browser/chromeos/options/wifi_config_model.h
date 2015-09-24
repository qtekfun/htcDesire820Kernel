// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_OPTIONS_WIFI_CONFIG_MODEL_H_
#define CHROME_BROWSER_CHROMEOS_OPTIONS_WIFI_CONFIG_MODEL_H_
#pragma once

#include <string>

#include "base/string16.h"
#include "net/base/cert_database.h"

namespace chromeos {

class WifiConfigModel {
 public:
  
  
  
  WifiConfigModel();
  ~WifiConfigModel();

  
  void UpdateCertificates();

  
  int GetUserCertCount() const;

  
  string16 GetUserCertName(int cert_index) const;

  
  std::string GetUserCertPkcs11Id(int cert_index) const;

  
  
  int GetUserCertIndex(const std::string& pkcs11_id) const;

  
  int GetServerCaCertCount() const;

  
  string16 GetServerCaCertName(int cert_index) const;

  
  std::string GetServerCaCertNssNickname(int cert_index) const;

  
  
  int GetServerCaCertIndex(const std::string& nss_nickname) const;

 private:
  net::CertDatabase cert_db_;

  
  net::CertificateList user_certs_;

  
  net::CertificateList server_ca_certs_;

  DISALLOW_COPY_AND_ASSIGN(WifiConfigModel);
};

}  

#endif  
