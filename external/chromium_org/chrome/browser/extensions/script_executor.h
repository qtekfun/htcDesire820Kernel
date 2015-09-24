// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_SCRIPT_EXECUTOR_H_
#define CHROME_BROWSER_EXTENSIONS_SCRIPT_EXECUTOR_H_

#include <string>

#include "base/callback_forward.h"
#include "base/observer_list.h"
#include "chrome/browser/extensions/tab_helper.h"
#include "extensions/common/user_script.h"

class GURL;

namespace base {
class ListValue;
}  

namespace content {
class WebContents;
}

namespace extensions {

class ScriptExecutor {
 public:
  ScriptExecutor(
      content::WebContents* web_contents,
      
      
      ObserverList<TabHelper::ScriptExecutionObserver>* script_observers);

  ~ScriptExecutor();

  
  enum ScriptType {
    JAVASCRIPT,
    CSS,
  };

  
  enum FrameScope {
    TOP_FRAME,
    ALL_FRAMES,
  };

  
  enum WorldType {
    MAIN_WORLD,
    ISOLATED_WORLD,
  };

  
  enum ProcessType {
    DEFAULT_PROCESS,
    WEB_VIEW_PROCESS,
  };

  
  enum ResultType {
    NO_RESULT,
    JSON_SERIALIZED_RESULT,
  };

  
  
  typedef base::Callback<void(const std::string&, int32, const GURL&,
                              const base::ListValue&)>
      ExecuteScriptCallback;

  
  
  
  
  
  
  void ExecuteScript(const std::string& extension_id,
                     ScriptType script_type,
                     const std::string& code,
                     FrameScope frame_scope,
                     UserScript::RunLocation run_at,
                     WorldType world_type,
                     ProcessType process_type,
                     const GURL& file_url,
                     ResultType result_type,
                     const ExecuteScriptCallback& callback);

 private:
  
  int next_request_id_;

  content::WebContents* web_contents_;

  ObserverList<TabHelper::ScriptExecutionObserver>* script_observers_;
};

}  

#endif  
