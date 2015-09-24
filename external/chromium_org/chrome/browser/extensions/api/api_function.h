// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_API_FUNCTION_H_
#define CHROME_BROWSER_EXTENSIONS_API_API_FUNCTION_H_

#include "chrome/browser/extensions/chrome_extension_function.h"
#include "content/public/browser/browser_thread.h"

namespace extensions {

class ApiFunction : public ChromeAsyncExtensionFunction {
 protected:
  ApiFunction();
  virtual ~ApiFunction();
};

class AsyncApiFunction : public ApiFunction {
 protected:
  AsyncApiFunction();
  virtual ~AsyncApiFunction();

  
  
  virtual bool PrePrepare();

  
  
  virtual bool Prepare() = 0;

  
  
  virtual void Work();

  
  virtual void AsyncWorkStart();

  
  void AsyncWorkCompleted();

  
  virtual bool Respond() = 0;

  
  virtual bool RunImpl() OVERRIDE;

 protected:
  void set_work_thread_id(content::BrowserThread::ID work_thread_id) {
    work_thread_id_ = work_thread_id;
  }

 private:
  void WorkOnWorkThread();
  void RespondOnUIThread();

  
  
  content::BrowserThread::ID work_thread_id_;
};

}  

#endif  
