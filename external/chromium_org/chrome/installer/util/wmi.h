// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_INSTALLER_UTIL_WMI_H_
#define CHROME_INSTALLER_UTIL_WMI_H_

#include <string>
#include <wbemidl.h>
#include "base/strings/string16.h"

namespace installer {

class WMI {
 public:
  
  
  
  
  
  
  
  static bool CreateLocalConnection(bool set_blanket,
                                    IWbemServices** wmi_services);

  
  
  
  
  
  
  static bool CreateClassMethodObject(IWbemServices* wmi_services,
                                      const std::wstring& class_name,
                                      const std::wstring& method_name,
                                      IWbemClassObject** class_instance);

  
  
  
  static bool SetParameter(IWbemClassObject* class_method,
                           const std::wstring& parameter_name,
                           VARIANT* parameter);
};

class WMIProcess {
 public:
  
  
  
  
  
  
  
  
  
  static bool Launch(const std::wstring& command_line, int* process_id);
};

class WMIComputerSystem {
 public:
  
  static string16 GetModel();
};

}  

#endif  
