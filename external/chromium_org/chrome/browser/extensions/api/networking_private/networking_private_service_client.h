// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_NETWORKING_PRIVATE_NETWORKING_PRIVATE_SERVICE_CLIENT_H_
#define CHROME_BROWSER_EXTENSIONS_API_NETWORKING_PRIVATE_NETWORKING_PRIVATE_SERVICE_CLIENT_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/id_map.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "base/supports_user_data.h"
#include "base/threading/sequenced_worker_pool.h"
#include "base/values.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/wifi/wifi_service.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/utility_process_host.h"
#include "content/public/browser/utility_process_host_client.h"

namespace base {
class SequencedTaskRunner;
}

namespace wifi {
class WiFiService;
}

namespace extensions {

using wifi::WiFiService;

class NetworkingPrivateServiceClient : public BrowserContextKeyedService {
 public:
  
  class CryptoVerify {
   public:
    CryptoVerify() {}
    virtual ~CryptoVerify() {}

    static CryptoVerify* Create();

    virtual void VerifyDestination(scoped_ptr<base::ListValue> args,
                                   bool* verified,
                                   std::string* error) = 0;

    virtual void VerifyAndEncryptData(scoped_ptr<base::ListValue> args,
                                      std::string* base64_encoded_ciphertext,
                                      std::string* error) = 0;
   private:
    DISALLOW_COPY_AND_ASSIGN(CryptoVerify);
  };

  
  class Observer {
   public:
    Observer() {}
    virtual ~Observer() {}

    virtual void OnNetworksChangedEvent(
        const std::vector<std::string>& network_guids) = 0;
    virtual void OnNetworkListChangedEvent(
        const std::vector<std::string>& network_guids) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  
  
  typedef base::Callback<
      void(const std::string& error_name,
           scoped_ptr<base::DictionaryValue> error_data)> ErrorCallback;

  
  
  
  
  typedef base::Callback<
      void(const std::string& error_name,
           const std::string& error)> CryptoErrorCallback;

  
  typedef base::Callback<void(bool result)> BoolResultCallback;

  
  typedef base::Callback<void(const std::string& result)> StringResultCallback;

  
  typedef base::Callback<
      void(const std::string& network_guid,
           const base::DictionaryValue& dictionary)> DictionaryResultCallback;

  
  typedef base::Callback<
      void(const base::ListValue& network_list)> ListResultCallback;

  
  
  
  NetworkingPrivateServiceClient(wifi::WiFiService* wifi_service,
                                 CryptoVerify* crypto_verify);

  
  virtual void Shutdown() OVERRIDE;

  
  
  void GetProperties(const std::string& network_guid,
                     const DictionaryResultCallback& callback,
                     const ErrorCallback& error_callback);

  
  
  
  
  void GetManagedProperties(const std::string& network_guid,
                            const DictionaryResultCallback& callback,
                            const ErrorCallback& error_callback);

  
  
  
  
  
  void GetState(const std::string& network_guid,
                const DictionaryResultCallback& callback,
                const ErrorCallback& error_callback);

  
  
  void StartConnect(const std::string& network_guid,
                    const base::Closure& callback,
                    const ErrorCallback& error_callback);

  
  
  void StartDisconnect(const std::string& network_guid,
                       const base::Closure& callback,
                       const ErrorCallback& error_callback);

  
  
  void SetProperties(const std::string& network_guid,
                     const base::DictionaryValue& properties,
                     const base::Closure& callback,
                     const ErrorCallback& error_callback);

  
  
  
  
  
  void CreateNetwork(bool shared,
                     const base::DictionaryValue& properties,
                     const StringResultCallback& callback,
                     const ErrorCallback& error_callback);

  
  void RequestNetworkScan();

  
  void GetVisibleNetworks(const std::string& network_type,
                          const ListResultCallback& callback);

  
  void VerifyDestination(scoped_ptr<base::ListValue> args,
                         const BoolResultCallback& callback,
                         const CryptoErrorCallback& error_callback);

  
  
  void VerifyAndEncryptData(scoped_ptr<base::ListValue> args,
                            const StringResultCallback& callback,
                            const CryptoErrorCallback& error_callback);

  
  void AddObserver(Observer* network_events_observer);

  
  
  void RemoveObserver(Observer* network_events_observer);

 private:
  
  
  
  typedef int32 ServiceCallbacksID;
  struct ServiceCallbacks {
    ServiceCallbacks();
    ~ServiceCallbacks();

    DictionaryResultCallback get_properties_callback;
    base::Closure start_connect_callback;
    base::Closure start_disconnect_callback;
    base::Closure set_properties_callback;
    StringResultCallback create_network_callback;
    ListResultCallback get_visible_networks_callback;
    ErrorCallback error_callback;

    BoolResultCallback verify_destination_callback;
    StringResultCallback verify_and_encrypt_data_callback;
    CryptoErrorCallback crypto_error_callback;

    ServiceCallbacksID id;
  };
  typedef IDMap<ServiceCallbacks, IDMapOwnPointer> ServiceCallbacksMap;

  virtual ~NetworkingPrivateServiceClient();

  
  void AfterGetProperties(ServiceCallbacksID callback_id,
                          const std::string& network_guid,
                          const DictionaryValue* properties,
                          const std::string* error);
  void AfterSetProperties(ServiceCallbacksID callback_id,
                          const std::string* error);
  void AfterCreateNetwork(ServiceCallbacksID callback_id,
                          const std::string* network_guid,
                          const std::string* error);
  void AfterGetVisibleNetworks(ServiceCallbacksID callback_id,
                               const ListValue* network_list);
  void AfterStartConnect(ServiceCallbacksID callback_id,
                         const std::string* error);
  void AfterStartDisconnect(ServiceCallbacksID callback_id,
                            const std::string* error);
  void AfterVerifyDestination(ServiceCallbacksID callback_id,
                              const bool* result,
                              const std::string* error);
  void AfterVerifyAndEncryptData(ServiceCallbacksID callback_id,
                                 const std::string* result,
                                 const std::string* error);

  void OnNetworksChangedEventOnUIThread(
      const WiFiService::NetworkGuidList& network_guid_list);
  void OnNetworkListChangedEventOnUIThread(
      const WiFiService::NetworkGuidList& network_guid_list);

  
  ServiceCallbacks* AddServiceCallbacks();
  
  void RemoveServiceCallbacks(ServiceCallbacksID callback_id);

  
  ServiceCallbacksMap callbacks_map_;
  
  ObserverList<Observer> network_events_observers_;
  
  scoped_ptr<CryptoVerify> crypto_verify_;
  
  scoped_ptr<wifi::WiFiService> wifi_service_;
  
  base::SequencedWorkerPool::SequenceToken sequence_token_;
  
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  
  base::WeakPtrFactory<NetworkingPrivateServiceClient> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateServiceClient);
};

}  

#endif  
