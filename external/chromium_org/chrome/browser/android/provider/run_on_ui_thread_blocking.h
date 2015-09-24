// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_PROVIDER_RUN_ON_UI_THREAD_BLOCKING_H_
#define CHROME_BROWSER_ANDROID_PROVIDER_RUN_ON_UI_THREAD_BLOCKING_H_

#include "base/bind.h"
#include "base/callback.h"
#include "base/synchronization/waitable_event.h"
#include "content/public/browser/browser_thread.h"

class RunOnUIThreadBlocking {
 public:
  
  
  template <typename Signature>
  static void Run(base::Callback<Signature> runnable) {
    DCHECK(!content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
    base::WaitableEvent finished(false, false);
    content::BrowserThread::PostTask(content::BrowserThread::UI, FROM_HERE,
          base::Bind(&RunOnUIThreadBlocking::RunOnUIThread<Signature>,
                     runnable, &finished));
    finished.Wait();
  }

 private:
  template <typename Signature>
  static void RunOnUIThread(base::Callback<Signature> runnable,
                            base::WaitableEvent* finished) {
    DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
    runnable.Run();
    finished->Signal();
  }
};

#endif  
