// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_NET_NET_ERROR_HELPER_H_
#define CHROME_RENDERER_NET_NET_ERROR_HELPER_H_

#include <string>

#include "chrome/common/net/net_error_info.h"
#include "content/public/renderer/render_view_observer.h"
#include "third_party/WebKit/public/platform/WebURLError.h"

namespace base {
class DictionaryValue;
}

namespace blink {
class WebFrame;
}

class NetErrorHelper : public content::RenderViewObserver {
 public:
  explicit NetErrorHelper(content::RenderView* render_view);
  virtual ~NetErrorHelper();

  
  virtual void DidStartProvisionalLoad(blink::WebFrame* frame) OVERRIDE;
  virtual void DidFailProvisionalLoad(
      blink::WebFrame* frame,
      const blink::WebURLError& error) OVERRIDE;
  virtual void DidCommitProvisionalLoad(
      blink::WebFrame* frame,
      bool is_new_navigation) OVERRIDE;
  virtual void DidFinishLoad(blink::WebFrame* frame) OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  
  
  
  
  static bool GetErrorStringsForDnsProbe(
      blink::WebFrame* frame,
      const blink::WebURLError& error,
      bool is_failed_post,
      const std::string& locale,
      const std::string& accept_languages,
      base::DictionaryValue* error_strings);

 protected:
  
  
  
  void OnStartLoad(bool is_main_frame, bool is_error_page);
  void OnFailLoad(bool is_main_frame, bool is_dns_error);
  void OnCommitLoad(bool is_main_frame);
  void OnFinishLoad(bool is_main_frame);

  void OnNetErrorInfo(int status);

  
  virtual void UpdateErrorPage();

  
  chrome_common_net::DnsProbeStatus last_probe_status_;

 private:
  blink::WebURLError GetUpdatedError() const;

  
  bool last_start_was_error_page_;

  
  bool last_fail_was_dns_error_;

  
  
  
  
  
  
  
  
  
  
  
  
  bool forwarding_probe_results_;

  
  blink::WebURLError last_error_;

  bool is_failed_post_;
};

#endif  
