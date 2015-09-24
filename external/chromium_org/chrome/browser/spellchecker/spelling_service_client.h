// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECKER_SPELLING_SERVICE_CLIENT_H_
#define CHROME_BROWSER_SPELLCHECKER_SPELLING_SERVICE_CLIENT_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "net/url_request/url_fetcher_delegate.h"

class GURL;
class TextCheckClientDelegate;
struct SpellCheckResult;

namespace content {
class BrowserContext;
}

namespace net {
class URLFetcher;
}  

class SpellingServiceClient : public net::URLFetcherDelegate {
 public:
  
  
  
  
  
  
  enum ServiceType {
    SUGGEST = 1,
    SPELLCHECK = 2,
  };
  typedef base::Callback<void(
      bool ,
      const base::string16& ,
      const std::vector<SpellCheckResult>& )>
          TextCheckCompleteCallback;

  SpellingServiceClient();
  virtual ~SpellingServiceClient();

  
  
  
  
  bool RequestTextCheck(content::BrowserContext* context,
                        ServiceType type,
                        const base::string16& text,
                        const TextCheckCompleteCallback& callback);

  
  static bool IsAvailable(content::BrowserContext* context, ServiceType type);

 protected:
  
  bool ParseResponse(const std::string& data,
                     std::vector<SpellCheckResult>* results);

 private:
  struct TextCheckCallbackData {
    TextCheckCallbackData(TextCheckCompleteCallback callback,
                          base::string16 text);
    ~TextCheckCallbackData();

    
    
    TextCheckCompleteCallback callback;

    
    base::string16 text;
  };

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  
  virtual net::URLFetcher* CreateURLFetcher(const GURL& url);

  
  std::map<const net::URLFetcher*, TextCheckCallbackData*> spellcheck_fetchers_;
};

#endif  
