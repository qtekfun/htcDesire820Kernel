// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_UTIL_CONSTANTS_H_
#define CHROME_INSTALLER_UTIL_UTIL_CONSTANTS_H_

#include "base/basictypes.h"

namespace installer {

enum InstallStatus {
  FIRST_INSTALL_SUCCESS,  
  INSTALL_REPAIRED,       
  NEW_VERSION_UPDATED,    
  EXISTING_VERSION_LAUNCHED,  
  HIGHER_VERSION_EXISTS,  
  USER_LEVEL_INSTALL_EXISTS,  
  SYSTEM_LEVEL_INSTALL_EXISTS,  
  INSTALL_FAILED,         
  SETUP_PATCH_FAILED,     
  OS_NOT_SUPPORTED,       
  OS_ERROR,               
  TEMP_DIR_FAILED,        
  UNCOMPRESSION_FAILED,   
  INVALID_ARCHIVE,        
  INSUFFICIENT_RIGHTS,    
  CHROME_NOT_INSTALLED,   
                          
  CHROME_RUNNING,         
                          
  UNINSTALL_CONFIRMED,    
  UNINSTALL_DELETE_PROFILE,  
  UNINSTALL_SUCCESSFUL,   
  UNINSTALL_FAILED,       
  UNINSTALL_CANCELLED,    
  UNKNOWN_STATUS,         
  RENAME_SUCCESSFUL,      
  RENAME_FAILED,          
  EULA_REJECTED,          
  EULA_ACCEPTED,          
  EULA_ACCEPTED_OPT_IN,   
  INSTALL_DIR_IN_USE,     
                          
  UNINSTALL_REQUIRES_REBOOT,  
  IN_USE_UPDATED,         
                          
  SAME_VERSION_REPAIR_FAILED,  
  REENTRY_SYS_UPDATE,     
                          
  SXS_OPTION_NOT_SUPPORTED,  
                             
  NON_MULTI_INSTALLATION_EXISTS,  
                                  
                                  
                                  
  MULTI_INSTALLATION_EXISTS,  
                              
                              
                              
  READY_MODE_OPT_IN_FAILED,   
  READY_MODE_TEMP_OPT_OUT_FAILED,   
                                    
  READY_MODE_END_TEMP_OPT_OUT_FAILED,   
                                        
  CONFLICTING_CHANNEL_EXISTS,  
                               
  READY_MODE_REQUIRES_CHROME,  
  APP_HOST_REQUIRES_MULTI_INSTALL,  
                               
  APPLY_DIFF_PATCH_FAILED,     
  INCONSISTENT_UPDATE_POLICY,  
  APP_HOST_REQUIRES_USER_LEVEL,  
  APP_HOST_REQUIRES_BINARIES,  
  INSTALL_OF_GOOGLE_UPDATE_FAILED,  
  INVALID_STATE_FOR_OPTION,    
                               
  WAIT_FOR_EXISTING_FAILED,    
  PATCH_INVALID_ARGUMENTS,     
                               
  DIFF_PATCH_SOURCE_MISSING,   
                               
