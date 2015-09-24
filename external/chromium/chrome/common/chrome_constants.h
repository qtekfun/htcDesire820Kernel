// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_CHROME_CONSTANTS_H_
#define CHROME_COMMON_CHROME_CONSTANTS_H_
#pragma once

#include "base/file_path.h"

namespace chrome {

extern const char kChromeVersion[];

extern const char kChromeVersionEnvVar[];

extern const FilePath::CharType kBrowserProcessExecutableName[];
extern const FilePath::CharType kHelperProcessExecutableName[];
extern const FilePath::CharType kBrowserProcessExecutablePath[];
extern const FilePath::CharType kHelperProcessExecutablePath[];
#if defined(OS_MACOSX)
extern const FilePath::CharType kFrameworkName[];
#endif
extern const wchar_t kBrowserAppName[];
#if defined(OS_WIN)
extern const wchar_t kStatusTrayWindowClass[];
#endif  
extern const wchar_t kMessageWindowClass[];
extern const wchar_t kCrashReportLog[];
extern const wchar_t kTestingInterfaceDLL[];
extern const char    kNotSignedInProfile[];
extern const char    kStatsFilename[];
extern const wchar_t kBrowserResourcesDll[];
extern const wchar_t kNaClAppName[];
extern const FilePath::CharType kExtensionFileExtension[];
extern const FilePath::CharType kExtensionKeyFileExtension[];

extern const FilePath::CharType kArchivedHistoryFilename[];
extern const FilePath::CharType kCacheDirname[];
extern const FilePath::CharType kMediaCacheDirname[];
extern const FilePath::CharType kOffTheRecordMediaCacheDirname[];
extern const FilePath::CharType kAppCacheDirname[];
extern const FilePath::CharType kThemePackFilename[];
extern const FilePath::CharType kCookieFilename[];
extern const FilePath::CharType kExtensionsCookieFilename[];
extern const FilePath::CharType kIsolatedAppStateDirname[];
extern const FilePath::CharType kFaviconsFilename[];
extern const FilePath::CharType kHistoryFilename[];
extern const FilePath::CharType kLocalStateFilename[];
extern const FilePath::CharType kPreferencesFilename[];
extern const FilePath::CharType kSafeBrowsingBaseFilename[];
extern const FilePath::CharType kSafeBrowsingPhishingModelFilename[];
extern const FilePath::CharType kSingletonCookieFilename[];
extern const FilePath::CharType kSingletonSocketFilename[];
extern const FilePath::CharType kSingletonLockFilename[];
extern const FilePath::CharType kThumbnailsFilename[];
extern const FilePath::CharType kNewTabThumbnailsFilename[];
extern const FilePath::CharType kTopSitesFilename[];
extern const wchar_t kUserDataDirname[];
extern const FilePath::CharType kUserScriptsDirname[];
extern const FilePath::CharType kWebDataFilename[];
extern const FilePath::CharType kBookmarksFileName[];
extern const FilePath::CharType kHistoryBookmarksFileName[];
extern const FilePath::CharType kCustomDictionaryFileName[];
extern const FilePath::CharType kLoginDataFileName[];
extern const FilePath::CharType kJumpListIconDirname[];
extern const FilePath::CharType kWebAppDirname[];
extern const FilePath::CharType kServiceStateFileName[];

extern const unsigned int kMaxRendererProcessCount;
extern const int kStatsMaxThreads;
extern const int kStatsMaxCounters;

extern const bool kRecordModeEnabled;

extern const int kHistogramSynchronizerReservedSequenceNumber;

extern const char* const kUnknownLanguageCode;

extern const int kJavascriptMessageExpectedDelay;

}  

#endif  
