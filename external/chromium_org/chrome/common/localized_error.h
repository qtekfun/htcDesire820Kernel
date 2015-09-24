// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_LOCALIZED_ERROR_H_
#define CHROME_COMMON_LOCALIZED_ERROR_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"

class GURL;

namespace base {
class DictionaryValue;
}

namespace extensions {
class Extension;
}

namespace blink {
struct WebURLError;
}

class LocalizedError {
 public:
  
  
  static void GetStrings(int error_code,
                         const std::string& error_domain,
                         const GURL& failed_url,
                         bool is_post,
                         const std::string& locale,
                         const std::string& accept_languages,
                         base::DictionaryValue* strings);

  
  static base::string16 GetErrorDetails(const blink::WebURLError& error,
                                        bool is_post);

  
  static bool HasStrings(const std::string& error_domain, int error_code);

  
  
  
  
  static void GetAppErrorStrings(const GURL& display_url,
                                 const extensions::Extension* app,
                                 base::DictionaryValue* error_strings);

  static const char kHttpErrorDomain[];

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(LocalizedError);
};

#endif  
