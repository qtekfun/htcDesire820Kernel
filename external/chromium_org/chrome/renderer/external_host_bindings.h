// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTERNAL_HOST_BINDINGS_H_
#define CHROME_RENDERER_EXTERNAL_HOST_BINDINGS_H_

#include "ipc/ipc_sender.h"
#include "webkit/renderer/cpp_bound_class.h"

class ExternalHostBindings : public webkit_glue::CppBoundClass {
 public:
  ExternalHostBindings(IPC::Sender* sender, int routing_id);
  virtual ~ExternalHostBindings();

  
  
  bool ForwardMessageFromExternalHost(const std::string& message,
                                      const std::string& origin,
                                      const std::string& target);

  
  void BindToJavascript(blink::WebFrame* frame, const std::string& classname);

 private:
  
  
  
  bool CreateMessageEvent(NPObject** message_event);

  
  void PostMessage(const webkit_glue::CppArgumentList& args,
                   webkit_glue::CppVariant* result);

  webkit_glue::CppVariant on_message_handler_;
  blink::WebFrame* frame_;
  IPC::Sender* sender_;
  int routing_id_;

  DISALLOW_COPY_AND_ASSIGN(ExternalHostBindings);
};

#endif  
