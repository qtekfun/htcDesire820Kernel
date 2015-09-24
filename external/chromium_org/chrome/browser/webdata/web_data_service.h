// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// information and MUST not be extracted, overwritten or modified except

#ifndef CHROME_BROWSER_WEBDATA_WEB_DATA_SERVICE_H__
#define CHROME_BROWSER_WEBDATA_WEB_DATA_SERVICE_H__

#include <map>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/location.h"
#include "base/memory/ref_counted.h"
#include "base/sequenced_task_runner_helpers.h"
#include "chrome/browser/search_engines/template_url.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "chrome/browser/webdata/keyword_table.h"
#include "components/webdata/common/web_data_results.h"
#include "components/webdata/common/web_data_service_base.h"
#include "components/webdata/common/web_data_service_consumer.h"
#include "components/webdata/common/web_database.h"

struct DefaultWebIntentService;
class GURL;
#if defined(OS_WIN)
struct IE7PasswordInfo;
#endif
class Profile;
class SkBitmap;
class WebDatabaseService;

namespace base {
class Thread;
}

namespace content {
class BrowserContext;
}

namespace webkit_glue {
struct WebIntentServiceData;
}




typedef base::Callback<scoped_ptr<WDTypedResult>(void)> ResultTask;

struct WDAppImagesResult {
  WDAppImagesResult();
  ~WDAppImagesResult();

  
  bool has_all_images;

  
  std::vector<SkBitmap> images;
};

struct WDKeywordsResult {
  WDKeywordsResult();
  ~WDKeywordsResult();

  KeywordTable::Keywords keywords;
  
  
  int64 default_search_provider_id;
  
  int builtin_keyword_version;
};

class WebDataServiceConsumer;

class WebDataService : public WebDataServiceBase {
 public:
  
  static scoped_refptr<WebDataService> FromBrowserContext(
      content::BrowserContext* context);

  WebDataService(scoped_refptr<WebDatabaseService> wdbs,
                 const ProfileErrorCallback& callback);

  
  
  
  
  

  
  
  
  
  
  

  void AddKeyword(const TemplateURLData& data);
  void RemoveKeyword(TemplateURLID id);
  void UpdateKeyword(const TemplateURLData& data);

  
  
  Handle GetKeywords(WebDataServiceConsumer* consumer);

  
  void SetDefaultSearchProvider(const TemplateURL* url);

  
  void SetBuiltinKeywordVersion(int version);

  
  
  
  
  

  
  
  
  void SetWebAppImage(const GURL& app_url, const SkBitmap& image);

  
  void SetWebAppHasAllImages(const GURL& app_url, bool has_all_images);

  
  void RemoveWebApp(const GURL& app_url);

  
  
  Handle GetWebAppImages(const GURL& app_url, WebDataServiceConsumer* consumer);

#if defined(OS_WIN)
  
  
  
  
  

  
  void AddIE7Login(const IE7PasswordInfo& info);

  
  void RemoveIE7Login(const IE7PasswordInfo& info);

  
  
  
  
  Handle GetIE7Login(const IE7PasswordInfo& info,
                     WebDataServiceConsumer* consumer);
#endif  

 protected:
  
  WebDataService();

  virtual ~WebDataService();

 private:
  
  
  
  
  

  
  
  
  
  
  WebDatabase::State AddKeywordImpl(
      const TemplateURLData& data, WebDatabase* db);
  WebDatabase::State RemoveKeywordImpl(
      TemplateURLID id, WebDatabase* db);
  WebDatabase::State UpdateKeywordImpl(
      const TemplateURLData& data, WebDatabase* db);
  scoped_ptr<WDTypedResult> GetKeywordsImpl(WebDatabase* db);
  WebDatabase::State SetDefaultSearchProviderImpl(
      TemplateURLID r, WebDatabase* db);
  WebDatabase::State SetBuiltinKeywordVersionImpl(int version, WebDatabase* db);

  
  
  
  
  

  WebDatabase::State SetWebAppImageImpl(const GURL& app_url,
      const SkBitmap& image, WebDatabase* db);
  WebDatabase::State SetWebAppHasAllImagesImpl(const GURL& app_url,
      bool has_all_images, WebDatabase* db);
  WebDatabase::State RemoveWebAppImpl(const GURL& app_url, WebDatabase* db);
  scoped_ptr<WDTypedResult> GetWebAppImagesImpl(
      const GURL& app_url, WebDatabase* db);

#if defined(ENABLE_WEB_INTENTS)
  
  
  
  
  
  WebDatabase::State AddWebIntentServiceImpl(
      const webkit_glue::WebIntentServiceData& service);
  WebDatabase::State RemoveWebIntentServiceImpl(
      const webkit_glue::WebIntentServiceData& service);
  scoped_ptr<WDTypedResult> GetWebIntentServicesImpl(
      const base::string16& action);
  scoped_ptr<WDTypedResult> GetWebIntentServicesForURLImpl(
      const base::string16& service_url);
  scoped_ptr<WDTypedResult> GetAllWebIntentServicesImpl();
  WebDatabase::State AddDefaultWebIntentServiceImpl(
      const DefaultWebIntentService& service);
  WebDatabase::State RemoveDefaultWebIntentServiceImpl(
      const DefaultWebIntentService& service);
  WebDatabase::State RemoveWebIntentServiceDefaultsImpl(
      const GURL& service_url);
  scoped_ptr<WDTypedResult> GetDefaultWebIntentServicesForActionImpl(
      const base::string16& action);
  scoped_ptr<WDTypedResult> GetAllDefaultWebIntentServicesImpl();
#endif

#if defined(OS_WIN)
  
  
  
  
  
  WebDatabase::State AddIE7LoginImpl(
      const IE7PasswordInfo& info, WebDatabase* db);
  WebDatabase::State RemoveIE7LoginImpl(
      const IE7PasswordInfo& info, WebDatabase* db);
  scoped_ptr<WDTypedResult> GetIE7LoginImpl(
      const IE7PasswordInfo& info, WebDatabase* db);
#endif  

  DISALLOW_COPY_AND_ASSIGN(WebDataService);
};

#endif  
