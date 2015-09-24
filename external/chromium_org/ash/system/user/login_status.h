// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_USER_LOGIN_STATUS_H_
#define ASH_SYSTEM_USER_LOGIN_STATUS_H_

#include "base/strings/string16.h"

namespace ash {
namespace user {

enum LoginStatus {
  LOGGED_IN_NONE,             
  LOGGED_IN_LOCKED,           
  LOGGED_IN_USER,             
  LOGGED_IN_OWNER,            
  LOGGED_IN_GUEST,            
  LOGGED_IN_RETAIL_MODE,      
  LOGGED_IN_PUBLIC,           
  LOGGED_IN_LOCALLY_MANAGED,  
  LOGGED_IN_KIOSK_APP         
};

base::string16 GetLocalizedSignOutStringForStatus(LoginStatus status,
                                                  bool multiline);

}  
}  

#endif  
