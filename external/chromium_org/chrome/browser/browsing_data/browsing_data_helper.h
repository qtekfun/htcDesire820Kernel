// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_HELPER_H_

#include <string>

#include "base/basictypes.h"

class ExtensionSpecialStoragePolicy;
class GURL;

class BrowsingDataHelper {
 public:
  enum OriginSetMask {
    UNPROTECTED_WEB = 1 << 0,  
    PROTECTED_WEB = 1 << 1,    
    EXTENSION = 1 << 2,        
    
    ALL = UNPROTECTED_WEB | PROTECTED_WEB | EXTENSION,
  };

  
  
  
  
  static bool IsWebScheme(const std::string& scheme);
  static bool HasWebScheme(const GURL& origin);

  
  static bool IsExtensionScheme(const std::string& scheme);
  static bool HasExtensionScheme(const GURL& origin);

  
  static bool DoesOriginMatchMask(const GURL& origin,
                                  int origin_set_mask,
                                  ExtensionSpecialStoragePolicy* policy);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(BrowsingDataHelper);
};

#endif  
