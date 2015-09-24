// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_RESOURCE_TYPE_H__
#define WEBKIT_GLUE_RESOURCE_TYPE_H__

#include "base/basictypes.h"

class ResourceType {
 public:
  enum Type {
    MAIN_FRAME = 0,  
    SUB_FRAME,       
    STYLESHEET,      
    SCRIPT,          
    IMAGE,           
    FONT_RESOURCE,   
    SUB_RESOURCE,    
    OBJECT,          
                     
    MEDIA,           
    WORKER,          
    SHARED_WORKER,   
    PREFETCH,        
    FAVICON,         
    LAST_TYPE        
                     
  };

  static bool ValidType(int32 type) {
    return type >= MAIN_FRAME && type < LAST_TYPE;
  }

  static Type FromInt(int32 type) {
    return static_cast<Type>(type);
  }

  static bool IsFrame(ResourceType::Type type) {
    return type == MAIN_FRAME || type == SUB_FRAME;
  }

  static bool IsSharedWorker(ResourceType::Type type) {
    return type == SHARED_WORKER;
  }

  static bool IsSubresource(ResourceType::Type type) {
    return type == STYLESHEET ||
           type == SCRIPT ||
           type == IMAGE ||
           type == FONT_RESOURCE ||
           type == SUB_RESOURCE ||
           type == WORKER;
  }

 private:
  
  ResourceType();
  ~ResourceType();
};
#endif  
