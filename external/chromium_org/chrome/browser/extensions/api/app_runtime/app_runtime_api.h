// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_APP_RUNTIME_APP_RUNTIME_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_APP_RUNTIME_APP_RUNTIME_API_H_

#include "base/strings/string16.h"
#include "extensions/browser/extension_function.h"

class Profile;

namespace content {
class WebContents;
}

namespace extensions {

class Extension;

namespace app_file_handler_util {
struct GrantedFileEntry;
}

class AppEventRouter {
 public:
  
  static void DispatchOnLaunchedEvent(Profile* profile,
                                      const Extension* extension);

  
  
  static void DispatchOnRestartedEvent(Profile* profile,
                                       const Extension* extension);

  
  
  
  
  
  
  
  
  
  
  

  
  
  
  static void DispatchOnLaunchedEventWithFileEntry(
      Profile* profile,
      const Extension* extension,
      const std::string& handler_id,
      const std::string& mime_type,
      const extensions::app_file_handler_util::GrantedFileEntry& file_entry);

  
  
  static void DispatchOnLaunchedEventWithUrl(
      Profile* profile,
      const Extension* extension,
      const std::string& handler_id,
      const GURL& url,
      const GURL& referrer_url);
};

}  

#endif  
