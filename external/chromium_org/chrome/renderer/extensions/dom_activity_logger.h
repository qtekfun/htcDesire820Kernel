// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_DOM_ACTIVITY_LOGGER_H_
#define CHROME_RENDERER_EXTENSIONS_DOM_ACTIVITY_LOGGER_H_

#include <string>

#include "base/strings/string_piece.h"
#include "third_party/WebKit/public/platform/WebString.h"
#include "third_party/WebKit/public/platform/WebURL.h"
#include "third_party/WebKit/public/web/WebDOMActivityLogger.h"
#include "url/gurl.h"
#include "v8/include/v8.h"

namespace extensions {

class DOMActivityLogger: public blink::WebDOMActivityLogger {
 public:
  static const int kMainWorldId = 0;
  explicit DOMActivityLogger(const std::string& extension_id);

  
  
  
  
  virtual void log(const blink::WebString& api_name,
                   int argc,
                   const v8::Handle<v8::Value> argv[],
                   const blink::WebString& call_type,
                   const blink::WebURL& url,
                   const blink::WebString& title);

  
  
  
  static void AttachToWorld(int world_id,
                            const std::string& extension_id);

 private:
  std::string extension_id_;

  DISALLOW_COPY_AND_ASSIGN(DOMActivityLogger);
};

}  

#endif  

