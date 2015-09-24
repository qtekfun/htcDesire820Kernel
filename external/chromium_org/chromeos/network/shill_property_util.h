// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_SHILL_PROPERTY_UTIL_H_
#define CHROMEOS_NETWORK_SHILL_PROPERTY_UTIL_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"

namespace base {
class DictionaryValue;
class Value;
}

namespace chromeos {

class NetworkUIData;

namespace shill_property_util {

CHROMEOS_EXPORT void SetSSID(const std::string ssid,
                             base::DictionaryValue* properties);

CHROMEOS_EXPORT std::string GetSSIDFromProperties(
    const base::DictionaryValue& properties,
    bool* unknown_encoding);

CHROMEOS_EXPORT std::string GetNameFromProperties(
    const std::string& service_path,
    const base::DictionaryValue& properties);

scoped_ptr<NetworkUIData> GetUIDataFromValue(const base::Value& value);

scoped_ptr<NetworkUIData> GetUIDataFromProperties(
    const base::DictionaryValue& shill_dictionary);

void SetUIData(const NetworkUIData& ui_data,
               base::DictionaryValue* shill_dictionary);

bool CopyIdentifyingProperties(const base::DictionaryValue& service_properties,
                               base::DictionaryValue* dest);

bool DoIdentifyingPropertiesMatch(const base::DictionaryValue& properties_a,
                                  const base::DictionaryValue& properties_b);

}  

class CHROMEOS_EXPORT NetworkTypePattern {
 public:
  
  static NetworkTypePattern Default();

  
  static NetworkTypePattern Wireless();

  
  static NetworkTypePattern Mobile();

  
  static NetworkTypePattern NonVirtual();

  
  static NetworkTypePattern Ethernet();

  static NetworkTypePattern WiFi();
  static NetworkTypePattern Cellular();
  static NetworkTypePattern VPN();
  static NetworkTypePattern Wimax();

  
  
  
  
  
  static NetworkTypePattern Primitive(const std::string& shill_network_type);

  bool Equals(const NetworkTypePattern& other) const;
  bool MatchesType(const std::string& shill_network_type) const;

  
  
  
  
  bool MatchesPattern(const NetworkTypePattern& other_pattern) const;

  std::string ToDebugString() const;

 private:
  explicit NetworkTypePattern(int pattern);

  
  int pattern_;

  DISALLOW_ASSIGN(NetworkTypePattern);
};

}  

#endif  
