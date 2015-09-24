// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_JAVA_JAVA_TYPE_H_
#define CONTENT_BROWSER_RENDERER_HOST_JAVA_JAVA_TYPE_H_

#include <string>

#include "base/memory/scoped_ptr.h"

namespace content {

struct JavaType {
  JavaType();
  JavaType(const JavaType& other);
  ~JavaType();
  JavaType& operator=(const JavaType& other);

  
  
  static JavaType CreateFromBinaryName(const std::string& binary_name);

  enum Type {
    TypeBoolean,
    TypeByte,
    TypeChar,
    TypeShort,
    TypeInt,
    TypeLong,
    TypeFloat,
    TypeDouble,
    
    
    TypeVoid,
    TypeArray,
    
    TypeString,
    TypeObject,
  };

  Type type;
  scoped_ptr<JavaType> inner_type;  
};

}  

#endif  
