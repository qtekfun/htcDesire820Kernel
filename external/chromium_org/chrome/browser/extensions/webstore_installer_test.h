// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_WEBSTORE_INSTALLER_TEST_H_
#define CHROME_BROWSER_EXTENSIONS_WEBSTORE_INSTALLER_TEST_H_

#include <string>

#include "chrome/test/base/in_process_browser_test.h"
#include "url/gurl.h"

namespace base {
class CommandLine;
}  

class WebstoreInstallerTest : public InProcessBrowserTest {
 public:
  WebstoreInstallerTest(const std::string& webstore_domain,
                        const std::string& test_data_path,
                        const std::string& crx_filename,
                        const std::string& verified_domain,
                        const std::string& unverified_domain);
  virtual ~WebstoreInstallerTest();

  virtual void SetUpCommandLine(CommandLine* command_line) OVERRIDE;
  virtual void SetUpInProcessBrowserTestFixture() OVERRIDE;

 protected:
  GURL GenerateTestServerUrl(const std::string& domain,
                             const std::string& page_filename);

  void RunTest(const std::string& test_function_name);

  
  
  
  
  
  
  bool RunIndexedTest(const std::string& test_function_name, int i);

  
  void RunTestAsync(const std::string& test_function_name);

  std::string webstore_domain_;
  std::string test_data_path_;
  std::string crx_filename_;
  std::string verified_domain_;
  std::string unverified_domain_;
  std::string test_gallery_url_;
};

#endif  
