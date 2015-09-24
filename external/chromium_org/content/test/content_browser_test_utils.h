// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_CONTENT_BROWSER_TEST_UTILS_H_
#define CONTENT_TEST_CONTENT_BROWSER_TEST_UTILS_H_

#include "base/memory/ref_counted.h"
#include "ui/gfx/native_widget_types.h"
#include "url/gurl.h"

namespace base {
class FilePath;
}

namespace gfx {
class Rect;
}


namespace content {

class MessageLoopRunner;
class Shell;

base::FilePath GetTestFilePath(const char* dir, const char* file);

GURL GetTestUrl(const char* dir, const char* file);

void NavigateToURL(Shell* window, const GURL& url);

void NavigateToURLBlockUntilNavigationsComplete(Shell* window,
                                                const GURL& url,
                                                int number_of_navigations);

void WaitForAppModalDialog(Shell* window);

class ShellAddedObserver {
 public:
  ShellAddedObserver();
  ~ShellAddedObserver();

  
  
  Shell* GetShell();

 private:
  void ShellCreated(Shell* shell);

  Shell* shell_;
  scoped_refptr<MessageLoopRunner> runner_;

  DISALLOW_COPY_AND_ASSIGN(ShellAddedObserver);
};

#if defined OS_MACOSX
void SetWindowBounds(gfx::NativeWindow window, const gfx::Rect& bounds);
#endif

}  

#endif  
