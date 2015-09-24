// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_TEST_JS_CHECKER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_TEST_JS_CHECKER_H_

#include <string>

namespace content {
class WebContents;
}

namespace chromeos {
namespace test {

class JSChecker {
 public:
  JSChecker();
  explicit JSChecker(content::WebContents* web_contents);

  
  void Evaluate(const std::string& expression);

  
  bool GetBool(const std::string& expression);
  int GetInt(const std::string& expression);
  std::string GetString(const std::string& expression);

  
  void ExpectTrue(const std::string& expression);
  void ExpectFalse(const std::string& expression);

  
  void ExpectEQ(const std::string& expression, int result);
  void ExpectNE(const std::string& expression, int result);
  void ExpectEQ(const std::string& expression, const std::string& result);
  void ExpectNE(const std::string& expression, const std::string& result);

  void set_web_contents(content::WebContents* web_contents) {
    web_contents_ = web_contents;
  }

 private:
  void GetBoolImpl(const std::string& expression, bool* result);
  void GetIntImpl(const std::string& expression, int* result);
  void GetStringImpl(const std::string& expression, std::string* result);

  content::WebContents* web_contents_;
};

}  
}  

#endif  
