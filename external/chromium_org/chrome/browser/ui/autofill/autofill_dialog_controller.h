// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_CONTROLLER_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_CONTROLLER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/autofill/autofill_dialog_types.h"
#include "components/autofill/core/browser/form_structure.h"

class GURL;
class PrefRegistrySimple;

namespace content {
class WebContents;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace autofill {

class AutofillDialogController {
 public:
  virtual ~AutofillDialogController();

  
  static base::WeakPtr<AutofillDialogController> Create(
      content::WebContents* contents,
      const FormData& form_structure,
      const GURL& source_url,
      const base::Callback<void(const FormStructure*)>& callback);

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  
  virtual void TabActivated() = 0;
};

}  

#endif  
