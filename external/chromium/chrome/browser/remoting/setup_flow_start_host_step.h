// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_REMOTING_SETUP_FLOW_START_HOST_STEP_H_
#define CHROME_BROWSER_REMOTING_SETUP_FLOW_START_HOST_STEP_H_

#include "chrome/browser/remoting/setup_flow.h"
#include "chrome/browser/service/service_process_control.h"
#include "chrome/common/net/gaia/gaia_auth_consumer.h"
#include "chrome/common/net/gaia/gaia_auth_fetcher.h"

namespace remoting {

class SetupFlowStartHostStep : public SetupFlowStepBase,
                               public ServiceProcessControl::MessageHandler {
 public:
  SetupFlowStartHostStep();
  virtual ~SetupFlowStartHostStep();

  
  virtual void HandleMessage(const std::string& message, const Value* arg);
  virtual void Cancel();

  
  virtual void OnRemotingHostInfo(
      const remoting::ChromotingHostInfo& host_info);

 protected:
  virtual void DoStart();

 private:
  void RequestStatus();

  ScopedRunnableMethodFactory<SetupFlowStartHostStep> task_factory_;
  ServiceProcessControl* process_control_;
  bool status_requested_;

  DISALLOW_COPY_AND_ASSIGN(SetupFlowStartHostStep);
};

class SetupFlowStartHostErrorStep : public SetupFlowErrorStepBase {
 public:
  SetupFlowStartHostErrorStep();
  virtual ~SetupFlowStartHostErrorStep();

 protected:
  virtual string16 GetErrorMessage();
  virtual void Retry();

 private:
  DISALLOW_COPY_AND_ASSIGN(SetupFlowStartHostErrorStep);
};

}  

#endif  
