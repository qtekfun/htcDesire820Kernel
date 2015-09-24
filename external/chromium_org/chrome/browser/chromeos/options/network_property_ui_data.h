// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_OPTIONS_NETWORK_PROPERTY_UI_DATA_H_
#define CHROME_BROWSER_CHROMEOS_OPTIONS_NETWORK_PROPERTY_UI_DATA_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "components/onc/onc_constants.h"

namespace base {
class DictionaryValue;
class Value;
}

namespace chromeos {

class NetworkPropertyUIData {
 public:
  
  NetworkPropertyUIData();

  
  explicit NetworkPropertyUIData(::onc::ONCSource onc_source);

  ~NetworkPropertyUIData();

  
  
  void ParseOncProperty(::onc::ONCSource onc_source,
                        const base::DictionaryValue* onc,
                        const std::string& property_key);

  const base::Value* default_value() const { return default_value_.get(); }
  bool IsManaged() const {
    return (onc_source_ == ::onc::ONC_SOURCE_DEVICE_POLICY ||
            onc_source_ == ::onc::ONC_SOURCE_USER_POLICY);
  }
  bool IsEditable() const { return !IsManaged(); }

 private:
  ::onc::ONCSource onc_source_;
  scoped_ptr<base::Value> default_value_;

  DISALLOW_COPY_AND_ASSIGN(NetworkPropertyUIData);
};

}  

#endif  
