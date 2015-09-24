// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_COOKIES_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_COOKIES_API_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "base/time.h"
#include "chrome/browser/extensions/extension_function.h"
#include "chrome/browser/net/chrome_cookie_notification_details.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"
#include "net/base/cookie_monster.h"

class DictionaryValue;

namespace net {
class URLRequestContextGetter;
}

class ExtensionCookiesEventRouter : public NotificationObserver {
 public:
  
  static ExtensionCookiesEventRouter* GetInstance();

  void Init();

 private:
  friend struct DefaultSingletonTraits<ExtensionCookiesEventRouter>;

  ExtensionCookiesEventRouter() {}
  virtual ~ExtensionCookiesEventRouter() {}

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void CookieChanged(Profile* profile,
                     ChromeCookieDetails* details);

  
  void DispatchEvent(Profile* context,
                     const char* event_name,
                     const std::string& json_args,
                     GURL& cookie_domain);

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionCookiesEventRouter);
};

class CookiesFunction : public AsyncExtensionFunction {
 protected:
  
  
  
  
  
  bool ParseUrl(const DictionaryValue* details, GURL* url,
                bool check_host_permissions);

  
  
  
  
  
  
  
  bool ParseStoreContext(const DictionaryValue* details,
                         net::URLRequestContextGetter** context,
                         std::string* store_id);
};

class GetCookieFunction : public CookiesFunction {
 public:
  GetCookieFunction();
  virtual ~GetCookieFunction();
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("cookies.get")

 private:
  void GetCookieOnIOThread();
  void RespondOnUIThread();

  std::string name_;
  GURL url_;
  std::string store_id_;
  scoped_refptr<net::URLRequestContextGetter> store_context_;
};

class GetAllCookiesFunction : public CookiesFunction {
 public:
  GetAllCookiesFunction();
  virtual ~GetAllCookiesFunction();
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("cookies.getAll")

 private:
  void GetAllCookiesOnIOThread();
  void RespondOnUIThread();

  DictionaryValue* details_;
  GURL url_;
  std::string store_id_;
  scoped_refptr<net::URLRequestContextGetter> store_context_;
};

class SetCookieFunction : public CookiesFunction {
 public:
  SetCookieFunction();
  virtual ~SetCookieFunction();
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("cookies.set")

 private:
  void SetCookieOnIOThread();
  void RespondOnUIThread();

  GURL url_;
  std::string name_;
  std::string value_;
  std::string domain_;
  std::string path_;
  bool secure_;
  bool http_only_;
  base::Time expiration_time_;
  bool success_;
  std::string store_id_;
  scoped_refptr<net::URLRequestContextGetter> store_context_;
};

class RemoveCookieFunction : public CookiesFunction {
 public:
  RemoveCookieFunction();
  virtual ~RemoveCookieFunction();
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("cookies.remove")

 private:
  void RemoveCookieOnIOThread();
  void RespondOnUIThread();

  GURL url_;
  std::string name_;
  bool success_;
  std::string store_id_;
  scoped_refptr<net::URLRequestContextGetter> store_context_;
};

class GetAllCookieStoresFunction : public CookiesFunction {
 public:
  virtual bool RunImpl();
  
  virtual void Run();
  DECLARE_EXTENSION_FUNCTION_NAME("cookies.getAllCookieStores")
};

#endif  
