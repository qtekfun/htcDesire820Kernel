// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_TEST_TEST_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_TEST_TEST_API_H_

#include "base/values.h"
#include "chrome/browser/extensions/chrome_extension_function.h"

template <typename T> struct DefaultSingletonTraits;

namespace extensions {

class TestExtensionFunction : public ChromeSyncExtensionFunction {
 protected:
  virtual ~TestExtensionFunction();

  
  virtual void Run() OVERRIDE;
};

class TestNotifyPassFunction : public TestExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("test.notifyPass", UNKNOWN)

 protected:
  virtual ~TestNotifyPassFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class TestNotifyFailFunction : public TestExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("test.notifyFail", UNKNOWN)

 protected:
  virtual ~TestNotifyFailFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class TestLogFunction : public TestExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("test.log", UNKNOWN)

 protected:
  virtual ~TestLogFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class TestResetQuotaFunction : public TestExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("test.resetQuota", UNKNOWN)

 protected:
  virtual ~TestResetQuotaFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class TestCreateIncognitoTabFunction : public TestExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("test.createIncognitoTab", UNKNOWN)

 protected:
  virtual ~TestCreateIncognitoTabFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class TestSendMessageFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("test.sendMessage", UNKNOWN)

  
  
  void Reply(const std::string& message);

 protected:
  virtual ~TestSendMessageFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class TestGetConfigFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("test.getConfig", UNKNOWN)

  
  
  static void set_test_config_state(base::DictionaryValue* value);

 protected:
  
  
  
  
  class TestConfigState {
   public:
    static TestConfigState* GetInstance();

    void set_config_state(base::DictionaryValue* config_state) {
      config_state_ = config_state;
    }

    const base::DictionaryValue* config_state() {
      return config_state_;
    }

   private:
    friend struct DefaultSingletonTraits<TestConfigState>;
    TestConfigState();

    base::DictionaryValue* config_state_;

    DISALLOW_COPY_AND_ASSIGN(TestConfigState);
  };

  virtual ~TestGetConfigFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  
