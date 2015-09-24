// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_DIALOG_CLOUD_INTERNAL_H_
#define CHROME_BROWSER_PRINTING_PRINT_DIALOG_CLOUD_INTERNAL_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "ui/web_dialogs/web_dialog_delegate.h"
#include "ui/web_dialogs/web_dialog_ui.h"

class CloudPrintWebDialogDelegateTest;
class GURL;

namespace base {
class ListValue;
class Value;
}

namespace internal_cloud_print_helpers {

class CloudPrintDataSenderHelper {
 public:
  explicit CloudPrintDataSenderHelper(content::WebUI* web_ui)
      : web_ui_(web_ui) {}
  virtual ~CloudPrintDataSenderHelper() {}

  
  
  virtual void CallJavascriptFunction(const std::string& function_name,
                                      const base::Value& arg1,
                                      const base::Value& arg2);

 private:
  content::WebUI* web_ui_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintDataSenderHelper);
};

class CloudPrintDataSender
    : public base::RefCountedThreadSafe<CloudPrintDataSender> {
 public:
  
  
  CloudPrintDataSender(CloudPrintDataSenderHelper* helper,
                       const base::string16& print_job_title,
                       const base::string16& print_ticket,
                       const std::string& file_type,
                       const base::RefCountedMemory* data);

  
  
  void SendPrintData();

  
  
  void CancelPrintDataFile();

 private:
  friend class base::RefCountedThreadSafe<CloudPrintDataSender>;
  virtual ~CloudPrintDataSender();

  base::Lock lock_;
  CloudPrintDataSenderHelper* volatile helper_;
  base::string16 print_job_title_;
  base::string16 print_ticket_;
  std::string file_type_;
  scoped_refptr<const base::RefCountedMemory> data_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintDataSender);
};

class CloudPrintWebDialogDelegate;

class CloudPrintFlowHandler : public content::WebUIMessageHandler,
                              public content::NotificationObserver {
 public:
  CloudPrintFlowHandler(const base::RefCountedMemory* data,
                        const base::string16& print_job_title,
                        const base::string16& print_ticket,
                        const std::string& file_type,
                        bool close_after_signin,
                        const base::Closure& callback);
  virtual ~CloudPrintFlowHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void HandleShowDebugger(const base::ListValue* args);
  void HandleSendPrintData(const base::ListValue* args);
  void HandleSetPageParameters(const base::ListValue* args);

  virtual void SetDialogDelegate(CloudPrintWebDialogDelegate *delegate);
  void StoreDialogClientSize() const;

  bool NavigationToURLDidCloseDialog(const GURL& url);

 private:
  virtual scoped_refptr<CloudPrintDataSender> CreateCloudPrintDataSender();

  
  
  
  void ShowDebugger();

  void CancelAnyRunningTask();
  bool IsCloudPrintDialogUrl(const GURL& url);

  CloudPrintWebDialogDelegate* dialog_delegate_;
  content::NotificationRegistrar registrar_;
  scoped_refptr<const base::RefCountedMemory> data_;
  base::string16 print_job_title_;
  base::string16 print_ticket_;
  std::string file_type_;
  scoped_refptr<CloudPrintDataSender> print_data_sender_;
  scoped_ptr<CloudPrintDataSenderHelper> print_data_helper_;
  bool close_after_signin_;
  base::Closure callback_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintFlowHandler);
};

class CloudPrintWebDialogDelegate : public ui::WebDialogDelegate {
 public:
  CloudPrintWebDialogDelegate(content::BrowserContext* browser_context,
                              gfx::NativeWindow modal_parent,
                              const base::RefCountedMemory* data,
                              const std::string& json_arguments,
                              const base::string16& print_job_title,
                              const base::string16& print_ticket,
                              const std::string& file_type,
                              bool close_after_signin,
                              const base::Closure& callback);
  virtual ~CloudPrintWebDialogDelegate();

  
  virtual ui::ModalType GetDialogModalType() const OVERRIDE;
  virtual base::string16 GetDialogTitle() const OVERRIDE;
  virtual GURL GetDialogContentURL() const OVERRIDE;
  virtual void GetWebUIMessageHandlers(
      std::vector<content::WebUIMessageHandler*>* handlers) const OVERRIDE;
  virtual void GetDialogSize(gfx::Size* size) const OVERRIDE;
  virtual std::string GetDialogArgs() const OVERRIDE;
  virtual void OnDialogClosed(const std::string& json_retval) OVERRIDE;
  virtual void OnCloseContents(content::WebContents* source,
                               bool* out_close_dialog) OVERRIDE;
  virtual bool ShouldShowDialogTitle() const OVERRIDE;
  virtual bool HandleContextMenu(
      const content::ContextMenuParams& params) OVERRIDE;
  virtual bool HandleOpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params,
      content::WebContents** out_new_contents) OVERRIDE;

 private:
  friend class ::CloudPrintWebDialogDelegateTest;

  
  CloudPrintWebDialogDelegate(CloudPrintFlowHandler* flow_handler,
                              const std::string& json_arguments);
  void Init(content::BrowserContext* browser_context,
            const std::string& json_arguments);

  CloudPrintFlowHandler* flow_handler_;
  gfx::NativeWindow modal_parent_;
  mutable bool owns_flow_handler_;
  bool keep_alive_when_non_modal_;

  
  ui::WebDialogUI::WebDialogParams params_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintWebDialogDelegate);
};

void CreateDialogForFileImpl(content::BrowserContext* browser_context,
                             gfx::NativeWindow modal_parent,
                             const base::FilePath& path_to_file,
                             const base::string16& print_job_title,
                             const base::string16& print_ticket,
                             const std::string& file_type,
                             bool delete_on_close);
void CreateDialogSigninImpl(content::BrowserContext* browser_context,
                            gfx::NativeWindow modal_parent,
                            const base::Closure& callback);

}  

#endif  
