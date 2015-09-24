// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_USER_SCRIPT_SCHEDULER_H_
#define CHROME_RENDERER_EXTENSIONS_USER_SCRIPT_SCHEDULER_H_

#include <map>
#include <queue>

#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "extensions/common/user_script.h"

class RenderView;
struct ExtensionMsg_ExecuteCode_Params;

namespace blink {
class WebFrame;
}

namespace extensions {
class Dispatcher;

class UserScriptScheduler {
 public:
  UserScriptScheduler(blink::WebFrame* frame, Dispatcher* dispatcher);
  ~UserScriptScheduler();

  void ExecuteCode(const ExtensionMsg_ExecuteCode_Params& params);
  void DidCreateDocumentElement();
  void DidFinishDocumentLoad();
  void DidFinishLoad();
  void DidStartProvisionalLoad();

 private:
  typedef
    std::queue<linked_ptr<ExtensionMsg_ExecuteCode_Params> >
    ExecutionQueue;

  
  
  void MaybeRun();

  
  
  void ExecuteCodeImpl(const ExtensionMsg_ExecuteCode_Params& params);

  
  bool GetAllChildFrames(blink::WebFrame* parent_frame,
                         std::vector<blink::WebFrame*>* frames_vector) const;

  
  void IdleTimeout();

  base::WeakPtrFactory<UserScriptScheduler> weak_factory_;

  
  blink::WebFrame* frame_;

  
  
  UserScript::RunLocation current_location_;

  
  bool has_run_idle_;

  
  std::map<UserScript::RunLocation, ExecutionQueue> pending_execution_map_;

  Dispatcher* dispatcher_;
};

}  

#endif  
