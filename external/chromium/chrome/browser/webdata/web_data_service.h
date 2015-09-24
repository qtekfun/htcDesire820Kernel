// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEBDATA_WEB_DATA_SERVICE_H__
#define CHROME_BROWSER_WEBDATA_WEB_DATA_SERVICE_H__
#pragma once

#include <map>
#include <string>
#include <vector>

#include "app/sql/init_status.h"
#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#ifdef ANDROID
#include "base/task.h"
#include "base/time.h"
#endif
#include "chrome/browser/search_engines/template_url_id.h"
#ifndef ANDROID
#include "content/browser/browser_thread.h"
#endif
#ifdef ANDROID
#include "third_party/skia/include/core/SkBitmap.h"
#endif

class AutofillChange;
class AutofillProfile;
class CreditCard;
class GURL;
#if defined(OS_WIN)
struct IE7PasswordInfo;
#endif
class MessageLoop;
class SkBitmap;
class Task;
class TemplateURL;
class WebDatabase;

namespace base {
class Thread;
}

namespace webkit_glue {
struct FormField;
struct PasswordForm;
}




typedef enum {
  BOOL_RESULT = 1,             
  KEYWORDS_RESULT,             
  INT64_RESULT,                
  PASSWORD_RESULT,             
#if defined(OS_WIN)
  PASSWORD_IE7_RESULT,         
#endif
  WEB_APP_IMAGES,              
  TOKEN_RESULT,                
  AUTOFILL_VALUE_RESULT,       
  AUTOFILL_CHANGES,            
  AUTOFILL_PROFILE_RESULT,     
  AUTOFILL_PROFILES_RESULT,    
  AUTOFILL_CREDITCARD_RESULT,  
  AUTOFILL_CREDITCARDS_RESULT  
} WDResultType;

typedef std::vector<AutofillChange> AutofillChangeList;

struct WDAppImagesResult {
  WDAppImagesResult();
  ~WDAppImagesResult();

  
  bool has_all_images;

  
  std::vector<SkBitmap> images;
};

struct WDKeywordsResult {
  WDKeywordsResult();
  ~WDKeywordsResult();

  std::vector<TemplateURL*> keywords;
  
  
  int64 default_search_provider_id;
  
  int builtin_keyword_version;
};

class WDTypedResult {
 public:
  virtual ~WDTypedResult() {}

  
  WDResultType GetType() const {
    return type_;
  }

 protected:
  explicit WDTypedResult(WDResultType type) : type_(type) {
  }

 private:
  WDResultType type_;
  DISALLOW_COPY_AND_ASSIGN(WDTypedResult);
};

template <class T> class WDResult : public WDTypedResult {
 public:

  WDResult(WDResultType type, const T& v) : WDTypedResult(type), value_(v) {
  }

  virtual ~WDResult() {
  }

  
  T GetValue() const {
    return value_;
  }

 private:
  T value_;

  DISALLOW_COPY_AND_ASSIGN(WDResult);
};

template <class T> class WDObjectResult : public WDTypedResult {
 public:
  explicit WDObjectResult(WDResultType type) : WDTypedResult(type) {
  }

  T* GetValue() const {
    return &value_;
  }

 private:
  
  mutable T value_;
  DISALLOW_COPY_AND_ASSIGN(WDObjectResult);
};

class WebDataServiceConsumer;

class WebDataService
    : public base::RefCountedThreadSafe<WebDataService
#ifndef ANDROID
                                        , BrowserThread::DeleteOnUIThread
