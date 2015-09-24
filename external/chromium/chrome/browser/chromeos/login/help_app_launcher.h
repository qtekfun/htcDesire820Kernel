// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_HELP_APP_LAUNCHER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_HELP_APP_LAUNCHER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/login/login_html_dialog.h"
#include "ui/gfx/native_widget_types.h"

namespace chromeos {

class HelpAppLauncher : public LoginHtmlDialog::Delegate,
                        public base::RefCountedThreadSafe<HelpAppLauncher> {
 public:
  
  enum HelpTopic {
    
    HELP_CONNECTIVITY = 188752,
    
    HELP_STATS_USAGE = 183078,
    
    HELP_CANT_ACCESS_ACCOUNT_OFFLINE = 188755,
    
    HELP_CANT_ACCESS_ACCOUNT = 188036,
    
    HELP_ACCOUNT_DISABLED = 188756,
    
    HELP_HOSTED_ACCOUNT = 1054228,
  };

  
  explicit HelpAppLauncher(gfx::NativeWindow parent_window);

  
  void ShowHelpTopic(HelpTopic help_topic_id);

  
  bool is_open() const { return dialog_.get() && dialog_->is_open(); }

 protected:
  
  virtual void OnDialogClosed() {}

 private:
  
  void ShowHelpTopicDialog(const GURL& topic_url);

  
  scoped_ptr<LoginHtmlDialog> dialog_;

  
  gfx::NativeWindow parent_window_;

  DISALLOW_COPY_AND_ASSIGN(HelpAppLauncher);
};

}  

#endif  
