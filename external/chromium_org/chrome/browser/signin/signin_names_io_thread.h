// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_NAMES_IO_THREAD_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_NAMES_IO_THREAD_H_

#include <set>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace content {
class NotificationDetails;
class NotificationSource;
}


class SigninNamesOnIOThread : public content::NotificationObserver {
 public:
  typedef std::set<base::string16> EmailSet;

  
  SigninNamesOnIOThread();
  virtual ~SigninNamesOnIOThread();

  
  
  const EmailSet& GetEmails() const;

  
  
  void ReleaseResourcesOnUIThread();

 private:
  
  virtual void Observe(int type,
                        const content::NotificationSource& source,
                        const content::NotificationDetails& details) OVERRIDE;

  
  void CheckOnIOThread() const;

  
  void CheckOnUIThread() const;

  
  
  void PostTaskToIOThread(int type, const base::string16& email);

  
  void UpdateOnIOThread(int type, const base::string16& email);

  scoped_ptr<content::NotificationRegistrar> registrar_;
  EmailSet emails_;

  DISALLOW_COPY_AND_ASSIGN(SigninNamesOnIOThread);
};

#endif  