  UNUSED_BINARIES,             
                               
                               
  UNUSED_BINARIES_UNINSTALLED,  
  UNSUPPORTED_OPTION,          
  
};


COMPILE_ASSERT(installer::UNSUPPORTED_OPTION == 53,
               dont_change_enum);

enum ArchiveType {
  UNKNOWN_ARCHIVE_TYPE,     
  FULL_ARCHIVE_TYPE,        
  INCREMENTAL_ARCHIVE_TYPE  
};

enum InstallerStage {
  NO_STAGE,                    
  PRECONDITIONS,               
  UNCOMPRESSING,               
  ENSEMBLE_PATCHING,           
  BINARY_PATCHING,             
  UNPACKING,                   
  BUILDING,                    
  EXECUTING,                   
  ROLLINGBACK,                 
  REFRESHING_POLICY,           
  UPDATING_CHANNELS,           
  COPYING_PREFERENCES_FILE,    
  CREATING_SHORTCUTS,          
  REGISTERING_CHROME,          
  REMOVING_OLD_VERSIONS,       
  FINISHING,                   
  CONFIGURE_AUTO_LAUNCH,       
  CREATING_VISUAL_MANIFEST,    
  DEFERRING_TO_HIGHER_VERSION,  
  UNINSTALLING_BINARIES,       
  UNINSTALLING_CHROME_FRAME,   
  NUM_STAGES                   
};

COMPILE_ASSERT(UNINSTALLING_CHROME_FRAME == 20,
               never_ever_ever_change_InstallerStage_values_bang);

namespace switches {

extern const char kAutoLaunchChrome[];
extern const char kChrome[];
extern const char kChromeAppHostDeprecated[];  
extern const char kChromeAppLauncher[];
extern const char kChromeFrame[];
extern const char kChromeSxS[];
extern const char kConfigureUserSettings[];
extern const char kCriticalUpdateVersion[];
extern const char kDeleteProfile[];
extern const char kDisableLogging[];
extern const char kDoNotLaunchChrome[];
extern const char kDoNotRegisterForUpdateLaunch[];
extern const char kDoNotRemoveSharedItems[];
extern const char kEnableLogging[];
extern const char kEnsureGoogleUpdatePresent[];
extern const char kForceConfigureUserSettings[];
extern const char kForceUninstall[];
extern const char kInstallArchive[];
extern const char kInstallerData[];
extern const char kLogFile[];
extern const char kMakeChromeDefault[];
extern const char kMsi[];
extern const char kMultiInstall[];
extern const char kNewSetupExe[];
extern const char kOnOsUpgrade[];
extern const char kQueryEULAAcceptance[];
extern const char kRegisterChromeBrowser[];
extern const char kRegisterChromeBrowserSuffix[];
extern const char kRegisterDevChrome[];
extern const char kRegisterURLProtocol[];
extern const char kRenameChromeExe[];
extern const char kRemoveChromeRegistration[];
extern const char kRunAsAdmin[];
extern const char kSelfDestruct[];
extern const char kSystemLevel[];
extern const char kUninstall[];
extern const char kUpdateSetupExe[];
extern const char kUncompressedArchive[];
extern const char kVerboseLogging[];
extern const char kShowEula[];
extern const char kShowEulaForMetro[];
extern const char kInactiveUserToast[];
extern const char kSystemLevelToast[];
extern const char kExperimentGroup[];
extern const char kToastResultsKey[];
extern const char kPatch[];
extern const char kInputFile[];
extern const char kPatchFile[];
extern const char kOutputFile[];

}  

extern const wchar_t kActiveSetupExe[];
extern const wchar_t kChromeAppHostExe[];
extern const wchar_t kChromeDll[];
extern const wchar_t kChromeChildDll[];
extern const wchar_t kChromeExe[];
extern const wchar_t kChromeFrameDll[];
extern const wchar_t kChromeFrameHelperDll[];
extern const wchar_t kChromeFrameHelperExe[];
extern const wchar_t kChromeFrameHelperWndClass[];
extern const wchar_t kChromeLauncherExe[];
extern const wchar_t kChromeOldExe[];
extern const wchar_t kChromeNewExe[];
extern const wchar_t kCmdInstallApp[];
extern const wchar_t kCmdInstallExtension[];
extern const wchar_t kCmdOnOsUpgrade[];
extern const wchar_t kCmdQueryEULAAcceptance[];
extern const wchar_t kCmdQuickEnableApplicationHost[];
extern const wchar_t kCmdQuickEnableCf[];
extern const wchar_t kDelegateExecuteExe[];
extern const wchar_t kEULASentinelFile[];
extern const wchar_t kGoogleChromeInstallSubDir1[];
extern const wchar_t kGoogleChromeInstallSubDir2[];
extern const wchar_t kInstallBinaryDir[];
extern const wchar_t kInstallerDir[];
extern const wchar_t kInstallTempDir[];
extern const wchar_t kInstallUserDataDir[];
extern const wchar_t kLnkExt[];
extern const wchar_t kNaClExe[];
extern const wchar_t kSetupExe[];
extern const wchar_t kSxSSuffix[];
extern const wchar_t kUninstallArgumentsField[];
extern const wchar_t kUninstallDisplayNameField[];
extern const wchar_t kUninstallInstallationDate[];
extern const char kUninstallMetricsName[];
extern const wchar_t kUninstallStringField[];

extern const wchar_t kInstallerError[];
extern const wchar_t kInstallerExtraCode1[];
extern const wchar_t kInstallerResult[];
extern const wchar_t kInstallerResultUIString[];
extern const wchar_t kInstallerSuccessLaunchCmdLine[];

extern const wchar_t kOptionMultiInstall[];

extern const wchar_t kChromeChannelUnknown[];
extern const wchar_t kChromeChannelCanary[];
extern const wchar_t kChromeChannelDev[];
extern const wchar_t kChromeChannelBeta[];
extern const wchar_t kChromeChannelStable[];

extern const size_t kMaxAppModelIdLength;

const int kCourgetteErrorOffset = 300;
const int kBsdiffErrorOffset = 600;

extern const char kCourgette[];
extern const char kBsdiff[];

}  

#endif  
