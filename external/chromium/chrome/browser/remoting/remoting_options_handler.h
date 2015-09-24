// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_REMOTING_REMOTING_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_REMOTING_REMOTING_OPTIONS_HANDLER_H_

#include "base/basictypes.h"
#include "chrome/browser/service/service_process_control.h"

class WebUI;

namespace remoting {

class RemotingOptionsHandler : public ServiceProcessControl::MessageHandler {
 public:
  RemotingOptionsHandler();
  virtual ~RemotingOptionsHandler();

  void Init(WebUI* web_ui);

  
  virtual void OnRemotingHostInfo(
      const remoting::ChromotingHostInfo& host_info);

 private:
  void SetStatus(bool enabled, const std::string& login);

  WebUI* web_ui_;
  ServiceProcessControl* process_control_;

  DISALLOW_COPY_AND_ASSIGN(RemotingOptionsHandler);
};

}  

#endif  
