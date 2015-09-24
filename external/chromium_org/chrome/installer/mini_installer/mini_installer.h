// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_MINI_INSTALLER_MINI_INSTALLER_H_
#define CHROME_INSTALLER_MINI_INSTALLER_MINI_INSTALLER_H_

namespace mini_installer {

const wchar_t kSetupName[] = L"setup.exe";
const wchar_t kChromePrefix[] = L"chrome";
const wchar_t kSetupPrefix[] = L"setup";

const wchar_t kCmdInstallArchive[] = L" --install-archive";
const wchar_t kCmdUpdateSetupExe[] = L" --update-setup-exe";
const wchar_t kCmdNewSetupExe[] = L" --new-setup-exe";

const wchar_t kTempPrefix[] = L"CR_";
const wchar_t kFullInstallerSuffix[] = L"-full";
const wchar_t kMultiInstallTag[] = L"-multi";

const wchar_t kBinResourceType[] = L"BN";
const wchar_t kLZCResourceType[] = L"BL";
const wchar_t kLZMAResourceType[] = L"B7";

const wchar_t kApRegistryValueName[] = L"ap";
const wchar_t kCleanupRegistryValueName[] = L"ChromeInstallerCleanup";
const wchar_t kUninstallRegistryValueName[] = L"UninstallString";

#if defined(GOOGLE_CHROME_BUILD)
const wchar_t kApRegistryKeyBase[] = L"Software\\Google\\Update\\ClientState\\";
const wchar_t kUninstallRegistryKey[] =
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Google Chrome";
const wchar_t kCleanupRegistryKey[] = L"Software\\Google";
#else
const wchar_t kApRegistryKeyBase[] = L"Software\\Chromium";
const wchar_t kUninstallRegistryKey[] =
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Chromium";
const wchar_t kCleanupRegistryKey[] = L"Software\\Chromium";
#endif

const int kMaxResourceSize = 1024*1024*1024;

const wchar_t kManifestFilename[] = L"packed_files.txt";

}  

#endif  

