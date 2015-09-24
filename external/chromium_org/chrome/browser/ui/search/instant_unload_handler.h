// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_INSTANT_UNLOAD_HANDLER_H_
#define CHROME_BROWSER_UI_SEARCH_INSTANT_UNLOAD_HANDLER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"

class Browser;

namespace content {
class WebContents;
}

class InstantUnloadHandler {
 public:
  explicit InstantUnloadHandler(Browser* browser);
  ~InstantUnloadHandler();

  
  void RunUnloadListenersOrDestroy(scoped_ptr<content::WebContents> contents,
                                   int index);

 private:
  class WebContentsDelegateImpl;

  
  
  void Activate(WebContentsDelegateImpl* delegate,
                scoped_ptr<content::WebContents> contents,
                int index);

  
  void Destroy(WebContentsDelegateImpl* delegate);

  
  
  Browser* const browser_;

  ScopedVector<WebContentsDelegateImpl> delegates_;

  DISALLOW_COPY_AND_ASSIGN(InstantUnloadHandler);
};

#endif  
