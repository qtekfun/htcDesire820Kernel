// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_PEPPER_PEPPER_PLATFORM_VERIFICATION_MESSAGE_FILTER_H_
#define CHROME_BROWSER_RENDERER_HOST_PEPPER_PEPPER_PLATFORM_VERIFICATION_MESSAGE_FILTER_H_

#include "chrome/browser/chromeos/attestation/platform_verification_flow.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/host/resource_message_filter.h"

namespace content {
class BrowserPpapiHost;
}  

namespace ppapi {
namespace host {
struct HostMessageContext;
}  
}  

namespace chrome {

class PepperPlatformVerificationMessageFilter
    : public ppapi::host::ResourceMessageFilter {
 public:
  PepperPlatformVerificationMessageFilter(content::BrowserPpapiHost* host,
                                          PP_Instance instance);

 private:
  virtual ~PepperPlatformVerificationMessageFilter();

  
  virtual scoped_refptr<base::TaskRunner> OverrideTaskRunnerForMessage(
      const IPC::Message& message) OVERRIDE;
  virtual int32_t OnResourceMessageReceived(
      const IPC::Message& msg,
      ppapi::host::HostMessageContext* context) OVERRIDE;

  int32_t OnChallengePlatform(ppapi::host::HostMessageContext* context,
                              const std::string& service_id,
                              const std::vector<uint8_t>& challenge);

  
  void ChallengePlatformCallback(
      ppapi::host::ReplyMessageContext reply_context,
      chromeos::attestation::PlatformVerificationFlow::Result challenge_result,
      const std::string& signed_data,
      const std::string& signature,
      const std::string& platform_key_certificate);

  
  int render_process_id_;
  int render_view_id_;

  
  scoped_refptr<chromeos::attestation::PlatformVerificationFlow> pv_;

  DISALLOW_COPY_AND_ASSIGN(PepperPlatformVerificationMessageFilter);
};

}  

#endif  
