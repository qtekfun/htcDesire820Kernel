// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_REMOTING_SETUP_FLOW_H_
#define CHROME_BROWSER_REMOTING_SETUP_FLOW_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/webui/html_dialog_ui.h"
#include "chrome/common/remoting/chromoting_host_info.h"
#include "content/browser/webui/web_ui.h"

class ListValue;
class ServiceProcessControl;

namespace remoting {

class SetupFlow;

class SetupFlowStep {
 public:
  typedef Callback0::Type DoneCallback;

  SetupFlowStep();
  virtual ~SetupFlowStep();

  
  
  
  virtual void Start(SetupFlow* flow, DoneCallback* done_callback) = 0;

  
  
  virtual void HandleMessage(const std::string& message, const Value* arg) = 0;

  
  virtual void Cancel() = 0;

  
  
  virtual SetupFlowStep* GetNextStep() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(SetupFlowStep);
};

class SetupFlowStepBase : public SetupFlowStep {
 public:
  SetupFlowStepBase();
  ~SetupFlowStepBase();

  
  virtual void Start(SetupFlow* flow, DoneCallback* done_callback);
  virtual SetupFlowStep* GetNextStep();

 protected:
  SetupFlow* flow() { return flow_; }

  void ExecuteJavascriptInIFrame(const std::wstring& iframe_xpath,
                                 const std::wstring& js);

  
  
  void FinishStep(SetupFlowStep* next_step);

  
  
  virtual void DoStart() = 0;

 private:
  SetupFlow* flow_;
  scoped_ptr<DoneCallback> done_callback_;
  bool done_;

  
  SetupFlowStep* next_step_;

  DISALLOW_COPY_AND_ASSIGN(SetupFlowStepBase);
};

class SetupFlowErrorStepBase : public SetupFlowStepBase {
 public:
  SetupFlowErrorStepBase();
  virtual ~SetupFlowErrorStepBase();

  
  virtual void HandleMessage(const std::string& message, const Value* arg);
  virtual void Cancel();

 protected:
  virtual void DoStart();

  
  virtual string16 GetErrorMessage() = 0;

  
  
  virtual void Retry() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(SetupFlowErrorStepBase);
};

class SetupFlowDoneStep : public SetupFlowStepBase {
 public:
  SetupFlowDoneStep();
  explicit SetupFlowDoneStep(const string16& message);
  virtual ~SetupFlowDoneStep();

  
  virtual void HandleMessage(const std::string& message, const Value* arg);
  virtual void Cancel();

 protected:
  virtual void DoStart();

 private:
  string16 message_;

  DISALLOW_COPY_AND_ASSIGN(SetupFlowDoneStep);
};

struct SetupFlowContext {
  SetupFlowContext();
  ~SetupFlowContext();

  std::string login;
  std::string remoting_token;
  std::string talk_token;

  ChromotingHostInfo host_info;
};

class SetupFlow : public WebUIMessageHandler,
                  public HtmlDialogUIDelegate {
 public:
  virtual ~SetupFlow();

  static SetupFlow* OpenSetupDialog(Profile* profile);

  WebUI* web_ui() { return web_ui_; }
  Profile* profile() { return profile_; }
  SetupFlowContext* context() { return &context_; }

 private:
  explicit SetupFlow(const std::string& args, Profile* profile,
                     SetupFlowStep* first_step);

  
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

  
  virtual WebUIMessageHandler* Attach(WebUI* web_ui);
  virtual void RegisterMessages();

  
  void HandleSubmitAuth(const ListValue* args);
  void HandleUIMessage(const ListValue* args);

  void StartCurrentStep();
  void OnStepDone();

  
  
  WebUI* web_ui_;

  
  std::string dialog_start_args_;
  Profile* profile_;

  SetupFlowContext context_;

  scoped_ptr<SetupFlowStep> current_step_;

  DISALLOW_COPY_AND_ASSIGN(SetupFlow);
};

}  

#endif  
