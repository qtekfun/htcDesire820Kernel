// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_APITEST_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_APITEST_H_

#include <deque>
#include <string>

#include "base/compiler_specific.h"
#include "base/values.h"
#include "chrome/browser/extensions/extension_browsertest.h"
#include "content/public/browser/notification_registrar.h"

namespace base {
class FilePath;
}

namespace extensions {
class Extension;
}

class ExtensionApiTest : public ExtensionBrowserTest {
 public:
  
  
  
  enum Flags {
    kFlagNone = 0,

    
    kFlagEnableIncognito = 1 << 0,

    
    kFlagUseIncognito = 1 << 1,

    
    kFlagEnableFileAccess = 1 << 2,

    
    kFlagLoadAsComponent = 1 << 3,

    
    kFlagLaunchPlatformApp = 1 << 4,

    
    kFlagIgnoreManifestWarnings = 1 << 5,

    
    
    kFlagAllowOldManifestVersions = 1 << 6,
  };

  ExtensionApiTest();
  virtual ~ExtensionApiTest();

 protected:
  
  
  
  
  
  class ResultCatcher : public content::NotificationObserver {
   public:
    ResultCatcher();
    virtual ~ResultCatcher();

    
    
    bool GetNextResult();

    void RestrictToProfile(Profile* profile) { profile_restriction_ = profile; }

    const std::string& message() { return message_; }

   private:
    virtual void Observe(int type,
                         const content::NotificationSource& source,
                         const content::NotificationDetails& details) OVERRIDE;

    content::NotificationRegistrar registrar_;

    
    std::deque<bool> results_;

    
    std::deque<std::string> messages_;
    std::string message_;

    
    Profile* profile_restriction_;

    
    
    bool waiting_;
  };

  virtual void SetUpInProcessBrowserTestFixture() OVERRIDE;
  virtual void TearDownInProcessBrowserTestFixture() OVERRIDE;

  
  
  bool RunExtensionTest(const std::string& extension_name);

  
  bool RunExtensionTestIncognito(const std::string& extension_name);

  
  bool RunExtensionTestIgnoreManifestWarnings(
      const std::string& extension_name);

  
  bool RunExtensionTestAllowOldManifestVersion(
      const std::string& extension_name);

  
  bool RunComponentExtensionTest(const std::string& extension_name);

  
  bool RunExtensionTestNoFileAccess(const std::string& extension_name);

  
  bool RunExtensionTestIncognitoNoFileAccess(const std::string& extension_name);

  
  
  
  
  
  bool RunExtensionSubtest(const std::string& extension_name,
                           const std::string& page_url);

  
  
  bool RunExtensionSubtest(const std::string& extension_name,
                           const std::string& page_url,
                           int flags);

  
  
  bool RunPageTest(const std::string& page_url);
  bool RunPageTest(const std::string& page_url, int flags);

  
  
  bool RunPlatformAppTest(const std::string& extension_name);

  
  
  bool RunPlatformAppTestWithArg(
      const std::string& extension_name, const char* custom_arg);

  
  
  bool RunPlatformAppTestWithFlags(const std::string& extension_name,
                                   int flags);

  
  
  bool StartEmbeddedTestServer();

  
  
  
  bool StartWebSocketServer(const base::FilePath& root_directory);

  
  
  
  bool StartSpawnedTestServer();

  
  
  const extensions::Extension* GetSingleLoadedExtension();

  
  virtual void SetUpCommandLine(CommandLine* command_line) OVERRIDE;

  
  std::string message_;

 private:
  bool RunExtensionTestImpl(const std::string& extension_name,
                            const std::string& test_page,
                            const char* custom_arg,
                            int flags);

  
  
  scoped_ptr<DictionaryValue> test_config_;

  
  scoped_ptr<net::SpawnedTestServer> websocket_server_;
};

#endif  
