// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_CONTACT_PROVIDER_CHROMEOS_H_
#define CHROME_BROWSER_AUTOCOMPLETE_CONTACT_PROVIDER_CHROMEOS_H_

#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/autocomplete/autocomplete_provider.h"
#include "chrome/browser/chromeos/contacts/contact_manager_observer.h"

class AutocompleteInput;

namespace contacts {
class ContactManagerInterface;
}

class ContactProvider : public AutocompleteProvider,
                        public contacts::ContactManagerObserver {
 public:
  
  
  static const char kMatchContactIdKey[];

  ContactProvider(
      AutocompleteProviderListener* listener,
      Profile* profile,
      base::WeakPtr<contacts::ContactManagerInterface> contact_manager);

  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;

  
  virtual void OnContactsUpdated(Profile* profile) OVERRIDE;

 private:
  struct ContactData;
  typedef std::vector<ContactData> ContactDataVector;

  virtual ~ContactProvider();

  
  static bool CompareAffinity(const ContactData& a, const ContactData& b);

  
  
  void RefreshContacts();

  
  
  
  void AddContactIfMatched(const AutocompleteInput& input,
                           const std::vector<base::string16>& input_words,
                           const ContactData& contact);

  
  AutocompleteMatch CreateAutocompleteMatch(const AutocompleteInput& input,
                                            const ContactData& contact);

  base::WeakPtr<contacts::ContactManagerInterface> contact_manager_;

  
  ContactDataVector contacts_;

  DISALLOW_COPY_AND_ASSIGN(ContactProvider);
};

#endif  
