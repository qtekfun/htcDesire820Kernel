// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ANDROID_AUTOFILL_AUTOFILL_DIALOG_RESULT_H_
#define CHROME_BROWSER_UI_ANDROID_AUTOFILL_AUTOFILL_DIALOG_RESULT_H_

#include <jni.h>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

namespace autofill {
namespace wallet {
class FullWallet;
}
}

namespace autofill {

class AutofillDialogResult {
 public:
  
  
  
  static scoped_ptr<wallet::FullWallet> ConvertFromJava(
      JNIEnv* env, jobject wallet);

  
  
  static base::string16 GetWalletEmail(JNIEnv* env, jobject wallet);

  
  
  static std::string GetWalletGoogleTransactionId(JNIEnv* env, jobject wallet);

  
  static bool RegisterAutofillDialogResult(JNIEnv* env);

 private:
  DISALLOW_COPY_AND_ASSIGN(AutofillDialogResult);
};

}  

#endif  
