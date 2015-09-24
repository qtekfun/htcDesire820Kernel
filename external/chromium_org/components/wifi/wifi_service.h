// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_WIFI_WIFI_SERVICE_H_
#define CHROME_UTILITY_WIFI_WIFI_SERVICE_H_

#include <list>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/threading/sequenced_worker_pool.h"
#include "base/values.h"
#include "components/wifi/wifi_export.h"

namespace wifi {

class WIFI_EXPORT WiFiService {
 public:
  typedef std::vector<std::string> NetworkGuidList;
  typedef base::Callback<
      void(const NetworkGuidList& network_guid_list)> NetworkGuidListCallback;

  virtual ~WiFiService() {}

  
  virtual void Initialize(
      scoped_refptr<base::SequencedTaskRunner> task_runner) = 0;

  
  virtual void UnInitialize() = 0;

  
  static WiFiService* Create();
  
  static WiFiService* CreateForTest();

  
  
  virtual void GetProperties(const std::string& network_guid,
                             DictionaryValue* properties,
                             std::string* error) = 0;

  
  
  
  
  virtual void GetManagedProperties(const std::string& network_guid,
                                    DictionaryValue* managed_properties,
                                    std::string* error) = 0;

  
  
  
  
  
  virtual void GetState(const std::string& network_guid,
                        DictionaryValue* properties,
                        std::string* error) = 0;

  
  
  virtual void SetProperties(const std::string& network_guid,
                             scoped_ptr<base::DictionaryValue> properties,
                             std::string* error) = 0;

  
  
  
  
  virtual void CreateNetwork(bool shared,
                             scoped_ptr<base::DictionaryValue> properties,
                             std::string* network_guid,
                             std::string* error) = 0;

  
  
  virtual void GetVisibleNetworks(const std::string& network_type,
                                  ListValue* network_list) = 0;

  
  virtual void RequestNetworkScan() = 0;

  
  
  virtual void StartConnect(const std::string& network_guid,
                            std::string* error) = 0;

  
  
  virtual void StartDisconnect(const std::string& network_guid,
                               std::string* error) = 0;

  
  
  virtual void SetEventObservers(
      scoped_refptr<base::MessageLoopProxy> message_loop_proxy,
      const NetworkGuidListCallback& networks_changed_observer,
      const NetworkGuidListCallback& network_list_changed_observer) = 0;

 protected:
  WiFiService() {}

  typedef int32 Frequency;
  enum FrequencyEnum {
    kFrequencyAny = 0,
    kFrequencyUnknown = 0,
    kFrequency2400 = 2400,
    kFrequency5000 = 5000
  };

  typedef std::list<Frequency> FrequencyList;
  
  
  struct WIFI_EXPORT NetworkProperties {
    NetworkProperties();
    ~NetworkProperties();

    std::string connection_state;
    std::string guid;
    std::string name;
    std::string ssid;
    std::string bssid;
    std::string type;
    std::string security;
    
    
    
    std::string password;
    
    uint32 signal_strength;
    bool auto_connect;
    Frequency frequency;
    FrequencyList frequency_list;

    std::string json_extra;  

    scoped_ptr<base::DictionaryValue> ToValue(bool network_list) const;
    
    bool UpdateFromValue(const base::DictionaryValue& value);
    static std::string MacAddressAsString(const uint8 mac_as_int[6]);
    static bool OrderByType(const NetworkProperties& l,
                            const NetworkProperties& r);
  };

  typedef std::list<NetworkProperties> NetworkList;

 private:
  DISALLOW_COPY_AND_ASSIGN(WiFiService);
};

}  

#endif  
