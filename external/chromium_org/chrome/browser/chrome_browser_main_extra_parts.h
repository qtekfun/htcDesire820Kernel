// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_BROWSER_MAIN_EXTRA_PARTS_H_
#define CHROME_BROWSER_CHROME_BROWSER_MAIN_EXTRA_PARTS_H_




class ChromeBrowserMainExtraParts {
 public:
  virtual ~ChromeBrowserMainExtraParts() {}

  
  virtual void PreEarlyInitialization() {}
  virtual void PostEarlyInitialization() {}

  
  virtual void ToolkitInitialized() {}

  
  virtual void PreMainMessageLoopStart() {}
  virtual void PostMainMessageLoopStart() {}

  
  virtual void PreCreateThreads() {}
  virtual void PreProfileInit() {}
  virtual void PostProfileInit() {}
  virtual void PreBrowserStart() {}
  virtual void PostBrowserStart() {}
  virtual void PreMainMessageLoopRun() {}
  virtual void PostMainMessageLoopRun() {}
};

#endif  
