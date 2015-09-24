// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOAD_COMPLETE_LISTENER_H_
#define CHROME_BROWSER_UI_VIEWS_LOAD_COMPLETE_LISTENER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class LoadCompleteListener : public content::NotificationObserver {
 public:
  class Delegate {
   public:
    
    virtual void OnLoadCompleted() = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit LoadCompleteListener(Delegate* delegate);

  virtual ~LoadCompleteListener();

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;

  
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(LoadCompleteListener);
};

#endif  
