// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_BROWSER_SHELL_LOGIN_DIALOG_H_
#define CONTENT_SHELL_BROWSER_SHELL_LOGIN_DIALOG_H_

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "content/public/browser/resource_dispatcher_host_login_delegate.h"

#if defined(TOOLKIT_GTK)
#include "ui/base/gtk/gtk_signal.h"
#endif

#if defined(OS_MACOSX)
#if __OBJC__
@class ShellLoginDialogHelper;
#else
class ShellLoginDialogHelper;
#endif  
#endif  

namespace net {
class AuthChallengeInfo;
class URLRequest;
}

namespace content {

class ShellLoginDialog : public ResourceDispatcherHostLoginDelegate {
 public:
  
  ShellLoginDialog(net::AuthChallengeInfo* auth_info, net::URLRequest* request);

  
  
  virtual void OnRequestCancelled() OVERRIDE;

  
  
  
  
  void UserAcceptedAuth(const base::string16& username,
                        const base::string16& password);
  void UserCancelledAuth();

 protected:
  
  virtual ~ShellLoginDialog();

 private:
  
  
  
  
  void PlatformCreateDialog(const base::string16& message);
  
  
  void PlatformCleanUp();
  
  
  void PlatformRequestCancelled();

  
  
  void PrepDialog(const base::string16& host, const base::string16& realm);

  
  
  void SendAuthToRequester(bool success,
                           const base::string16& username,
                           const base::string16& password);

  
  
  scoped_refptr<net::AuthChallengeInfo> auth_info_;

  
  
  net::URLRequest* request_;

#if defined(OS_MACOSX)
  
  ShellLoginDialogHelper* helper_;  
#elif defined(TOOLKIT_GTK)
  GtkWidget* username_entry_;
  GtkWidget* password_entry_;
  GtkWidget* root_;
  CHROMEGTK_CALLBACK_1(ShellLoginDialog, void, OnResponse, int);
#endif
};

}  

#endif  
