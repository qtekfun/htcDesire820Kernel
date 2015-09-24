// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_CHROMEOS_PATHS_H_
#define CHROMEOS_CHROMEOS_PATHS_H_

#include "chromeos/chromeos_export.h"


namespace chromeos {

enum {
  PATH_START = 7000,

  FILE_DEFAULT_APP_ORDER,       
                                
  DIR_USER_POLICY_KEYS,         
                                
  FILE_OWNER_KEY,               
  FILE_INSTALL_ATTRIBUTES,      
  FILE_UPTIME,                  
                                
  FILE_UPDATE_REBOOT_NEEDED_UPTIME,  
                                     
                                     
                                     
  DIR_DEVICE_LOCAL_ACCOUNT_EXTENSIONS,  
                                        
                                        
                                        
  DIR_DEVICE_LOCAL_ACCOUNT_EXTERNAL_DATA,  
                                           
                                           
  PATH_END
};

CHROMEOS_EXPORT void RegisterPathProvider();

}  

#endif  
