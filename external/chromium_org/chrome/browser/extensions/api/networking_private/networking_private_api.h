// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_NETWORKING_PRIVATE_NETWORKING_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_NETWORKING_PRIVATE_NETWORKING_PRIVATE_API_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/values.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class NetworkingPrivateGetPropertiesFunction
    : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateGetPropertiesFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.getProperties",
                             NETWORKINGPRIVATE_GETPROPERTIES);

 protected:
  virtual ~NetworkingPrivateGetPropertiesFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void GetPropertiesSuccess(const std::string& service_path,
                            const base::DictionaryValue& result);
  void GetPropertiesFailed(const std::string& error_name,
                           scoped_ptr<base::DictionaryValue> error_data);
  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateGetPropertiesFunction);
};

class NetworkingPrivateGetManagedPropertiesFunction
    : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateGetManagedPropertiesFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.getManagedProperties",
                             NETWORKINGPRIVATE_GETMANAGEDPROPERTIES);

 protected:
  virtual ~NetworkingPrivateGetManagedPropertiesFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void Success(const std::string& service_path,
               const base::DictionaryValue& result);
  void Failure(const std::string& error_name,
              scoped_ptr<base::DictionaryValue> error_data);

  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateGetManagedPropertiesFunction);
};

class NetworkingPrivateGetStateFunction : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateGetStateFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.getState",
                             NETWORKINGPRIVATE_GETSTATE);

 protected:
  virtual ~NetworkingPrivateGetStateFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void Success(const std::string& service_path,
               const base::DictionaryValue& result);
  void Failure(const std::string& error_name,
               scoped_ptr<base::DictionaryValue> error_data);

  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateGetStateFunction);
};

class NetworkingPrivateSetPropertiesFunction
    : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateSetPropertiesFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.setProperties",
                             NETWORKINGPRIVATE_SETPROPERTIES);

 protected:
  virtual ~NetworkingPrivateSetPropertiesFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void ErrorCallback(const std::string& error_name,
                     const scoped_ptr<base::DictionaryValue> error_data);
  void ResultCallback();
  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateSetPropertiesFunction);
};

class NetworkingPrivateCreateNetworkFunction
    : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateCreateNetworkFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.createNetwork",
                             NETWORKINGPRIVATE_CREATENETWORK);

 protected:
  virtual ~NetworkingPrivateCreateNetworkFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void ErrorCallback(const std::string& error_name,
                     const scoped_ptr<base::DictionaryValue> error_data);
  void ResultCallback(const std::string& guid);
  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateCreateNetworkFunction);
};

class NetworkingPrivateGetVisibleNetworksFunction
    : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateGetVisibleNetworksFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.getVisibleNetworks",
                             NETWORKINGPRIVATE_GETVISIBLENETWORKS);

 protected:
  virtual ~NetworkingPrivateGetVisibleNetworksFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void ResultCallback(const base::ListValue& network_list);

  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateGetVisibleNetworksFunction);
};

class NetworkingPrivateGetEnabledNetworkTypesFunction
    : public ChromeSyncExtensionFunction {
 public:
  NetworkingPrivateGetEnabledNetworkTypesFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.getEnabledNetworkTypes",
                             NETWORKINGPRIVATE_GETENABLEDNETWORKTYPES);

 protected:
  virtual ~NetworkingPrivateGetEnabledNetworkTypesFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateGetEnabledNetworkTypesFunction);
};

class NetworkingPrivateEnableNetworkTypeFunction
    : public ChromeSyncExtensionFunction {
 public:
  NetworkingPrivateEnableNetworkTypeFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.enableNetworkType",
                             NETWORKINGPRIVATE_ENABLENETWORKTYPE);

 protected:
  virtual ~NetworkingPrivateEnableNetworkTypeFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateEnableNetworkTypeFunction);
};

class NetworkingPrivateDisableNetworkTypeFunction
    : public ChromeSyncExtensionFunction {
 public:
  NetworkingPrivateDisableNetworkTypeFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.disableNetworkType",
                             NETWORKINGPRIVATE_DISABLENETWORKTYPE);

 protected:
  virtual ~NetworkingPrivateDisableNetworkTypeFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateDisableNetworkTypeFunction);
};

class NetworkingPrivateRequestNetworkScanFunction
    : public ChromeSyncExtensionFunction {
 public:
  NetworkingPrivateRequestNetworkScanFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.requestNetworkScan",
                             NETWORKINGPRIVATE_REQUESTNETWORKSCAN);

 protected:
  virtual ~NetworkingPrivateRequestNetworkScanFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateRequestNetworkScanFunction);
};


class NetworkingPrivateStartConnectFunction
    : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateStartConnectFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.startConnect",
                             NETWORKINGPRIVATE_STARTCONNECT);

 protected:
  virtual ~NetworkingPrivateStartConnectFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  
  void ConnectionStartSuccess();

  void ConnectionStartFailed(
      const std::string& error_name,
      const scoped_ptr<base::DictionaryValue> error_data);

  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateStartConnectFunction);
};

class NetworkingPrivateStartDisconnectFunction
    : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateStartDisconnectFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.startDisconnect",
                             NETWORKINGPRIVATE_STARTDISCONNECT);

 protected:
  virtual ~NetworkingPrivateStartDisconnectFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  
  void DisconnectionStartSuccess();

  void DisconnectionStartFailed(
      const std::string& error_name,
      const scoped_ptr<base::DictionaryValue> error_data);

  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateStartDisconnectFunction);
};

class NetworkingPrivateVerifyDestinationFunction
    : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateVerifyDestinationFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.verifyDestination",
                             NETWORKINGPRIVATE_VERIFYDESTINATION);

 protected:
  virtual ~NetworkingPrivateVerifyDestinationFunction();

  
  virtual bool RunImpl() OVERRIDE;

  void ResultCallback(bool result);
  void ErrorCallback(const std::string& error_name, const std::string& error);

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateVerifyDestinationFunction);
};

class NetworkingPrivateVerifyAndEncryptCredentialsFunction
    : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateVerifyAndEncryptCredentialsFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.verifyAndEncryptCredentials",
                             NETWORKINGPRIVATE_VERIFYANDENCRYPTCREDENTIALS);

 protected:
  virtual ~NetworkingPrivateVerifyAndEncryptCredentialsFunction();

  
  virtual bool RunImpl() OVERRIDE;

  void ResultCallback(const std::string& result);
  void ErrorCallback(const std::string& error_name, const std::string& error);

 private:
  DISALLOW_COPY_AND_ASSIGN(
      NetworkingPrivateVerifyAndEncryptCredentialsFunction);
};

class NetworkingPrivateVerifyAndEncryptDataFunction
    : public ChromeAsyncExtensionFunction {
 public:
  NetworkingPrivateVerifyAndEncryptDataFunction() {}
  DECLARE_EXTENSION_FUNCTION("networkingPrivate.verifyAndEncryptData",
                             NETWORKINGPRIVATE_VERIFYANDENCRYPTDATA);

 protected:
  virtual ~NetworkingPrivateVerifyAndEncryptDataFunction();

  
  virtual bool RunImpl() OVERRIDE;

  void ResultCallback(const std::string& result);
  void ErrorCallback(const std::string& error_name, const std::string& error);

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateVerifyAndEncryptDataFunction);
};

#endif  
