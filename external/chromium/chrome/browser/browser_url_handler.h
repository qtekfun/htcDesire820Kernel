// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_BROWSER_URL_HANDLER_H_
#define CHROME_BROWSER_BROWSER_URL_HANDLER_H_
#pragma once

#include <vector>
#include <utility>

class GURL;
class Profile;

class BrowserURLHandler {
 public:
  
  
  
  
  typedef bool (*URLHandler)(GURL* url, Profile* profile);

  
  
  
  
  static void RewriteURLIfNecessary(GURL* url, Profile* profile,
                                    bool* reverse_on_redirect);

  
  static bool ReverseURLRewrite(GURL* url, const GURL& original,
                                Profile* profile);

  
  
  static void InitURLHandlers();

 private:
  
  typedef std::pair<URLHandler, URLHandler> HandlerPair;
  static std::vector<HandlerPair> url_handlers_;
};

#endif  