#endif
                                       > {
 public:
  
  typedef int Handle;

  
  
  
  
  
  
  
  class WebDataRequest {
   public:
    WebDataRequest(WebDataService* service,
                   Handle handle,
                   WebDataServiceConsumer* consumer);

    virtual ~WebDataRequest();

    Handle GetHandle() const;
    WebDataServiceConsumer* GetConsumer() const;
    bool IsCancelled() const;

    
    
    void Cancel();

    
    
    
    void RequestComplete();

    
    void SetResult(WDTypedResult* r);
    const WDTypedResult* GetResult() const;

   private:
    scoped_refptr<WebDataService> service_;
    MessageLoop* message_loop_;
    Handle handle_;
    bool canceled_;
    WebDataServiceConsumer* consumer_;
    WDTypedResult* result_;

    DISALLOW_COPY_AND_ASSIGN(WebDataRequest);
  };

  
  
  
  
  template <class T>
  class GenericRequest : public WebDataRequest {
   public:
    GenericRequest(WebDataService* service,
                   Handle handle,
                   WebDataServiceConsumer* consumer,
                   const T& arg)
        : WebDataRequest(service, handle, consumer),
          arg_(arg) {
    }

    virtual ~GenericRequest() {
    }

    T GetArgument() {
      return arg_;
    }

   private:
    T arg_;
  };

  template <class T, class U>
  class GenericRequest2 : public WebDataRequest {
   public:
    GenericRequest2(WebDataService* service,
                    Handle handle,
                    WebDataServiceConsumer* consumer,
                    const T& arg1,
                    const U& arg2)
        : WebDataRequest(service, handle, consumer),
          arg1_(arg1),
          arg2_(arg2) {
    }

    virtual ~GenericRequest2() { }

    T GetArgument1() {
      return arg1_;
    }

    U GetArgument2() {
      return arg2_;
    }

   private:
    T arg1_;
    U arg2_;
  };

  WebDataService();

  
  
  bool Init(const FilePath& profile_path);

  
  
  void Shutdown();

  
  bool IsRunning() const;

  
  
  void UnloadDatabase();

  
  
  void CancelRequest(Handle h);

  virtual bool IsDatabaseLoaded();
  virtual WebDatabase* GetDatabase();

  
  
  
  
  

  
  
  
  
  
  
  void AddKeyword(const TemplateURL& url);

  void RemoveKeyword(const TemplateURL& url);

  void UpdateKeyword(const TemplateURL& url);

  
  
  Handle GetKeywords(WebDataServiceConsumer* consumer);

  
  void SetDefaultSearchProvider(const TemplateURL* url);

  
  void SetBuiltinKeywordVersion(int version);

  
  
  
  
  

  
  
  
  void SetWebAppImage(const GURL& app_url, const SkBitmap& image);

  
  void SetWebAppHasAllImages(const GURL& app_url, bool has_all_images);

  
  void RemoveWebApp(const GURL& app_url);

  
  
  Handle GetWebAppImages(const GURL& app_url, WebDataServiceConsumer* consumer);

  
  
  
  
  

  
  void SetTokenForService(const std::string& service,
                          const std::string& token);

  
  void RemoveAllTokens();

  
  Handle GetAllTokens(WebDataServiceConsumer* consumer);

  
  
  
  
  
  
  
  

  
  void AddLogin(const webkit_glue::PasswordForm& form);

  
  void UpdateLogin(const webkit_glue::PasswordForm& form);

  
  void RemoveLogin(const webkit_glue::PasswordForm& form);

  
  void RemoveLoginsCreatedBetween(const base::Time& delete_begin,
                                  const base::Time& delete_end);

  
  void RemoveLoginsCreatedAfter(const base::Time& delete_begin);

  
  
  
  
  Handle GetLogins(const webkit_glue::PasswordForm& form,
                   WebDataServiceConsumer* consumer);

  
  
  
  
  
  Handle GetAutofillableLogins(WebDataServiceConsumer* consumer);

  
  
  
  
  Handle GetBlacklistLogins(WebDataServiceConsumer* consumer);

#if defined(OS_WIN)
  
  void AddIE7Login(const IE7PasswordInfo& info);

  
  void RemoveIE7Login(const IE7PasswordInfo& info);

  
  
  
  
  Handle GetIE7Login(const IE7PasswordInfo& info,
                     WebDataServiceConsumer* consumer);
#endif  

  
  
  
  
  

  
  virtual void AddFormFields(const std::vector<webkit_glue::FormField>& fields);

  
  
  
  
  Handle GetFormValuesForElementName(const string16& name,
                                     const string16& prefix,
                                     int limit,
                                     WebDataServiceConsumer* consumer);

  
  void RemoveFormElementsAddedBetween(const base::Time& delete_begin,
                                      const base::Time& delete_end);
  void RemoveFormValueForElementName(const string16& name,
                                     const string16& value);

  
  void AddAutofillProfile(const AutofillProfile& profile);

  
  void UpdateAutofillProfile(const AutofillProfile& profile);

  
  
  void RemoveAutofillProfile(const std::string& guid);

  
  
  
  
  Handle GetAutofillProfiles(WebDataServiceConsumer* consumer);

  
  
  void EmptyMigrationTrash(bool notify_sync);

  
  void AddCreditCard(const CreditCard& credit_card);

  
  void UpdateCreditCard(const CreditCard& credit_card);

  
  
  void RemoveCreditCard(const std::string& guid);

  
  
  
  
  Handle GetCreditCards(WebDataServiceConsumer* consumer);

  
  void RemoveAutofillProfilesAndCreditCardsModifiedBetween(
      const base::Time& delete_begin,
      const base::Time& delete_end);

  
#ifdef UNIT_TEST
  void set_failed_init(bool value) { failed_init_ = value; }
#endif

 protected:
  friend class TemplateURLModelTest;
  friend class TemplateURLModelTestingProfile;
  friend class WebDataServiceTest;
  friend class WebDataRequest;

  virtual ~WebDataService();

  
  bool InitWithPath(const FilePath& path);

  
  void RequestCompleted(Handle h);

  
  void RegisterRequest(WebDataRequest* request);

  
  
  
  
  
 private:
#ifndef ANDROID
  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
#endif
  friend class DeleteTask<WebDataService>;
  friend class ShutdownTask;

  typedef GenericRequest2<std::vector<const TemplateURL*>,
                          std::vector<TemplateURL*> > SetKeywordsRequest;

  
  void DBInitFailed(sql::InitStatus init_status);

  
  void InitializeDatabaseIfNecessary();

  
  void NotifyDatabaseLoadedOnUIThread();

  
  void ShutdownDatabase();

  
  void Commit();

  
  void ScheduleTask(Task* t);

  
  void ScheduleCommit();

  
  int GetNextRequestHandle();

  
  
  
  
  
  void AddKeywordImpl(GenericRequest<TemplateURL>* request);
  void RemoveKeywordImpl(GenericRequest<TemplateURLID>* request);
  void UpdateKeywordImpl(GenericRequest<TemplateURL>* request);
  void GetKeywordsImpl(WebDataRequest* request);
  void SetDefaultSearchProviderImpl(GenericRequest<TemplateURLID>* r);
  void SetBuiltinKeywordVersionImpl(GenericRequest<int>* r);

  
  
  
  
  
  void SetWebAppImageImpl(GenericRequest2<GURL, SkBitmap>* request);
  void SetWebAppHasAllImagesImpl(GenericRequest2<GURL, bool>* request);
  void RemoveWebAppImpl(GenericRequest<GURL>* request);
  void GetWebAppImagesImpl(GenericRequest<GURL>* request);

  
  
  
  
  

  void RemoveAllTokensImpl(GenericRequest<std::string>* request);
  void SetTokenForServiceImpl(
    GenericRequest2<std::string, std::string>* request);
  void GetAllTokensImpl(GenericRequest<std::string>* request);

  
  
  
  
  
  void AddLoginImpl(GenericRequest<webkit_glue::PasswordForm>* request);
  void UpdateLoginImpl(GenericRequest<webkit_glue::PasswordForm>* request);
  void RemoveLoginImpl(GenericRequest<webkit_glue::PasswordForm>* request);
  void RemoveLoginsCreatedBetweenImpl(
      GenericRequest2<base::Time, base::Time>* request);
  void GetLoginsImpl(GenericRequest<webkit_glue::PasswordForm>* request);
  void GetAutofillableLoginsImpl(WebDataRequest* request);
  void GetBlacklistLoginsImpl(WebDataRequest* request);
#if defined(OS_WIN)
  void AddIE7LoginImpl(GenericRequest<IE7PasswordInfo>* request);
  void RemoveIE7LoginImpl(GenericRequest<IE7PasswordInfo>* request);
  void GetIE7LoginImpl(GenericRequest<IE7PasswordInfo>* request);
#endif  

  
  
  
  
  
  void AddFormElementsImpl(
      GenericRequest<std::vector<webkit_glue::FormField> >* request);
  void GetFormValuesForElementNameImpl(WebDataRequest* request,
      const string16& name, const string16& prefix, int limit);
  void RemoveFormElementsAddedBetweenImpl(
      GenericRequest2<base::Time, base::Time>* request);
  void RemoveFormValueForElementNameImpl(
      GenericRequest2<string16, string16>* request);
  void AddAutofillProfileImpl(GenericRequest<AutofillProfile>* request);
  void UpdateAutofillProfileImpl(GenericRequest<AutofillProfile>* request);
  void RemoveAutofillProfileImpl(GenericRequest<std::string>* request);
  void GetAutofillProfilesImpl(WebDataRequest* request);
  void EmptyMigrationTrashImpl(GenericRequest<bool>* request);
  void AddCreditCardImpl(GenericRequest<CreditCard>* request);
  void UpdateCreditCardImpl(GenericRequest<CreditCard>* request);
  void RemoveCreditCardImpl(GenericRequest<std::string>* request);
  void GetCreditCardsImpl(WebDataRequest* request);
  void RemoveAutofillProfilesAndCreditCardsModifiedBetweenImpl(
      GenericRequest2<base::Time, base::Time>* request);

  
  bool is_running_;

  
  FilePath path_;

  
  WebDatabase* db_;

  
  
  bool failed_init_;

  
  bool should_commit_;

  
  base::Lock pending_lock_;

  
  Handle next_request_handle_;

  typedef std::map<Handle, WebDataRequest*> RequestMap;
  RequestMap pending_requests_;

  
  MessageLoop* main_loop_;

  DISALLOW_COPY_AND_ASSIGN(WebDataService);
};


class WebDataServiceConsumer {
 public:

  
  
  
  virtual void OnWebDataServiceRequestDone(WebDataService::Handle h,
                                           const WDTypedResult* result) = 0;

 protected:
  virtual ~WebDataServiceConsumer() {}
};

#endif  
