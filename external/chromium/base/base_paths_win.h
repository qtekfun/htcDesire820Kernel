// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_BASE_PATHS_WIN_H__
#define BASE_BASE_PATHS_WIN_H__
#pragma once


namespace base {

enum {
  PATH_WIN_START = 100,

  DIR_WINDOWS,  
  DIR_SYSTEM,   
  DIR_PROGRAM_FILES, 

  DIR_IE_INTERNET_CACHE,  
  DIR_COMMON_START_MENU,  
                          
  DIR_START_MENU,         
                          
  DIR_APP_DATA,           
  DIR_PROFILE,            
  DIR_LOCAL_APP_DATA_LOW,  
  DIR_LOCAL_APP_DATA,  
                       
  PATH_WIN_END
};

}  

#endif  
