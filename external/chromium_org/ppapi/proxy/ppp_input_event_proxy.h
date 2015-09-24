// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PPP_INPUT_EVENT_PROXY_H_
#define PPAPI_PROXY_PPP_INPUT_EVENT_PROXY_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/ppp_input_event.h"
#include "ppapi/proxy/interface_proxy.h"

namespace ppapi {

struct InputEventData;

namespace proxy {

class PPP_InputEvent_Proxy : public InterfaceProxy {
 public:
  PPP_InputEvent_Proxy(Dispatcher* dispatcher);
  virtual ~PPP_InputEvent_Proxy();

  static const Info* GetInfo();

  
  virtual bool OnMessageReceived(const IPC::Message& msg);

 private:
  
  void OnMsgHandleInputEvent(PP_Instance instance,
                             const ppapi::InputEventData& data);
  void OnMsgHandleFilteredInputEvent(PP_Instance instance,
                                     const ppapi::InputEventData& data,
                                     PP_Bool* result);

  
  
  
  const PPP_InputEvent* ppp_input_event_impl_;

  DISALLOW_COPY_AND_ASSIGN(PPP_InputEvent_Proxy);
};

}  
}  

#endif  
