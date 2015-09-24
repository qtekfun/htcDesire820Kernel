// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_WEBDATA_BACKEND_IMPL_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_WEBDATA_BACKEND_IMPL_H_

#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_delete_on_message_loop.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/supports_user_data.h"
#include "components/autofill/core/browser/webdata/autofill_webdata.h"
#include "components/autofill/core/browser/webdata/autofill_webdata_backend.h"
#include "components/autofill/core/common/form_field_data.h"
#include "components/webdata/common/web_data_results.h"
#include "components/webdata/common/web_data_service_base.h"
#include "components/webdata/common/web_data_service_consumer.h"
#include "components/webdata/common/web_database.h"

namespace base {
class MessageLoopProxy;
}

class WebDataServiceBackend;

namespace autofill {

class AutofillChange;
class AutofillProfile;
class AutofillWebDataServiceObserverOnDBThread;
class CreditCard;

class AutofillWebDataBackendImpl
    : public base::RefCountedDeleteOnMessageLoop<AutofillWebDataBackendImpl>,
      public AutofillWebDataBackend {
 public:
  
  
  
  
  
  
  AutofillWebDataBackendImpl(
      scoped_refptr<WebDataServiceBackend> web_database_backend,
      scoped_refptr<base::MessageLoopProxy> ui_thread,
      scoped_refptr<base::MessageLoopProxy> db_thread,
      const base::Closure& on_changed_callback);

  
  virtual void AddObserver(AutofillWebDataServiceObserverOnDBThread* observer)
      OVERRIDE;
  virtual void RemoveObserver(
      AutofillWebDataServiceObserverOnDBThread* observer) OVERRIDE;
  virtual WebDatabase* GetDatabase() OVERRIDE;
  virtual void RemoveExpiredFormElements() OVERRIDE;
  virtual void NotifyOfMultipleAutofillChanges() OVERRIDE;

  
  
  
  
  base::SupportsUserData* GetDBUserData();

  void ResetUserData();

  
  WebDatabase::State AddFormElements(const std::vector<FormFieldData>& fields,
                                     WebDatabase* db);

  
  
  scoped_ptr<WDTypedResult> GetFormValuesForElementName(
      const base::string16& name,
      const base::string16& prefix,
      int limit,
      WebDatabase* db);

  
  scoped_ptr<WDTypedResult> HasFormElements(WebDatabase* db);

  
  WebDatabase::State RemoveFormElementsAddedBetween(
      const base::Time& delete_begin,
      const base::Time& delete_end,
      WebDatabase* db);


  
  
  WebDatabase::State RemoveFormValueForElementName(const base::string16& name,
                                                   const base::string16& value,
                                                   WebDatabase* db);

  
  WebDatabase::State AddAutofillProfile(const AutofillProfile& profile,
                                        WebDatabase* db);

  
  WebDatabase::State UpdateAutofillProfile(const AutofillProfile& profile,
                                           WebDatabase* db);

  
  WebDatabase::State RemoveAutofillProfile(const std::string& guid,
                                           WebDatabase* db);

  
  scoped_ptr<WDTypedResult> GetAutofillProfiles(WebDatabase* db);

  
  WebDatabase::State AddCreditCard(const CreditCard& credit_card,
                                   WebDatabase* db);

  
  WebDatabase::State UpdateCreditCard(const CreditCard& credit_card,
                                      WebDatabase* db);

  
  WebDatabase::State RemoveCreditCard(const std::string& guid,
                                      WebDatabase* db);

  
  scoped_ptr<WDTypedResult> GetCreditCards(WebDatabase* db);

  
  WebDatabase::State RemoveAutofillDataModifiedBetween(
      const base::Time& delete_begin,
      const base::Time& delete_end,
      WebDatabase* db);

  
  
  WebDatabase::State RemoveOriginURLsModifiedBetween(
      const base::Time& delete_begin,
      const base::Time& delete_end,
      WebDatabase* db);

 protected:
  virtual ~AutofillWebDataBackendImpl();

 private:
  friend class base::RefCountedDeleteOnMessageLoop<AutofillWebDataBackendImpl>;
  friend class base::DeleteHelper<AutofillWebDataBackendImpl>;

  
  
  
  
  class SupportsUserDataAggregatable : public base::SupportsUserData {
   public:
    SupportsUserDataAggregatable() {}
    virtual ~SupportsUserDataAggregatable() {}
   private:
    DISALLOW_COPY_AND_ASSIGN(SupportsUserDataAggregatable);
  };

  
  scoped_refptr<base::MessageLoopProxy> ui_thread_;

  
  scoped_refptr<base::MessageLoopProxy> db_thread_;

  
  
  
  scoped_ptr<SupportsUserDataAggregatable> user_data_;

  WebDatabase::State RemoveExpiredFormElementsImpl(WebDatabase* db);

  
  
  void DestroyAutofillProfileResult(const WDTypedResult* result);
  void DestroyAutofillCreditCardResult(const WDTypedResult* result);

  ObserverList<AutofillWebDataServiceObserverOnDBThread> db_observer_list_;

  
  
  scoped_refptr<WebDataServiceBackend> web_database_backend_;

  base::Closure on_changed_callback_;

  DISALLOW_COPY_AND_ASSIGN(AutofillWebDataBackendImpl);
};

}  

#endif  
