// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_OMAHA_QUERY_PARAMS_OMAHA_QUERY_PARAMS_H_
#define CHROME_COMMON_OMAHA_QUERY_PARAMS_OMAHA_QUERY_PARAMS_H_

#include <string>

#include "base/basictypes.h"

namespace chrome {

class OmahaQueryParams {
 public:
  enum ProdId {
    CHROME = 0,
    CRX,
  };

  
  
  
  static std::string Get(ProdId prod);

  
  
  static const char* GetProdIdString(chrome::OmahaQueryParams::ProdId prod);

  
  
  static const char* getOS();

  
  
  static const char* getArch();

  
  
  
  
  static const char* getNaclArch();

  
  
  
  static const char* GetChannelString();

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(OmahaQueryParams);
};

}  

#endif  
