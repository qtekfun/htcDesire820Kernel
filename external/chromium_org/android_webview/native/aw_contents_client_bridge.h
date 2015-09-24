// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_NATIVE_AW_CONTENTS_CLIENT_BRIDGE_H_
#define ANDROID_WEBVIEW_NATIVE_AW_CONTENTS_CLIENT_BRIDGE_H_

#include <jni.h>

#include "android_webview/browser/aw_contents_client_bridge_base.h"
#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "base/callback.h"
#include "base/id_map.h"
#include "content/public/browser/javascript_dialog_manager.h"

namespace net {
class X509Certificate;
}

namespace android_webview {

class AwContentsClientBridge : public AwContentsClientBridgeBase {
 public:

  AwContentsClientBridge(JNIEnv* env, jobject obj);
  virtual ~AwContentsClientBridge();

  
  virtual void AllowCertificateError(int cert_error,
                                     net::X509Certificate* cert,
                                     const GURL& request_url,
                                     const base::Callback<void(bool)>& callback,
                                     bool* cancel_request) OVERRIDE;

  virtual void RunJavaScriptDialog(
      content::JavaScriptMessageType message_type,
      const GURL& origin_url,
      const string16& message_text,
      const string16& default_prompt_text,
      const content::JavaScriptDialogManager::DialogClosedCallback& callback)
      OVERRIDE;
  virtual void RunBeforeUnloadDialog(
      const GURL& origin_url,
      const string16& message_text,
      const content::JavaScriptDialogManager::DialogClosedCallback& callback)
      OVERRIDE;
  virtual bool ShouldOverrideUrlLoading(const base::string16& url) OVERRIDE;

  
  void ProceedSslError(JNIEnv* env, jobject obj, jboolean proceed, jint id);
  void ConfirmJsResult(JNIEnv*, jobject, int id, jstring prompt);
  void CancelJsResult(JNIEnv*, jobject, int id);

 private:
  JavaObjectWeakGlobalRef java_ref_;

  typedef const base::Callback<void(bool)> CertErrorCallback;
  IDMap<CertErrorCallback, IDMapOwnPointer> pending_cert_error_callbacks_;
  IDMap<content::JavaScriptDialogManager::DialogClosedCallback, IDMapOwnPointer>
      pending_js_dialog_callbacks_;
};

bool RegisterAwContentsClientBridge(JNIEnv* env);

}  

#endif  
