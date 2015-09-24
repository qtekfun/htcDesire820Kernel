// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_INSTALL_WARNING_H_
#define EXTENSIONS_COMMON_INSTALL_WARNING_H_

#include <ostream>
#include <string>

namespace extensions {

struct InstallWarning {
  InstallWarning(const std::string& message);
  InstallWarning(const std::string& message,
                 const std::string& key);
  InstallWarning(const std::string& message,
                 const std::string& key,
                 const std::string& specific);
  ~InstallWarning();

  bool operator==(const InstallWarning& other) const {
    
    
    
    
    
    return message == other.message;
  }

  
  std::string message;
  
  
  std::string key;
  
  
  std::string specific;
};

void PrintTo(const InstallWarning&, ::std::ostream* os);

}  

#endif  
