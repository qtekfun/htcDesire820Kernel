// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_DOM_AUTOMATION_CONTROLLER_H_
#define CONTENT_RENDERER_DOM_AUTOMATION_CONTROLLER_H_

#include "ipc/ipc_sender.h"
#include "webkit/renderer/cpp_bound_class.h"


namespace content {

class DomAutomationController : public webkit_glue::CppBoundClass {
 public:
  DomAutomationController();

  
  
  
  
  
  void Send(const webkit_glue::CppArgumentList& args,
            webkit_glue::CppVariant* result);

  
  
  
  void SendJSON(const webkit_glue::CppArgumentList& args,
                webkit_glue::CppVariant* result);

  
  
  
  
  
  
  void SendWithId(const webkit_glue::CppArgumentList& args,
                  webkit_glue::CppVariant* result);

  void SetAutomationId(const webkit_glue::CppArgumentList& args,
                       webkit_glue::CppVariant* result);

  
  
  

  void set_routing_id(int routing_id) { routing_id_ = routing_id; }

  void set_message_sender(IPC::Sender* sender) {
    sender_ = sender;
  }

 private:
  IPC::Sender* sender_;

  
  int routing_id_;  
  int automation_id_;  
};

}  

#endif  
