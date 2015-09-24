// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTO_LOGIN_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_UI_AUTO_LOGIN_INFOBAR_DELEGATE_H_

#include <string>
#include "chrome/browser/infobars/confirm_infobar_delegate.h"
#include "components/auto_login_parser/auto_login_parser.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class PrefService;
class Profile;

namespace content {
class NavigationController;
}

class AutoLoginInfoBarDelegate : public ConfirmInfoBarDelegate,
                                 public content::NotificationObserver {
 public:
  struct Params {
    
    auto_login_parser::HeaderData header;

    
    
    
    std::string username;
  };

  
  
  
  static bool Create(content::WebContents* web_contents, const Params& params);

 protected:
  AutoLoginInfoBarDelegate(const Params& params, Profile* profile);
  virtual ~AutoLoginInfoBarDelegate();

 private:
  
  enum Actions {
    SHOWN,       
    ACCEPTED,    
    REJECTED,    
    DISMISSED,   
    IGNORED,     
    LEARN_MORE,  
    HISTOGRAM_BOUNDING_VALUE
  };

  
  virtual void InfoBarDismissed() OVERRIDE;
  virtual int GetIconID() const OVERRIDE;
  virtual Type GetInfoBarType() const OVERRIDE;
  virtual AutoLoginInfoBarDelegate* AsAutoLoginInfoBarDelegate() OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void RecordHistogramAction(Actions action);

  const Params params_;

  
  bool button_pressed_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(AutoLoginInfoBarDelegate);
};

#endif  
