// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ANDROID_AUTOFILL_AUTOFILL_DIALOG_CONTROLLER_ANDROID_H_
#define CHROME_BROWSER_UI_ANDROID_AUTOFILL_AUTOFILL_DIALOG_CONTROLLER_ANDROID_H_

#include <jni.h>

#include "base/android/jni_string.h"
#include "base/android/scoped_java_ref.h"
#include "base/time/time.h"
#include "chrome/browser/ui/autofill/autofill_dialog_controller.h"
#include "chrome/browser/ui/autofill/autofill_dialog_types.h"

class Profile;

namespace autofill {

class AutofillDialogControllerAndroid : public AutofillDialogController {
 public:
  
  static base::WeakPtr<AutofillDialogController> Create(
      content::WebContents* contents,
      const FormData& form_structure,
      const GURL& source_url,
      const base::Callback<void(const FormStructure*)>& callback);

  virtual ~AutofillDialogControllerAndroid();

  
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void TabActivated() OVERRIDE;

  
  void DialogCancel(JNIEnv* env, jobject obj);
  void DialogContinue(JNIEnv* env,
                      jobject obj,
                      jobject full_wallet,
                      jboolean last_used_choice_is_autofill,
                      jstring last_used_account_name,
                      jstring last_used_billing,
                      jstring last_used_shipping,
                      jstring last_used_credit_card);

  
  static bool RegisterAutofillDialogControllerAndroid(JNIEnv* env);

 private:
  AutofillDialogControllerAndroid(
      content::WebContents* contents,
      const FormData& form_structure,
      const GURL& source_url,
      const base::Callback<void(const FormStructure*)>& callback);

  const AutofillMetrics& GetMetricLogger() const {
    return metric_logger_;
  }

  
  void LogOnFinishSubmitMetrics();

  
  void LogOnCancelMetrics();

  bool RequestingCreditCardInfo() const;
  bool TransmissionWillBeSecure() const;

  
  Profile* const profile_;

  
  content::WebContents* const contents_;

  
  const AutofillMetrics metric_logger_;
  base::Time dialog_shown_timestamp_;
  AutofillMetrics::DialogInitialUserStateMetric initial_user_state_;

  FormStructure form_structure_;

  
  
  bool invoked_from_same_origin_;

  
  GURL source_url_;

  
  base::Callback<void(const FormStructure*)> callback_;

  
  
  bool cares_about_shipping_;

  base::WeakPtrFactory<AutofillDialogControllerAndroid>
      weak_ptr_factory_;

  
  bool was_ui_latency_logged_;

  
  base::android::ScopedJavaGlobalRef<jobject> java_object_;

  DISALLOW_COPY_AND_ASSIGN(AutofillDialogControllerAndroid);
};

}  

#endif  
