// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_HISTORY_TYPES_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_HISTORY_TYPES_H_

#include <map>
#include <string>

namespace app_list {

enum KnownResultType {
  UNKNOWN_RESULT = 0,
  PERFECT_PRIMARY,    
  PERFECT_SECONDARY,  
  PREFIX_PRIMARY,     
  PREFIX_SECONDARY,   
};

typedef std::map<std::string, KnownResultType> KnownResults;

}  

#endif  
