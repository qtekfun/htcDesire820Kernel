// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_WEBSTORE_INSTALL_HELPER_H_
#define CHROME_BROWSER_EXTENSIONS_WEBSTORE_INSTALL_HELPER_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/utility_process_host_client.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "url/gurl.h"

class SkBitmap;

namespace base {
class DictionaryValue;
class ListValue;
}

namespace content {
class UtilityProcessHost;
}

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

namespace extensions {

class WebstoreInstallHelper : public content::UtilityProcessHostClient,
                              public net::URLFetcherDelegate {
 public:
  class Delegate {
   public:
    enum InstallHelperResultCode {
      UNKNOWN_ERROR,
      ICON_ERROR,
      MANIFEST_ERROR
    };

    
    
    virtual void OnWebstoreParseSuccess(
        const std::string& id,
        const SkBitmap& icon,
        base::DictionaryValue* parsed_manifest) = 0;

    
    
    virtual void OnWebstoreParseFailure(
        const std::string& id,
        InstallHelperResultCode result_code,
        const std::string& error_message) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  WebstoreInstallHelper(Delegate* delegate,
                        const std::string& id,
                        const std::string& manifest,
                        const std::string& icon_data,
                        const GURL& icon_url,
                        net::URLRequestContextGetter* context_getter);
  void Start();

 private:
  virtual ~WebstoreInstallHelper();

  void StartWorkOnIOThread();
  void StartFetchedImageDecode();
  void ReportResultsIfComplete();
  void ReportResultFromUIThread();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  void OnDecodeImageSucceeded(const SkBitmap& decoded_image);
  void OnDecodeImageFailed();
  void OnJSONParseSucceeded(const base::ListValue& wrapper);
  void OnJSONParseFailed(const std::string& error_message);

  
  Delegate* delegate_;

  
  std::string id_;

  
  std::string manifest_;

  
  
  
  
  std::string icon_base64_data_;
  GURL icon_url_;
  std::vector<unsigned char> fetched_icon_data_;

  
  scoped_ptr<net::URLFetcher> url_fetcher_;
  net::URLRequestContextGetter* context_getter_; 

  base::WeakPtr<content::UtilityProcessHost> utility_host_;

  
  bool icon_decode_complete_;
  bool manifest_parse_complete_;

  
  SkBitmap icon_;
  scoped_ptr<base::DictionaryValue> parsed_manifest_;

  
  std::string error_;

  
  
  Delegate::InstallHelperResultCode parse_error_;
};

}  

#endif  
