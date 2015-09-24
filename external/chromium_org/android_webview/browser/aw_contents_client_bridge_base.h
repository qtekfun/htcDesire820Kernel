// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_AW_CONTENTS_CLIENT_BRIDGE_BASE_H_
#define ANDROID_WEBVIEW_BROWSER_AW_CONTENTS_CLIENT_BRIDGE_BASE_H_

#include "base/callback_forward.h"
#include "base/supports_user_data.h"
#include "content/public/browser/javascript_dialog_manager.h"

class GURL;

namespace content {
class WebContents;
}

namespace net {
class X509Certificate;
}

namespace android_webview {

class AwContentsClientBridgeBase {
 public:
  
  static void Associate(content::WebContents* web_contents,
                        AwContentsClientBridgeBase* handler);
  static AwContentsClientBridgeBase* FromWebContents(
      content::WebContents* web_contents);
  static AwContentsClientBridgeBase* FromID(int render_process_id,
                                            int render_view_id);

  virtual ~AwContentsClientBridgeBase();

  virtual void AllowCertificateError(int cert_error,
                                     net::X509Certificate* cert,
                                     const GURL& request_url,
                                     const base::Callback<void(bool)>& callback,
                                     bool* cancel_request) = 0;

  virtual void RunJavaScriptDialog(
      content::JavaScriptMessageType message_type,
      const GURL& origin_url,
      const string16& message_text,
      const string16& default_prompt_text,
      const content::JavaScriptDialogManager::DialogClosedCallback& callback)
      = 0;

  virtual void RunBeforeUnloadDialog(
      const GURL& origin_url,
      const string16& message_text,
      const content::JavaScriptDialogManager::DialogClosedCallback& callback)
      = 0;

  virtual bool ShouldOverrideUrlLoading(const base::string16& url) = 0;
};

}  

#endif  
