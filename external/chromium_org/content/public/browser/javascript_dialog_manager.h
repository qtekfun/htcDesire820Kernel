// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_JAVASCRIPT_DIALOG_MANAGER_H_
#define CONTENT_PUBLIC_BROWSER_JAVASCRIPT_DIALOG_MANAGER_H_

#include <string>

#include "base/callback.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/public/common/javascript_message_type.h"
#include "ui/gfx/native_widget_types.h"
#include "url/gurl.h"

namespace content {

class WebContents;

class CONTENT_EXPORT JavaScriptDialogManager {
 public:
  typedef base::Callback<void(bool ,
                              const base::string16& )>
                                  DialogClosedCallback;

  
  
  virtual void RunJavaScriptDialog(
      WebContents* web_contents,
      const GURL& origin_url,
      const std::string& accept_lang,
      JavaScriptMessageType javascript_message_type,
      const base::string16& message_text,
      const base::string16& default_prompt_text,
      const DialogClosedCallback& callback,
      bool* did_suppress_message) = 0;

  
  virtual void RunBeforeUnloadDialog(WebContents* web_contents,
                                     const base::string16& message_text,
                                     bool is_reload,
                                     const DialogClosedCallback& callback) = 0;

  
  
  
  
  virtual bool HandleJavaScriptDialog(WebContents* web_contents,
                                      bool accept,
                                      const base::string16* prompt_override);

  
  virtual void CancelActiveAndPendingDialogs(WebContents* web_contents) = 0;

  
  
  virtual void WebContentsDestroyed(WebContents* web_contents) = 0;

  virtual ~JavaScriptDialogManager() {}
};

}  

#endif  
