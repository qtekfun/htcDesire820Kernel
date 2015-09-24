// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SERVICE_SERVICE_PROCESS_CONTROL_MANAGER_H_
#define CHROME_BROWSER_SERVICE_SERVICE_PROCESS_CONTROL_MANAGER_H_

#include <vector>

class Profile;
class ServiceProcessControl;

class ServiceProcessControlManager {
 public:
  typedef std::vector<ServiceProcessControl*> ServiceProcessControlList;

  ServiceProcessControlManager();
  ~ServiceProcessControlManager();

  
  
  
  
  
  
  ServiceProcessControl* GetProcessControl(Profile* profile);

  
  void Shutdown();

  
  static ServiceProcessControlManager* GetInstance();

 private:
  ServiceProcessControlList process_control_list_;
};

#endif  
