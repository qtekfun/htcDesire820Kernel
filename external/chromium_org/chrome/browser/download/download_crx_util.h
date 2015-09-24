// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_CRX_UTIL_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_CRX_UTIL_H_


#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

class ExtensionInstallPrompt;
class Profile;

namespace content {
class DownloadItem;
}

namespace extensions {
class CrxInstaller;
}

namespace download_crx_util {

void SetMockInstallPromptForTesting(
    scoped_ptr<ExtensionInstallPrompt> mock_prompt);

scoped_refptr<extensions::CrxInstaller> OpenChromeExtension(
    Profile* profile,
    const content::DownloadItem& download_item);

bool IsExtensionDownload(const content::DownloadItem& download_item);

bool OffStoreInstallAllowedByPrefs(Profile* profile,
                                   const content::DownloadItem& item);

}  

#endif  
