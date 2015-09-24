// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_GCAPI_GCAPI_H_
#define CHROME_INSTALLER_GCAPI_GCAPI_H_

#include <windows.h>

#define GCCC_ERROR_USERLEVELALREADYPRESENT       (1 << 0)
#define GCCC_ERROR_SYSTEMLEVELALREADYPRESENT     (1 << 1)
#define GCCC_ERROR_ACCESSDENIED                  (1 << 2)
#define GCCC_ERROR_OSNOTSUPPORTED                (1 << 3)
#define GCCC_ERROR_ALREADYOFFERED                (1 << 4)
#define GCCC_ERROR_INTEGRITYLEVEL                (1 << 5)

#define REACTIVATE_ERROR_NOTINSTALLED            (1 << 0)
#define REACTIVATE_ERROR_NOTDORMANT              (1 << 1)
#define REACTIVATE_ERROR_ALREADY_REACTIVATED     (1 << 2)
#define REACTIVATE_ERROR_INVALID_INPUT           (1 << 3)
#define REACTIVATE_ERROR_REACTIVATION_FAILED     (1 << 4)

#define RELAUNCH_ERROR_NOTINSTALLED              (1 << 0)
#define RELAUNCH_ERROR_INVALID_PARTNER           (1 << 1)
#define RELAUNCH_ERROR_PINGS_SENT                (1 << 2)
#define RELAUNCH_ERROR_NOTDORMANT                (1 << 3)
#define RELAUNCH_ERROR_ALREADY_RELAUNCHED        (1 << 4)
#define RELAUNCH_ERROR_INVALID_INPUT             (1 << 5)
#define RELAUNCH_ERROR_RELAUNCH_FAILED           (1 << 6)

#define GCAPI_INVOKED_STANDARD_SHELL             (1 << 0)
#define GCAPI_INVOKED_UAC_ELEVATION              (1 << 1)

#ifdef __cplusplus
extern "C" {
#endif

const int kReactivationMinDaysDormant = 50;

const int kRelaunchMinDaysDormant = 30;

BOOL __stdcall GoogleChromeCompatibilityCheck(BOOL set_flag,
                                              int shell_mode,
                                              DWORD* reasons);

BOOL __stdcall LaunchGoogleChrome();

BOOL __stdcall LaunchGoogleChromeInBackground();

BOOL __stdcall LaunchGoogleChromeWithDimensions(int x,
                                                int y,
                                                int width,
                                                int height,
                                                bool in_background);

int __stdcall GoogleChromeDaysSinceLastRun();

BOOL __stdcall CanOfferReactivation(const wchar_t* brand_code,
                                    int shell_mode,
                                    DWORD* error_code);

BOOL __stdcall ReactivateChrome(wchar_t* brand_code,
                                int shell_mode,
                                DWORD* error_code);

BOOL __stdcall CanOfferRelaunch(const wchar_t** partner_brandcode_list,
                                int partner_brandcode_list_length,
                                int shell_mode,
                                DWORD* error_code);

BOOL __stdcall SetRelaunchOffered(const wchar_t** partner_brandcode_list,
                                  int partner_brandcode_list_length,
                                  const wchar_t* relaunch_brandcode,
                                  int shell_mode,
                                  DWORD* error_code);

typedef BOOL (__stdcall *GCCC_CompatibilityCheck)(BOOL, int, DWORD *);
typedef BOOL (__stdcall *GCCC_LaunchGC)();
typedef BOOL (__stdcall *GCCC_LaunchGoogleChromeInBackground)();
typedef BOOL (__stdcall *GCCC_LaunchGCWithDimensions)(int, int, int, int, bool);
typedef int (__stdcall *GCCC_GoogleChromeDaysSinceLastRun)();
typedef BOOL (__stdcall *GCCC_CanOfferReactivation)(const wchar_t*,
                                                    int,
                                                    DWORD*);
typedef BOOL (__stdcall *GCCC_ReactivateChrome)(const wchar_t*,
                                                int,
                                                DWORD*);
typedef BOOL (__stdcall *GCCC_CanOfferRelaunch)(const wchar_t**,
                                                int,
                                                int,
                                                DWORD*);
typedef BOOL (__stdcall *GCCC_SetRelaunchOffered)(const wchar_t**,
                                                  int,
                                                  const wchar_t*,
                                                  int,
                                                  DWORD*);

#ifdef __cplusplus
}  
#endif

#endif  
