// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_DIALOG_CLOUD_INTERNAL_H_
#define CHROME_BROWSER_PRINTING_PRINT_DIALOG_CLOUD_INTERNAL_H_
#pragma once

#include <string>
#include <vector>

#include "base/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/ui/webui/html_dialog_ui.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class GURL;
class StringValue;
class CloudPrintHtmlDialogDelegateTest;

namespace internal_cloud_print_helpers {

class CloudPrintDataSenderHelper {
 public:
  explicit CloudPrintDataSenderHelper(WebUI* web_ui) : web_ui_(web_ui) {}
  virtual ~CloudPrintDataSenderHelper() {}

  
  
  virtual void CallJavascriptFunction(const std::wstring& function_name);
  virtual void CallJavascriptFunction(const std::wstring& function_name,
                                      const Value& arg);
  virtual void CallJavascriptFunction(const std::wstring& function_name,
                                      const Value& arg1,
                                      const Value& arg2);

 private:
  WebUI* web_ui_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintDataSenderHelper);
};

class CloudPrintDataSender
    : public base::RefCountedThreadSafe<CloudPrintDataSender> {
 public:
  
  
  CloudPrintDataSender(CloudPrintDataSenderHelper* helper,
                       const string16& print_job_title,
                       const std::string& file_type);

  
  
  
  
  void ReadPrintDataFile(const FilePath& path_to_file);
  void SendPrintDataFile();

  
  
  void CancelPrintDataFile();

 private:
  friend class base::RefCountedThreadSafe<CloudPrintDataSender>;
  virtual ~CloudPrintDataSender();

  base::Lock lock_;
  CloudPrintDataSenderHelper* volatile helper_;
  scoped_ptr<StringValue> print_data_;
  string16 print_job_title_;
  std::string file_type_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintDataSender);
};

class CloudPrintHtmlDialogDelegate;

class CloudPrintFlowHandler : public WebUIMessageHandler,
                              public NotificationObserver {
 public:
  explicit CloudPrintFlowHandler(const FilePath& path_to_file,
                                 const string16& print_job_title,
                                 const std::string& file_type);
  virtual ~CloudPrintFlowHandler();

  
  virtual void RegisterMessages();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void HandleShowDebugger(const ListValue* args);
  void HandleSendPrintData(const ListValue* args);
  void HandleSetPageParameters(const ListValue* args);

  virtual void SetDialogDelegate(CloudPrintHtmlDialogDelegate *delegate);
  void StoreDialogClientSize() const;

 private:
  virtual scoped_refptr<CloudPrintDataSender> CreateCloudPrintDataSender();

  
  
  
  void ShowDebugger();

  void CancelAnyRunningTask();

  CloudPrintHtmlDialogDelegate* dialog_delegate_;
  NotificationRegistrar registrar_;
  FilePath path_to_file_;
  string16 print_job_title_;
  std::string file_type_;
  scoped_refptr<CloudPrintDataSender> print_data_sender_;
  scoped_ptr<CloudPrintDataSenderHelper> print_data_helper_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintFlowHandler);
};

class CloudPrintHtmlDialogDelegate : public HtmlDialogUIDelegate {
 public:
  CloudPrintHtmlDialogDelegate(const FilePath& path_to_file,
                               int width, int height,
                               const std::string& json_arguments,
                               const string16& print_job_title,
                               const std::string& file_type,
                               bool modal);
  virtual ~CloudPrintHtmlDialogDelegate();

  
  virtual bool IsDialogModal() const;
  virtual std::wstring GetDialogTitle() const;
  virtual GURL GetDialogContentURL() const;
  virtual void GetWebUIMessageHandlers(
      std::vector<WebUIMessageHandler*>* handlers) const;
  virtual void GetDialogSize(gfx::Size* size) const;
  virtual std::string GetDialogArgs() const;
  virtual void OnDialogClosed(const std::string& json_retval);
  virtual void OnCloseContents(TabContents* source, bool* out_close_dialog);
  virtual bool ShouldShowDialogTitle() const;
  virtual bool HandleContextMenu(const ContextMenuParams& params);

 private:
  friend class ::CloudPrintHtmlDialogDelegateTest;

  
  CloudPrintHtmlDialogDelegate(CloudPrintFlowHandler* flow_handler,
                               int width, int height,
                               const std::string& json_arguments,
                               bool modal);
  void Init(int width, int height, const std::string& json_arguments);

  CloudPrintFlowHandler* flow_handler_;
  bool modal_;
  mutable bool owns_flow_handler_;

  
  HtmlDialogUI::HtmlDialogParams params_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintHtmlDialogDelegate);
};

void CreateDialogImpl(const FilePath& path_to_file,
                      const string16& print_job_title,
                      const std::string& file_type,
                      bool modal);

}  

#endif  
