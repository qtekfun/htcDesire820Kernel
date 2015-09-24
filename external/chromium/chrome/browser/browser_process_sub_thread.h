// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSER_PROCESS_SUB_THREAD_H_
#define CHROME_BROWSER_BROWSER_PROCESS_SUB_THREAD_H_
#pragma once

#include "base/basictypes.h"
#include "content/browser/browser_thread.h"

class NotificationService;

class BrowserProcessSubThread : public BrowserThread {
 public:
  explicit BrowserProcessSubThread(BrowserThread::ID identifier);
  virtual ~BrowserProcessSubThread();

 protected:
  virtual void Init();
  virtual void CleanUp();

 private:
  
  
  
  NotificationService* notification_service_;

  DISALLOW_COPY_AND_ASSIGN(BrowserProcessSubThread);
};

#endif  
