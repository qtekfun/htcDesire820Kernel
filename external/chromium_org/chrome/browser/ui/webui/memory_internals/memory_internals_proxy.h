// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_MEMORY_INTERNALS_MEMORY_INTERNALS_PROXY_H_
#define CHROME_BROWSER_UI_WEBUI_MEMORY_INTERNALS_MEMORY_INTERNALS_PROXY_H_

#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "base/values.h"
#include "chrome/browser/memory_details.h"
#include "content/public/browser/browser_thread.h"

class MemoryInternalsHandler;
class RendererDetails;

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

namespace content {
class WebContents;
}

class MemoryInternalsProxy
    : public base::RefCountedThreadSafe<
        MemoryInternalsProxy, content::BrowserThread::DeleteOnUIThread> {
 public:
  MemoryInternalsProxy();

  
  void Attach(MemoryInternalsHandler* handler);

  
  void Detach();

  
  void StartFetch(const base::ListValue* list);

 private:
  friend struct
      content::BrowserThread::DeleteOnThread<content::BrowserThread::UI>;
  friend class base::DeleteHelper<MemoryInternalsProxy>;

  typedef ProcessMemoryInformationList::const_iterator PMIIterator;

  virtual ~MemoryInternalsProxy();

  
  void OnProcessAvailable(const ProcessData& browser);

  
  void OnRendererAvailable(const base::ProcessId pid,
                           const size_t v8_allocated,
                           const size_t v8_used);

  
  
  
  void ConvertTabsInformation(
      const std::set<content::WebContents*>& web_contents,
      base::ListValue* processes);

  
  void RequestRendererDetails();

  
  void FinishCollection();

  
  void CallJavaScriptFunctionOnUIThread(const std::string& function,
                                        const base::Value& args);

  MemoryInternalsHandler* handler_;
  base::DictionaryValue* information_;
  RendererDetails* renderer_details_;

  DISALLOW_COPY_AND_ASSIGN(MemoryInternalsProxy);
};

#endif  
