// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_REMOVE_USER_DELEGATE_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_REMOVE_USER_DELEGATE_H_

namespace chromeos {

class RemoveUserDelegate {
 public:
  
  virtual void OnBeforeUserRemoved(const std::string& username) = 0;

  
  virtual void OnUserRemoved(const std::string& username) = 0;
};

}  

#endif  
