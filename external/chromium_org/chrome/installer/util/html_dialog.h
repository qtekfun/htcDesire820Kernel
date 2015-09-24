// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_HTML_DIALOG_H_
#define CHROME_INSTALLER_UTIL_HTML_DIALOG_H_

#include <string>

#include "base/basictypes.h"


namespace installer {

class HTMLDialog {
 public:
  enum DialogResult {
    HTML_DLG_ERROR    = 0,  
    HTML_DLG_ACCEPT   = 1,  
    HTML_DLG_DECLINE  = 2,  
    HTML_DLG_RETRY    = 3,  
    HTML_DLG_IGNORE   = 4,  
    HTML_DLG_TIMEOUT  = 5,  
    HTML_DLG_EXTRA    = 6   
  };

  
  class CustomizationCallback {
   public:
    
    
    virtual void OnBeforeCreation(void** extra) = 0;
    
    
    virtual void OnBeforeDisplay(void* window) = 0;

    protected:
     virtual ~CustomizationCallback() {}
  };

  virtual ~HTMLDialog() {}

  
  
  
  
  virtual DialogResult ShowModal(void* parent_window,
                                 CustomizationCallback* callback) = 0;

  
  
  virtual std::wstring GetExtraResult() = 0;
};

HTMLDialog* CreateNativeHTMLDialog(const std::wstring& url);

class EulaHTMLDialog {
 public:
  
  
  
  EulaHTMLDialog(const std::wstring& file, const std::wstring& param);
  ~EulaHTMLDialog();

  enum Outcome {
    REJECTED,           
    ACCEPTED,           
    ACCEPTED_OPT_IN,    
  };

  
  
  Outcome ShowModal();

 private:
  class Customizer : public HTMLDialog::CustomizationCallback {
   public:
    virtual void OnBeforeCreation(void** extra);
    virtual void OnBeforeDisplay(void* window);
  };

  HTMLDialog* dialog_;
  DISALLOW_COPY_AND_ASSIGN(EulaHTMLDialog);
};

}  

#endif  
