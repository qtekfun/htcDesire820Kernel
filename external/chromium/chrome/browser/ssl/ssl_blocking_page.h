// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_BLOCKING_PAGE_H_
#define CHROME_BROWSER_SSL_SSL_BLOCKING_PAGE_H_
#pragma once

#include <string>
#include <vector>

#include "base/string16.h"
#include "chrome/browser/ssl/ssl_error_info.h"
#include "content/browser/tab_contents/interstitial_page.h"

class DictionaryValue;
class SSLCertErrorHandler;

class SSLBlockingPage : public InterstitialPage {
 public:
  
  
  class Delegate {
   public:
    
    
    virtual SSLErrorInfo GetSSLErrorInfo(SSLCertErrorHandler* handler) = 0;

    
    virtual void OnDenyCertificate(SSLCertErrorHandler* handler) = 0;

    
    virtual void OnAllowCertificate(SSLCertErrorHandler* handler) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  enum ErrorLevel {
    ERROR_OVERRIDABLE,  
    ERROR_FATAL,        
                        
  };

  SSLBlockingPage(SSLCertErrorHandler* handler, Delegate* delegate,
                  ErrorLevel error_level);
  virtual ~SSLBlockingPage();

  
  
  
  
  static void SetExtraInfo(DictionaryValue* strings,
                           const std::vector<string16>& extra_info);

 protected:
  
  virtual std::string GetHTMLContents();
  virtual void CommandReceived(const std::string& command);
  virtual void UpdateEntry(NavigationEntry* entry);
  virtual void Proceed();
  virtual void DontProceed();

 private:
  void NotifyDenyCertificate();
  void NotifyAllowCertificate();

  
  
  scoped_refptr<SSLCertErrorHandler> handler_;

  
  
  Delegate* delegate_;

  
  bool delegate_has_been_notified_;

  
  ErrorLevel error_level_;

  DISALLOW_COPY_AND_ASSIGN(SSLBlockingPage);
};

#endif  
