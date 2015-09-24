// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOCOMPLETE_HISTORY_MANAGER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOCOMPLETE_HISTORY_MANAGER_H_

#include <vector>

#include "base/gtest_prod_util.h"
#include "base/prefs/pref_member.h"
#include "components/autofill/core/browser/webdata/autofill_webdata_service.h"
#include "components/webdata/common/web_data_service_consumer.h"

namespace autofill {

class AutofillDriver;
class AutofillExternalDelegate;
class AutofillManagerDelegate;
struct FormData;

class AutocompleteHistoryManager : public WebDataServiceConsumer {
 public:
  AutocompleteHistoryManager(AutofillDriver* driver,
                             AutofillManagerDelegate* delegate);
  virtual ~AutocompleteHistoryManager();

  
  virtual void OnWebDataServiceRequestDone(
      WebDataServiceBase::Handle h,
      const WDTypedResult* result) OVERRIDE;

  
  
  void OnGetAutocompleteSuggestions(
      int query_id,
      const base::string16& name,
      const base::string16& prefix,
      const std::vector<base::string16>& autofill_values,
      const std::vector<base::string16>& autofill_labels,
      const std::vector<base::string16>& autofill_icons,
      const std::vector<int>& autofill_unique_ids);
  virtual void OnFormSubmitted(const FormData& form);

  
  void OnRemoveAutocompleteEntry(const base::string16& name,
                                 const base::string16& value);

  
  void SetExternalDelegate(AutofillExternalDelegate* delegate);

 protected:
  friend class AutofillManagerTest;

  
  void SendSuggestions(const std::vector<base::string16>* suggestions);

  
  void set_send_ipc(bool send_ipc) { send_ipc_ = send_ipc; }

 private:
  
  void CancelPendingQuery();

  
  AutofillDriver* driver_;
  scoped_refptr<AutofillWebDataService> database_;

  
  
  
  WebDataServiceBase::Handle pending_query_handle_;
  int query_id_;
  std::vector<base::string16> autofill_values_;
  std::vector<base::string16> autofill_labels_;
  std::vector<base::string16> autofill_icons_;
  std::vector<int> autofill_unique_ids_;

  
  
  AutofillExternalDelegate* external_delegate_;

  
  AutofillManagerDelegate* const manager_delegate_;

  
  bool send_ipc_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteHistoryManager);
};

}  

#endif  
