// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_APITEST_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_APITEST_H_
#pragma once

#include <deque>
#include <string>

#include "base/values.h"
#include "chrome/browser/extensions/extension_browsertest.h"
#include "content/common/notification_registrar.h"

class Extension;


class ExtensionApiTest : public ExtensionBrowserTest {
 public:
  ExtensionApiTest();
  virtual ~ExtensionApiTest();

 protected:
  
  
  
  
  
  class ResultCatcher : public NotificationObserver {
   public:
    ResultCatcher();
    ~ResultCatcher();

    
    
    bool GetNextResult();

    void RestrictToProfile(Profile* profile) { profile_restriction_ = profile; }

    const std::string& message() { return message_; }

   private:
    virtual void Observe(NotificationType type,
                         const NotificationSource& source,
                         const NotificationDetails& details);

    NotificationRegistrar registrar_;

    
    std::deque<bool> results_;

    
    std::deque<std::string> messages_;
    std::string message_;

    
    Profile* profile_restriction_;

    
    
    bool waiting_;
  };

  virtual void SetUpInProcessBrowserTestFixture();
  virtual void TearDownInProcessBrowserTestFixture();

  
  
  bool RunExtensionTest(const char* extension_name);

  
  bool RunExtensionTestIncognito(const char* extension_name);

  
  bool RunComponentExtensionTest(const char* extension_name);

  
  bool RunExtensionTestNoFileAccess(const char* extension_name);

  
  bool RunExtensionTestIncognitoNoFileAccess(const char* extension_name);

  
  
  
  
  
  bool RunExtensionSubtest(const char* extension_name,
                           const std::string& page_url);

  
  
  bool RunPageTest(const std::string& page_url);

  
  
  bool StartTestServer();

  
  
  const Extension* GetSingleLoadedExtension();

  
  virtual void SetUpCommandLine(CommandLine* command_line);

  
  std::string message_;

 private:
  bool RunExtensionTestImpl(const char* extension_name,
                            const std::string& test_page,
                            bool enable_incogntio,
                            bool enable_fileaccess,
                            bool load_as_component);

  
  
  scoped_ptr<DictionaryValue> test_config_;
};

#endif  
