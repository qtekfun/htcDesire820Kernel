// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_IN_PROCESS_BROWSER_TEST_H_
#define CHROME_TEST_BASE_IN_PROCESS_BROWSER_TEST_H_

#include "base/compiler_specific.h"
#include "base/files/scoped_temp_dir.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_base.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {
#if defined(OS_MACOSX)
namespace mac {
class ScopedNSAutoreleasePool;
}  
#endif  

#if defined(OS_WIN) && defined(USE_AURA)
namespace win {
class ScopedCOMInitializer;
}
#endif  
}  

class Browser;
class CommandLine;
class Profile;

namespace content {
class ContentRendererClient;
}


class InProcessBrowserTest : public content::BrowserTestBase {
 public:
  InProcessBrowserTest();
  virtual ~InProcessBrowserTest();

  
  
  virtual void SetUp() OVERRIDE;

  
  virtual void TearDown() OVERRIDE;

 protected:
  
  Browser* browser() const { return browser_; }

  
  void AddTabAtIndexToBrowser(Browser* browser,
                              int index,
                              const GURL& url,
                              content::PageTransition transition);
  void AddTabAtIndex(int index, const GURL& url,
                     content::PageTransition transition);

  
  
  
  
  
  virtual bool SetUpUserDataDirectory() WARN_UNUSED_RESULT;

  
  
  virtual void CleanUpOnMainThread() {}

  
  virtual void RunTestOnMainThreadLoop() OVERRIDE;

  
  
  
  
  Browser* CreateBrowser(Profile* profile);

  
  Browser* CreateIncognitoBrowser();

  
  
  Browser* CreateBrowserForPopup(Profile* profile);

  
  
  Browser* CreateBrowserForApp(const std::string& app_name, Profile* profile);

  
  
  void AddBlankTabAndShow(Browser* browser);

#if !defined OS_MACOSX
  
  
  
  
  
  
  CommandLine GetCommandLineForRelaunch();
#endif

#if defined(OS_MACOSX)
  
  base::mac::ScopedNSAutoreleasePool* AutoreleasePool() const {
    return autorelease_pool_;
  }
#endif  

  void set_exit_when_last_browser_closes(bool value) {
    exit_when_last_browser_closes_ = value;
  }

  
  void set_multi_desktop_test(bool multi_desktop_test) {
    multi_desktop_test_ = multi_desktop_test;
  }

 private:
  
  
  virtual bool CreateUserDataDirectory() WARN_UNUSED_RESULT;

  
  void QuitBrowsers();

  
  
  void PrepareTestCommandLine(CommandLine* command_line);

  
  Browser* browser_;

  
  
  base::ScopedTempDir temp_user_data_dir_;

  
  bool exit_when_last_browser_closes_;

  
  
  bool multi_desktop_test_;

#if defined(OS_MACOSX)
  base::mac::ScopedNSAutoreleasePool* autorelease_pool_;
#endif  

#if defined(OS_WIN) && defined(USE_AURA)
  scoped_ptr<base::win::ScopedCOMInitializer> com_initializer_;
#endif
};

#endif  
