// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_SEARCH_PROVIDER_H_
#define CHROME_COMMON_SEARCH_PROVIDER_H_

namespace search_provider {

enum OSDDType {
  
  AUTODETECTED_PROVIDER,

  
  EXPLICIT_PROVIDER,
};

enum InstallState {
  
  DENIED = -1,

  
  
  

  
  NOT_INSTALLED = 0,

  
  INSTALLED_BUT_NOT_DEFAULT = 1,

  
  INSTALLED_AS_DEFAULT = 2
};

}  

#endif  
