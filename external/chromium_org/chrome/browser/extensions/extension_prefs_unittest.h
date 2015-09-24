// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_PREFS_UNITTEST_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_PREFS_UNITTEST_H_

#include "base/message_loop/message_loop.h"
#include "chrome/browser/extensions/test_extension_prefs.h"
#include "content/public/test/test_browser_thread.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {
class Value;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace extensions {
class Extension;

class ExtensionPrefsTest : public testing::Test {
 public:
  ExtensionPrefsTest();
  virtual ~ExtensionPrefsTest();

  
  
  virtual void Initialize() = 0;

  
  
  
  virtual void Verify() = 0;

  
  virtual void RegisterPreferences(user_prefs::PrefRegistrySyncable* registry);

  virtual void SetUp() OVERRIDE;

  virtual void TearDown() OVERRIDE;

 protected:
  ExtensionPrefs* prefs() { return prefs_.prefs(); }

  base::MessageLoop message_loop_;
  content::TestBrowserThread ui_thread_;

  TestExtensionPrefs prefs_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ExtensionPrefsTest);
};


class PrefsPrepopulatedTestBase : public ExtensionPrefsTest {
 public:
  static const size_t kNumInstalledExtensions = 4;

  PrefsPrepopulatedTestBase();
  virtual ~PrefsPrepopulatedTestBase();

  Extension* extension1() { return extension1_.get(); }
  Extension* extension2() { return extension2_.get(); }
  Extension* extension3() { return extension3_.get(); }
  Extension* extension4() { return extension4_.get(); }

 protected:
  bool installed_[kNumInstalledExtensions];

  scoped_refptr<Extension> extension1_;
  scoped_refptr<Extension> extension2_;
  scoped_refptr<Extension> extension3_;
  scoped_refptr<Extension> extension4_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PrefsPrepopulatedTestBase);
};

}  

#endif  
