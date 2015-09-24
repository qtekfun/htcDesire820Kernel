// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_SHILL_CLIENT_UNITTEST_BASE_H_
#define CHROMEOS_DBUS_SHILL_CLIENT_UNITTEST_BASE_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "chromeos/dbus/shill_client_helper.h"
#include "chromeos/dbus/shill_property_changed_observer.h"
#include "dbus/mock_bus.h"
#include "dbus/mock_object_proxy.h"
#include "dbus/object_proxy.h"
#include "testing/gtest/include/gtest/gtest.h"

using ::testing::MatcherInterface;
using ::testing::MatchResultListener;
using ::testing::Matcher;
using ::testing::MakeMatcher;

namespace base {

class Value;
class DictionaryValue;

}  

namespace dbus {

class MessageReader;

}  

namespace chromeos {

class ValueMatcher : public MatcherInterface<const base::Value&> {
 public:
  explicit ValueMatcher(const base::Value& value);

  
  virtual bool MatchAndExplain(const base::Value& value,
                               MatchResultListener* listener) const OVERRIDE;
  virtual void DescribeTo(::std::ostream* os) const OVERRIDE;
  virtual void DescribeNegationTo(::std::ostream* os) const OVERRIDE;

 private:
  scoped_ptr<base::Value> expected_value_;
};

inline Matcher<const base::Value&> ValueEq(const base::Value& expected_value) {
  return MakeMatcher(new ValueMatcher(expected_value));
}

class ShillClientUnittestBase : public testing::Test {
 public:
  
  class MockClosure {
   public:
    MockClosure();
    ~MockClosure();
    MOCK_METHOD0(Run, void());
    base::Closure GetCallback();
  };

  class MockListValueCallback {
   public:
    MockListValueCallback();
    ~MockListValueCallback();
    MOCK_METHOD1(Run, void(const base::ListValue& list));
    ShillClientHelper::ListValueCallback GetCallback();
  };

  
  class MockErrorCallback {
   public:
    MockErrorCallback();
    ~MockErrorCallback();
    MOCK_METHOD2(Run, void(const std::string& error_name,
                           const std::string& error_message));
    ShillClientHelper::ErrorCallback GetCallback();
  };

  
  class MockPropertyChangeObserver
      : public ShillPropertyChangedObserver {
   public:
    MockPropertyChangeObserver();
    ~MockPropertyChangeObserver();
    MOCK_METHOD2(OnPropertyChanged, void(const std::string& name,
                                         const base::Value& value));
  };

  explicit ShillClientUnittestBase(const std::string& interface_name,
                                      const dbus::ObjectPath& object_path);
  virtual ~ShillClientUnittestBase();

  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

 protected:
  
  typedef base::Callback<void(
      dbus::MessageReader* reader)> ArgumentCheckCallback;

  
  void PrepareForMethodCall(const std::string& method_name,
                            const ArgumentCheckCallback& argument_checker,
                            dbus::Response* response);

  
  void SendPropertyChangedSignal(dbus::Signal* signal);

  
  static void ExpectPropertyChanged(const std::string& expected_name,
                                    const base::Value* expected_value,
                                    const std::string& name,
                                    const base::Value& value);

  
  static void ExpectNoArgument(dbus::MessageReader* reader);

  
  static void ExpectStringArgument(const std::string& expected_string,
                                   dbus::MessageReader* reader);

  static void ExpectArrayOfStringsArgument(
      const std::vector<std::string>& expected_strings,
      dbus::MessageReader* reader);

  
  static void ExpectValueArgument(const base::Value* expected_value,
                                  dbus::MessageReader* reader);

  
  static void ExpectStringAndValueArguments(const std::string& expected_string,
                                            const base::Value* expected_value,
                                            dbus::MessageReader* reader);

  
  static void ExpectDictionaryValueArgument(
      const base::DictionaryValue* expected_dictionary,
      dbus::MessageReader* reader);

  
  
  static base::DictionaryValue* CreateExampleServiceProperties();

  
  static void ExpectNoResultValue(DBusMethodCallStatus call_status);

  
  static void ExpectObjectPathResult(const dbus::ObjectPath& expected_result,
                                     DBusMethodCallStatus call_status,
                                     const dbus::ObjectPath& result);

  static void ExpectObjectPathResultWithoutStatus(
      const dbus::ObjectPath& expected_result,
      const dbus::ObjectPath& result);

  static void ExpectBoolResultWithoutStatus(
      bool expected_result,
      bool result);

  static void ExpectStringResultWithoutStatus(
      const std::string& expected_result,
      const std::string& result);

  
  static void ExpectDictionaryValueResult(
      const base::DictionaryValue* expected_result,
      DBusMethodCallStatus call_status,
      const base::DictionaryValue& result);

  
  static void ExpectDictionaryValueResultWithoutStatus(
      const base::DictionaryValue* expected_result,
      const base::DictionaryValue& result);

  
  base::MessageLoop message_loop_;
  
  scoped_refptr<dbus::MockBus> mock_bus_;

 private:
  
  
  void OnConnectToSignal(
      const std::string& interface_name,
      const std::string& signal_name,
      const dbus::ObjectProxy::SignalCallback& signal_callback,
      const dbus::ObjectProxy::OnConnectedCallback& on_connected_callback);

  
  
  void OnCallMethod(
      dbus::MethodCall* method_call,
      int timeout_ms,
      const dbus::ObjectProxy::ResponseCallback& response_callback);

  
  
  void OnCallMethodWithErrorCallback(
      dbus::MethodCall* method_call,
      int timeout_ms,
      const dbus::ObjectProxy::ResponseCallback& response_callback,
      const dbus::ObjectProxy::ErrorCallback& error_callback);

  
  const std::string interface_name_;
  
  const dbus::ObjectPath object_path_;
  
  scoped_refptr<dbus::MockObjectProxy> mock_proxy_;
  
  dbus::ObjectProxy::SignalCallback property_changed_handler_;
  
  std::string expected_method_name_;
  
  dbus::Response* response_;
  
  ArgumentCheckCallback argument_checker_;
};

}  

#endif  
