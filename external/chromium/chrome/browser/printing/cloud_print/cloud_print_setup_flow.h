// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_CLOUD_PRINT_CLOUD_PRINT_SETUP_FLOW_H_
#define CHROME_BROWSER_PRINTING_CLOUD_PRINT_CLOUD_PRINT_SETUP_FLOW_H_

#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/time.h"
#include "chrome/browser/ui/webui/html_dialog_ui.h"
#include "chrome/common/net/gaia/gaia_auth_consumer.h"
#include "chrome/common/net/gaia/gaia_auth_fetcher.h"
#include "chrome/common/net/gaia/google_service_auth_error.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/gfx/native_widget_types.h"

class GaiaAuthFetcher;
class CloudPrintServiceProcessHelper;
class CloudPrintSetupMessageHandler;
class ServiceProcessControl;
class GoogleServiceAuthError;
class Browser;

class CloudPrintSetupFlow : public HtmlDialogUIDelegate,
                            public GaiaAuthConsumer {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}
    
    virtual void OnDialogClosed() = 0;
  };
  virtual ~CloudPrintSetupFlow();

  
  
  
  static CloudPrintSetupFlow* OpenDialog(
      Profile* service,
      const base::WeakPtr<Delegate>& delegate,
      gfx::NativeWindow parent_window);

  
  
  void Focus();

  
  virtual GURL GetDialogContentURL() const;
  virtual void GetWebUIMessageHandlers(
      std::vector<WebUIMessageHandler*>* handlers) const;
  virtual void GetDialogSize(gfx::Size* size) const;
  virtual std::string GetDialogArgs() const;
  virtual void OnDialogClosed(const std::string& json_retval);
  virtual void OnCloseContents(TabContents* source, bool* out_close_dialog);
  virtual std::wstring GetDialogTitle() const;
  virtual bool IsDialogModal() const;
  virtual bool ShouldShowDialogTitle() const;

  
  virtual void OnClientLoginSuccess(
      const GaiaAuthConsumer::ClientLoginResult& credentials);
  virtual void OnClientLoginFailure(
      const GoogleServiceAuthError& error);

 private:
  friend class CloudPrintServiceProcessHelper;
  friend class CloudPrintSetupMessageHandler;

  
  CloudPrintSetupFlow(const std::string& args, Profile* profile,
                      const base::WeakPtr<Delegate>& delegate, bool setup_done);

  
  
  
  void Attach(WebUI* web_ui);

  
  
  void OnUserSubmittedAuth(const std::string& user,
                           const std::string& password,
                           const std::string& captcha,
                           const std::string& access_code);

  
  
  void OnUserClickedLearnMore();
  void OnUserClickedPrintTestPage();

  
  void ShowGaiaLogin(const DictionaryValue& args);
  void ShowGaiaSuccessAndSettingUp();
  void ShowGaiaFailed(const GoogleServiceAuthError& error);
  void ShowSetupDone();
  void ExecuteJavascriptInIFrame(const string16& iframe_xpath,
                                 const string16& js);

  
  
  
  WebUI* web_ui_;

  
  std::string dialog_start_args_;
  Profile* profile_;

  
  scoped_ptr<GaiaAuthFetcher> authenticator_;
  std::string login_;
  std::string lsid_;

  
  GoogleServiceAuthError last_auth_error_;

  
  bool setup_done_;

  
  ServiceProcessControl* process_control_;
  base::WeakPtr<Delegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintSetupFlow);
};

#endif  
