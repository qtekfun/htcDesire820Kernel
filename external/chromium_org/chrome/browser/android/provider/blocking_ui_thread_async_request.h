// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_PROVIDER_BLOCKING_UI_THREAD_ASYNC_REQUEST_H_
#define CHROME_BROWSER_ANDROID_PROVIDER_BLOCKING_UI_THREAD_ASYNC_REQUEST_H_

#include "base/bind.h"
#include "base/callback.h"
#include "base/synchronization/waitable_event.h"
#include "chrome/browser/android/provider/run_on_ui_thread_blocking.h"
#include "content/public/browser/browser_thread.h"

class BlockingUIThreadAsyncRequest {
 public:
  BlockingUIThreadAsyncRequest();

  
  
  
  
  template <typename Signature>
  void RunAsyncRequestOnUIThreadBlocking(base::Callback<Signature> request) {
    DCHECK(!content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));

    
    request_completed_.Reset();
    RunOnUIThreadBlocking::Run(
        base::Bind(
            &BlockingUIThreadAsyncRequest::RunRequestOnUIThread<Signature>,
            request));

    
    request_completed_.Wait();
  }

  
  void RequestCompleted();

 private:
  template <typename Signature>
  static void RunRequestOnUIThread(base::Callback<Signature> request) {
    request.Run();
  }

  base::WaitableEvent request_completed_;

  DISALLOW_COPY_AND_ASSIGN(BlockingUIThreadAsyncRequest);
};

#endif  
