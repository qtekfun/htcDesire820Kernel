// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_GCAPI_MAC_GCAPI_H_
#define CHROME_INSTALLER_GCAPI_MAC_GCAPI_H_

#define GCCC_ERROR_ALREADYPRESENT                (1 << 0)
#define GCCC_ERROR_ACCESSDENIED                  (1 << 1)
#define GCCC_ERROR_OSNOTSUPPORTED                (1 << 2)
#define GCCC_ERROR_ALREADYOFFERED                (1 << 3)
#define GCCC_ERROR_INTEGRITYLEVEL                (1 << 4)

#ifdef __cplusplus
extern "C" {
#endif

int GoogleChromeCompatibilityCheck(unsigned* reasons);

int InstallGoogleChrome(const char* source_path,
                        const char* brand_code,
                        const char* master_prefs_contents,
                        unsigned master_prefs_contents_size);

int LaunchGoogleChrome();

#ifdef __cplusplus
}  
#endif

#endif  
