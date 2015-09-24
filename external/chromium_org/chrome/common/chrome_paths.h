// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CHROME_PATHS_H__
#define CHROME_COMMON_CHROME_PATHS_H__

#include "build/build_config.h"


namespace chrome {

enum {
  PATH_START = 1000,

  DIR_APP = PATH_START,         
  DIR_LOGS,                     // Directory where logs should be written.
  DIR_USER_DATA,                // Directory where user data can be written.
  DIR_CRASH_DUMPS,              // Directory where crash dumps are written.
  DIR_RESOURCES,                
                                
  DIR_INSPECTOR,                
  DIR_APP_DICTIONARIES,         
  DIR_USER_DOCUMENTS,           
  DIR_USER_MUSIC,               
  DIR_USER_PICTURES,            
  DIR_USER_VIDEOS,              
  DIR_DEFAULT_DOWNLOADS_SAFE,   
                                
                                
  DIR_DEFAULT_DOWNLOADS,        
  DIR_INTERNAL_PLUGINS,         
#if defined(OS_POSIX) && !defined(OS_MACOSX)
  DIR_POLICY_FILES,             
                                
                                
                                
#endif
#if defined(OS_MACOSX) && !defined(OS_IOS)
  DIR_MANAGED_PREFS,            
                                
#endif
#if defined(OS_CHROMEOS) || (defined(OS_MACOSX) && !defined(OS_IOS))
  DIR_USER_EXTERNAL_EXTENSIONS,  
                                 
                                 
                                 
#endif

#if defined(OS_LINUX)
  DIR_STANDALONE_EXTERNAL_EXTENSIONS,  
                                       
                                       
                                       
#endif
  DIR_EXTERNAL_EXTENSIONS,      

  DIR_DEFAULT_APPS,             
                                
  DIR_PEPPER_FLASH_PLUGIN,      
                                
  DIR_COMPONENT_UPDATED_PEPPER_FLASH_PLUGIN,  
                                              
                                              
  FILE_RESOURCE_MODULE,         
                                
                                
  FILE_LOCAL_STATE,             
                                
  FILE_RECORDED_SCRIPT,         
                                
                                
  FILE_FLASH_PLUGIN,            
                                
                                
  FILE_PEPPER_FLASH_PLUGIN,     
                                
  FILE_PDF_PLUGIN,              

  FILE_NACL_PLUGIN,             
  DIR_PNACL_BASE,               
  DIR_PNACL_COMPONENT,          
                                
  FILE_O1D_PLUGIN,              
  FILE_EFFECTS_PLUGIN,          
  FILE_GTALK_PLUGIN,            
  DIR_COMPONENT_WIDEVINE_CDM,   
                                
  FILE_WIDEVINE_CDM_ADAPTER,    
  FILE_RESOURCES_PACK,          
                                
                                
  DIR_RESOURCES_EXTENSION,      
#if defined(OS_CHROMEOS)
  DIR_CHROMEOS_WALLPAPERS,      
                                
  DIR_CHROMEOS_WALLPAPER_THUMBNAILS,  
                                      
  DIR_CHROMEOS_CUSTOM_WALLPAPERS,     
                                      
#endif
  DIR_MANAGED_USERS_DEFAULT_APPS,  
                                   
                                   

  
  DIR_GEN_TEST_DATA,            
  DIR_TEST_DATA,                
  DIR_TEST_TOOLS,               

  PATH_END
};

void RegisterPathProvider();

}  

#endif  
