// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_CHROME_CONSTANTS_H_
#define CHROME_COMMON_CHROME_CONSTANTS_H_

#include "base/files/file_path.h"

namespace chrome {

extern const char kChromeVersion[];

extern const char kChromeVersionEnvVar[];

extern const base::FilePath::CharType kBrowserProcessExecutableName[];
extern const base::FilePath::CharType kHelperProcessExecutableName[];
extern const base::FilePath::CharType kBrowserProcessExecutablePath[];
extern const base::FilePath::CharType kHelperProcessExecutablePath[];
extern const base::FilePath::CharType kBrowserProcessExecutableNameChromium[];
extern const base::FilePath::CharType kHelperProcessExecutableNameChromium[];
extern const base::FilePath::CharType kBrowserProcessExecutablePathChromium[];
extern const base::FilePath::CharType kHelperProcessExecutablePathChromium[];
#if defined(OS_MACOSX)
extern const base::FilePath::CharType kFrameworkName[];

extern const base::FilePath::CharType* const kHelperFlavorSuffixes[];
#endif  
#if defined(OS_WIN)
extern const base::FilePath::CharType kMetroDriverDll[];
extern const wchar_t kStatusTrayWindowClass[];
#endif  
extern const wchar_t kCrashReportLog[];
extern const wchar_t kTestingInterfaceDLL[];
extern const char    kInitialProfile[];
extern const char    kMultiProfileDirPrefix[];
extern const base::FilePath::CharType kGuestProfileDir[];
extern const wchar_t kBrowserResourcesDll[];

#if defined(OS_ANDROID)
extern const base::FilePath::CharType kAndroidCacheFilename[];
#endif
extern const base::FilePath::CharType kArchivedHistoryFilename[];
extern const base::FilePath::CharType kBookmarksFileName[];
extern const base::FilePath::CharType kCacheDirname[];
extern const base::FilePath::CharType kCookieFilename[];
extern const base::FilePath::CharType kCRLSetFilename[];
extern const base::FilePath::CharType kCustomDictionaryFileName[];
extern const base::FilePath::CharType kExtensionActivityLogFilename[];
extern const base::FilePath::CharType kExtensionsCookieFilename[];
extern const base::FilePath::CharType kFaviconsFilename[];
extern const base::FilePath::CharType kFirstRunSentinel[];
extern const base::FilePath::CharType kHistoryFilename[];
extern const base::FilePath::CharType kJumpListIconDirname[];
extern const base::FilePath::CharType kLocalStateFilename[];
extern const base::FilePath::CharType kLocalStorePoolName[];
extern const base::FilePath::CharType kLoginDataFileName[];
extern const base::FilePath::CharType kManagedUserSettingsFilename[];
extern const base::FilePath::CharType kMediaCacheDirname[];
extern const base::FilePath::CharType kNewTabThumbnailsFilename[];
extern const base::FilePath::CharType kOBCertFilename[];
extern const base::FilePath::CharType kPreferencesFilename[];
extern const base::FilePath::CharType kReadmeFilename[];
extern const base::FilePath::CharType kResetPromptMementoFilename[];
extern const base::FilePath::CharType kSafeBrowsingBaseFilename[];
extern const base::FilePath::CharType kServiceStateFileName[];
extern const base::FilePath::CharType kShortcutsDatabaseName[];
extern const base::FilePath::CharType kSingletonCookieFilename[];
extern const base::FilePath::CharType kSingletonLockFilename[];
extern const base::FilePath::CharType kSingletonSocketFilename[];
extern const base::FilePath::CharType kSyncCredentialsFilename[];
extern const base::FilePath::CharType kThemePackFilename[];
extern const base::FilePath::CharType kThumbnailsFilename[];
extern const base::FilePath::CharType kTopSitesFilename[];
extern const base::FilePath::CharType kWebAppDirname[];

extern const base::FilePath::CharType kPepperFlashPluginFilename[];

extern const wchar_t kUserDataDirname[];

extern const bool kRecordModeEnabled;

extern const int kJavaScriptMessageExpectedDelay;

extern const bool kEnableTouchIcon;

extern const float kMaxShareOfExtensionProcesses;

extern const char kPreReadEnvironmentVariable[];

#if defined(OS_LINUX)
extern const int kLowestRendererOomScore;
extern const int kHighestRendererOomScore;
#endif

#if defined(OS_WIN)
extern const wchar_t kMetroChromeUserDataSubDir[];
extern const wchar_t kMetroNavigationAndSearchMessage[];
extern const wchar_t kMetroGetCurrentTabInfoMessage[];
extern const wchar_t kMetroRegistryPath[];
extern const wchar_t kLaunchModeValue[];
extern const wchar_t kBrowserCrashDumpAttemptsRegistryPath[];
extern const char kSafeModeEnvVar[];
#endif

#if defined(OS_CHROMEOS)
extern const char kProfileDirPrefix[];

extern const char kLegacyProfileDir[];

extern const char kTestUserProfileDir[];
#endif

extern const char kApplicationClientIDStringForAVScanning[];

}  

#endif  
