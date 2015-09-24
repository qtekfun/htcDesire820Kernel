// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_MEDIA_INTERNALS_HANDLER_H_
#define CONTENT_BROWSER_MEDIA_MEDIA_INTERNALS_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class ListValue;
}

namespace content {
class MediaInternalsProxy;

class MediaInternalsMessageHandler : public WebUIMessageHandler {
 public:
  MediaInternalsMessageHandler();
  virtual ~MediaInternalsMessageHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void OnGetEverything(const base::ListValue* list);

  
  void OnUpdate(const base::string16& update);

 private:
  scoped_refptr<MediaInternalsProxy> proxy_;

  
  
  bool page_load_complete_;

  DISALLOW_COPY_AND_ASSIGN(MediaInternalsMessageHandler);
};

}  

#endif  
