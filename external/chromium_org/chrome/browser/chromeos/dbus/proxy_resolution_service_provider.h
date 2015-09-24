// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DBUS_PROXY_RESOLUTION_SERVICE_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_DBUS_PROXY_RESOLUTION_SERVICE_PROVIDER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"
#include "chrome/browser/chromeos/dbus/cros_dbus_service.h"
#include "dbus/exported_object.h"

namespace dbus {
class MethodCall;
class Response;
}

namespace chromeos {

class ProxyResolverInterface;


class ProxyResolutionServiceProvider
    : public CrosDBusService::ServiceProviderInterface {
 public:
  virtual ~ProxyResolutionServiceProvider();

  
  virtual void Start(
      scoped_refptr<dbus::ExportedObject> exported_object) OVERRIDE;

  
  static ProxyResolutionServiceProvider* Create();

 private:
  explicit ProxyResolutionServiceProvider(ProxyResolverInterface *resovler);

  
  friend class ProxyResolutionServiceProviderTest;
  static ProxyResolutionServiceProvider* CreateForTesting(
      ProxyResolverInterface* resolver);

  
  
  void OnExported(const std::string& interface_name,
                  const std::string& method_name,
                  bool success);

  
  
  
  void ResolveProxyHandler(dbus::MethodCall* method_call,
      dbus::ExportedObject::ResponseSender response_sender);

  
  
  static void CallResolveProxyHandler(
      base::WeakPtr<ProxyResolutionServiceProvider> weak_ptr,
      dbus::MethodCall* method_call,
      dbus::ExportedObject::ResponseSender response_sender);

  
  bool OnOriginThread();

  scoped_refptr<dbus::ExportedObject> exported_object_;
  scoped_ptr<ProxyResolverInterface> resolver_;
  base::PlatformThreadId origin_thread_id_;
  base::WeakPtrFactory<ProxyResolutionServiceProvider> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ProxyResolutionServiceProvider);
};

class ProxyResolverInterface {
 public:
  
  
  
  
  
  
  
  
  virtual void ResolveProxy(
      const std::string& source_url,
      const std::string& signal_interface,
      const std::string& signal_name,
      scoped_refptr<dbus::ExportedObject> exported_object) = 0;

  virtual ~ProxyResolverInterface();
};

}  

#endif  
