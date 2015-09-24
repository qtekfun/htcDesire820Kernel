// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CHROME_PATHS_H__
#define CHROME_COMMON_CHROME_PATHS_H__
#pragma once

#include "build/build_config.h"


namespace chrome {

enum {
  PATH_START = 1000,

  DIR_APP = PATH_START,         
  DIR_LOGS,                     // Directory where logs should be written.
  DIR_USER_DATA,                // Directory where user data can be written.
  DIR_CRASH_DUMPS,              // Directory where crash dumps are written.
  DIR_USER_DESKTOP,             
  DIR_RESOURCES,                
                                
  DIR_SHARED_RESOURCES,         
                                
  DIR_INSPECTOR,                
  DIR_APP_DICTIONARIES,         
  DIR_USER_DOCUMENTS,           
  DIR_DEFAULT_DOWNLOADS_SAFE,   
                                
  DIR_DEFAULT_DOWNLOADS,        
  DIR_USER_DATA_TEMP,           
                                
                                
                                
                                
                                
                                
  DIR_INTERNAL_PLUGINS,         
  DIR_MEDIA_LIBS,               
#if !defined(OS_MACOSX) && defined(OS_POSIX)
  DIR_POLICY_FILES,             
                                
                                
                                
#endif
#if defined(OS_MACOSX)
  DIR_MANAGED_PREFS,            
                                
#endif
#if defined(OS_CHROMEOS)
  DIR_USER_EXTERNAL_EXTENSIONS,  
                                 
                                 
#endif

  FILE_RESOURCE_MODULE,         
                                
                                
  FILE_LOCAL_STATE,             
                                
  FILE_RECORDED_SCRIPT,         
                                
                                
  FILE_FLASH_PLUGIN,            
  FILE_PDF_PLUGIN,              
  FILE_NACL_PLUGIN,             
  FILE_LIBAVCODEC,              
                                
  FILE_LIBAVFORMAT,             
                                
  FILE_LIBAVUTIL,               
  FILE_RESOURCES_PACK,          
                                
                                
#if defined(OS_CHROMEOS)
  FILE_CHROMEOS_API,            
#endif


  
  DIR_TEST_DATA,                
  DIR_TEST_TOOLS,               

  PATH_END
};

void RegisterPathProvider();

}  

#endif  
