// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_MESSAGING_BINDINGS_H_
#define CHROME_RENDERER_EXTENSIONS_MESSAGING_BINDINGS_H_

#include <string>

#include "chrome/renderer/extensions/chrome_v8_context_set.h"

namespace base {
class DictionaryValue;
}

namespace content {
class RenderView;
}

namespace v8 {
class Extension;
}

namespace extensions {
class ChromeV8Extension;
class Dispatcher;
struct Message;

class MessagingBindings {
 public:
  
  static ChromeV8Extension* Get(Dispatcher* dispatcher,
                                ChromeV8Context* context);

  
  
  
  static void DispatchOnConnect(
      const ChromeV8ContextSet::ContextSet& contexts,
      int target_port_id,
      const std::string& channel_name,
      const base::DictionaryValue& source_tab,
      const std::string& source_extension_id,
      const std::string& target_extension_id,
      const GURL& source_url,
      const std::string& tls_channel_id,
      content::RenderView* restrict_to_render_view);

  
  
  
  static void DeliverMessage(
      const ChromeV8ContextSet::ContextSet& context_set,
      int target_port_id,
      const Message& message,
      content::RenderView* restrict_to_render_view);

  
  static void DispatchOnDisconnect(
      const ChromeV8ContextSet::ContextSet& context_set,
      int port_id,
      const std::string& error_message,
      content::RenderView* restrict_to_render_view);
};

}  

#endif  
