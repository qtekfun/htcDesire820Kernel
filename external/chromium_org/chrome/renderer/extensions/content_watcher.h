// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_CONTENT_WATCHER_H_
#define CHROME_RENDERER_EXTENSIONS_CONTENT_WATCHER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "third_party/WebKit/public/platform/WebVector.h"

namespace blink {
class WebFrame;
class WebString;
}

namespace extensions {
class Dispatcher;
class Extension;
class NativeHandler;

class ContentWatcher {
 public:
  ContentWatcher();
  ~ContentWatcher();

  
  void OnWatchPages(const std::vector<std::string>& css_selectors);

  
  
  
  void DidCreateDocumentElement(blink::WebFrame* frame);

  
  
  void DidMatchCSS(
      blink::WebFrame* frame,
      const blink::WebVector<blink::WebString>& newly_matching_selectors,
      const blink::WebVector<blink::WebString>& stopped_matching_selectors);

 private:
  
  
  
  
  
  void NotifyBrowserOfChange(blink::WebFrame* changed_frame) const;

  
  
  blink::WebVector<blink::WebString> css_selectors_;

  
  
  std::map<blink::WebFrame*, std::set<std::string> > matching_selectors_;
};

}  

#endif  
