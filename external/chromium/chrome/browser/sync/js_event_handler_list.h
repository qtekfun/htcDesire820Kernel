// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_JS_EVENT_HANDLER_LIST_H_
#define CHROME_BROWSER_SYNC_JS_EVENT_HANDLER_LIST_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "chrome/browser/sync/js_arg_list.h"
#include "chrome/browser/sync/js_event_router.h"
#include "chrome/browser/sync/js_frontend.h"

namespace browser_sync {

class JsBackend;
class JsEventHandler;

class JsEventHandlerList : public JsFrontend, public JsEventRouter {
 public:
  JsEventHandlerList();

  virtual ~JsEventHandlerList();

  
  
  void SetBackend(JsBackend* backend);

  
  void RemoveBackend();

  
  
  
  virtual void AddHandler(JsEventHandler* handler);
  virtual void RemoveHandler(JsEventHandler* handler);
  virtual void ProcessMessage(
      const std::string& name, const JsArgList& args,
      const JsEventHandler* sender);

  
  
  virtual void RouteJsEvent(const std::string& name,
                            const JsArgList& args,
                            const JsEventHandler* target);

 private:
  
  
  struct PendingMessage {
    std::string name;
    JsArgList args;
    const JsEventHandler* sender;

    PendingMessage(const std::string& name, const JsArgList& args,
                   const JsEventHandler* sender);
  };

  typedef std::vector<PendingMessage> PendingMessageList;

  JsBackend* backend_;
  ObserverList<JsEventHandler> handlers_;
  PendingMessageList pending_messages_;

  DISALLOW_COPY_AND_ASSIGN(JsEventHandlerList);
};

}  

#endif  
