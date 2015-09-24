// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DBUS_SERVICE_PROVIDER_TEST_HELPER_H_
#define CHROME_BROWSER_CHROMEOS_DBUS_SERVICE_PROVIDER_TEST_HELPER_H_

#include <string>

#include "base/message_loop/message_loop.h"
#include "chrome/browser/chromeos/dbus/cros_dbus_service.h"
#include "dbus/mock_exported_object.h"
#include "dbus/mock_object_proxy.h"
#include "dbus/object_proxy.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace dbus {

class MockBus;

}  

namespace chromeos {

class ServiceProviderTestHelper {
 public:
  ServiceProviderTestHelper();
  ~ServiceProviderTestHelper();

  
  void SetUp(const std::string& exported_method_name,
             CrosDBusService::ServiceProviderInterface* service_provider);

  
  
  void SetUpReturnSignal(
      const std::string& interface_name,
      const std::string& signal_name,
      dbus::ObjectProxy::SignalCallback signal_callback,
      dbus::ObjectProxy::OnConnectedCallback on_connected_callback);

  
  scoped_ptr<dbus::Response> CallMethod(dbus::MethodCall* method_call);

  
  void TearDown();

 private:
  
  void MockExportMethod(
      const std::string& interface_name,
      const std::string& method_name,
      dbus::ExportedObject::MethodCallCallback method_callback,
      dbus::ExportedObject::OnExportedCallback on_exported_callback);

  
  dbus::Response* MockCallMethodAndBlock(
      dbus::MethodCall* method_call,
      ::testing::Unused);

  
  void MockConnectToSignal(
      const std::string& interface_name,
      const std::string& signal_name,
      dbus::ObjectProxy::SignalCallback signal_callback,
      dbus::ObjectProxy::OnConnectedCallback connected_callback);

  
  void MockSendSignal(dbus::Signal* signal);

  
  void OnResponse(scoped_ptr<dbus::Response> response);

  scoped_refptr<dbus::MockBus> mock_bus_;
  scoped_refptr<dbus::MockExportedObject> mock_exported_object_;
  scoped_refptr<dbus::MockObjectProxy> mock_object_proxy_;
  dbus::ExportedObject::MethodCallCallback method_callback_;
  dbus::ObjectProxy::SignalCallback on_signal_callback_;
  base::MessageLoop message_loop_;
  bool response_received_;
  scoped_ptr<dbus::Response> response_;
};

}  

#endif  
