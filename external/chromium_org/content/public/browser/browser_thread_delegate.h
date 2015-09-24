// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_THREAD_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_THREAD_DELEGATE_H_

namespace content {

class BrowserThreadDelegate {
 public:
  virtual ~BrowserThreadDelegate() {}

  
  virtual void Init() = 0;

  
  virtual void InitAsync() = 0;

  
  virtual void CleanUp() = 0;
};

}  

#endif  
