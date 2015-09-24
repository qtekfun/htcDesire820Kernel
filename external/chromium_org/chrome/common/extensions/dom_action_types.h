// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_DOM_ACTION_TYPES_H_
#define CHROME_COMMON_EXTENSIONS_DOM_ACTION_TYPES_H_

namespace extensions {

struct DomActionType {
  
  
  enum Type {
    GETTER = 0,      
    SETTER = 1,      
    METHOD = 2,      
    INSERTED = 3,    
    XHR = 4,         
    WEBREQUEST = 5,  
    MODIFIED = 6,    
  };
};

}  

#endif  
