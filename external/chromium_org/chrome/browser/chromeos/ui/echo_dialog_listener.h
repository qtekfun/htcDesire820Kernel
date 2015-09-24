// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_UI_ECHO_DIALOG_LISTENER_H_
#define CHROME_BROWSER_CHROMEOS_UI_ECHO_DIALOG_LISTENER_H_

namespace chromeos {

class EchoDialogListener {
 public:
  
  
  virtual void OnAccept() = 0;

  
  
  virtual void OnCancel() = 0;

  
  virtual void OnMoreInfoLinkClicked() = 0;

 protected:
  virtual ~EchoDialogListener() {}
};

}  

#endif  
