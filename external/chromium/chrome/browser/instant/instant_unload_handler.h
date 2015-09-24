// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTANT_INSTANT_UNLOAD_HANDLER_H_
#define CHROME_BROWSER_INSTANT_INSTANT_UNLOAD_HANDLER_H_
#pragma once

#include "base/memory/scoped_vector.h"

class Browser;
class TabContentsWrapper;

class InstantUnloadHandler {
 public:
  explicit InstantUnloadHandler(Browser* browser);
  ~InstantUnloadHandler();

  
  void RunUnloadListenersOrDestroy(TabContentsWrapper* tab_contents, int index);

 private:
  class TabContentsDelegateImpl;

  
  
  void Activate(TabContentsDelegateImpl* delegate);

  
  void Destroy(TabContentsDelegateImpl* delegate);

  
  
  Browser* browser_;

  ScopedVector<TabContentsDelegateImpl> delegates_;

  DISALLOW_COPY_AND_ASSIGN(InstantUnloadHandler);
};

#endif  
