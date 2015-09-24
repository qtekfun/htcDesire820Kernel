// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_TRANSLATE_TRANSLATE_ERRORS_H_
#define CHROME_COMMON_TRANSLATE_TRANSLATE_ERRORS_H_

#include "base/basictypes.h"


class TranslateErrors {
 public:
  enum Type {
    NONE = 0,
    NETWORK,                  
    INITIALIZATION_ERROR,     
    UNKNOWN_LANGUAGE,         
    UNSUPPORTED_LANGUAGE,     
                              
    IDENTICAL_LANGUAGES,      
    TRANSLATION_ERROR,        
                              
    TRANSLATION_TIMEOUT,      
    UNEXPECTED_SCRIPT_ERROR,  
    BAD_ORIGIN,               
    SCRIPT_LOAD_ERROR,        
    TRANSLATE_ERROR_MAX,
  };

 private:
  TranslateErrors() {}

  DISALLOW_COPY_AND_ASSIGN(TranslateErrors);
};

#endif  
