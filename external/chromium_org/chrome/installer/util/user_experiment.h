// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_USER_EXPERIMENT_H_
#define CHROME_INSTALLER_UTIL_USER_EXPERIMENT_H_

#include "base/strings/string16.h"
#include "chrome/installer/util/util_constants.h"

class CommandLine;

namespace base {
class FilePath;
}

namespace installer {

class Product;

enum ToastUiFlags {
  kToastUiUninstall          = 1 << 0,  
  kToastUiDontBugMeAsButton  = 1 << 1,  
  kToastUiWhyLink            = 1 << 2,  
  kToastUiMakeDefault        = 1 << 3,  
};

struct ExperimentDetails {
  string16 prefix;      
                        
  int flavor;           
  int heading;          
  int flags;            
  int control_group;    
                        
                        
};

bool CreateExperimentDetails(int flavor, ExperimentDetails* experiment);

void LaunchBrowserUserExperiment(const CommandLine& base_command,
                                 InstallStatus status,
                                 bool system_level);

void InactiveUserToastExperiment(int flavor,
                                 const string16& experiment_group,
                                 const Product& product,
                                 const base::FilePath& application_path);

}  

#endif  
