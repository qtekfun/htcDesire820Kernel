// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_OFFLINE_OFFLINE_LOAD_PAGE_H_
#define CHROME_BROWSER_CHROMEOS_OFFLINE_OFFLINE_LOAD_PAGE_H_
#pragma once

#include <string>

#include "base/task.h"
#include "chrome/browser/chromeos/network_state_notifier.h"
#include "content/browser/tab_contents/interstitial_page.h"

class DictionaryValue;
class Extension;
class TabContents;

namespace chromeos {

class OfflineLoadPage : public InterstitialPage {
 public:
  
  
  class Delegate {
   public:
    Delegate() {}
    virtual ~Delegate() {}
    
    
    virtual void OnBlockingPageComplete(bool proceed) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };
  static void Show(int process_host_id, int render_view_id,
                   const GURL& url, Delegate* delegate);
  
  using InterstitialPage::Show;

 protected:
  
  OfflineLoadPage(TabContents* tab_contents, const GURL& url,
                  Delegate* delegate);
  virtual ~OfflineLoadPage() {}

  
  void EnableTest() {
    in_test_ = true;
  }

 private:
  
  virtual std::string GetHTMLContents();
  virtual void CommandReceived(const std::string& command);
  virtual void Proceed();
  virtual void DontProceed();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void GetAppOfflineStrings(const Extension* app,
                            const string16& faield_url,
                            DictionaryValue* strings) const;
  void GetNormalOfflineStrings(const string16& faield_url,
                               DictionaryValue* strings) const;

  
  
  bool ShowActivationMessage();

  Delegate* delegate_;
  NotificationRegistrar registrar_;

  
  bool proceeded_;
  ScopedRunnableMethodFactory<OfflineLoadPage> method_factory_;

  bool in_test_;

  DISALLOW_COPY_AND_ASSIGN(OfflineLoadPage);
};

}  

#endif  
