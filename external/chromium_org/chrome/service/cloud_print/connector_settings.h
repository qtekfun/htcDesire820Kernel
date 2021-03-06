// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_CLOUD_PRINT_CONNECTOR_SETTINGS_H_
#define CHROME_SERVICE_CLOUD_PRINT_CONNECTOR_SETTINGS_H_

#include <set>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "url/gurl.h"

class ServiceProcessPrefs;

namespace base {
  class DictionaryValue;
}

namespace cloud_print {

class ConnectorSettings {
 public:
  ConnectorSettings();
  ~ConnectorSettings();

  void InitFrom(ServiceProcessPrefs* prefs);

  void CopyFrom(const ConnectorSettings& source);

  const GURL& server_url() const {
    return server_url_;
  };

  const std::string& proxy_id() const {
    return proxy_id_;
  }

  bool delete_on_enum_fail() const {
    return delete_on_enum_fail_;
  }

  bool xmpp_ping_enabled() const {
    return xmpp_ping_enabled_;
  }

  int xmpp_ping_timeout_sec() const {
    return xmpp_ping_timeout_sec_;
  }

  const base::DictionaryValue* print_system_settings() const {
    return print_system_settings_.get();
  };

  bool ShouldConnect(const std::string& printer_name) const;

  void SetXmppPingTimeoutSec(int timeout);

 private:
  friend class ConnectorSettingsTest;
  FRIEND_TEST_ALL_PREFIXES(ConnectorSettingsTest, SettersTest);

  void set_xmpp_ping_enabled(bool enabled) {
    xmpp_ping_enabled_ = enabled;
  }

  
  GURL server_url_;

  
  
  std::string proxy_id_;

  
  
  bool delete_on_enum_fail_;

  
  bool connect_new_printers_;

  
  bool xmpp_ping_enabled_;

  
  int xmpp_ping_timeout_sec_;

  
  typedef std::set<std::string> Printers;
  Printers printers_;

  
  scoped_ptr<base::DictionaryValue> print_system_settings_;

  DISALLOW_COPY_AND_ASSIGN(ConnectorSettings);
};

}  

#endif  

