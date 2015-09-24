// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_SETUP_INSTALL_WORKER_H_
#define CHROME_INSTALLER_SETUP_INSTALL_WORKER_H_

#include <windows.h>

#include <vector>

class BrowserDistribution;
class CommandLine;
class WorkItemList;

namespace base {
class FilePath;
class Version;
}

namespace installer {

class InstallationState;
class InstallerState;
class Product;

void AddUninstallShortcutWorkItems(const InstallerState& installer_state,
                                   const base::FilePath& setup_path,
                                   const base::Version& new_version,
                                   const Product& product,
                                   WorkItemList* install_list);

void AddVersionKeyWorkItems(HKEY root,
                            BrowserDistribution* dist,
                            const base::Version& new_version,
                            bool add_language_identifier,
                            WorkItemList* list);

void AddOemInstallWorkItems(const InstallationState& original_state,
                            const InstallerState& installer_state,
                            WorkItemList* install_list);

void AddEulaAcceptedWorkItems(const InstallationState& original_state,
                              const InstallerState& installer_state,
                              WorkItemList* install_list);

void AddGoogleUpdateWorkItems(const InstallationState& original_state,
                              const InstallerState& installer_state,
                              WorkItemList* install_list);

void AddUsageStatsWorkItems(const InstallationState& original_state,
                            const InstallerState& installer_state,
                            WorkItemList* install_list);

bool AppendPostInstallTasks(const InstallerState& installer_state,
                            const base::FilePath& setup_path,
                            const base::Version* current_version,
                            const base::Version& new_version,
                            const base::FilePath& temp_path,
                            WorkItemList* post_install_task_list);

void AddInstallWorkItems(const InstallationState& original_state,
                         const InstallerState& installer_state,
                         const base::FilePath& setup_path,
                         const base::FilePath& archive_path,
                         const base::FilePath& src_path,
                         const base::FilePath& temp_path,
                         const base::Version* current_version,
                         const base::Version& new_version,
                         WorkItemList* install_list);

void AddRegisterComDllWorkItems(const base::FilePath& dll_folder,
                                const std::vector<base::FilePath>& dll_files,
                                bool system_level,
                                bool do_register,
                                bool ignore_failures,
                                WorkItemList* work_item_list);

void AddSetMsiMarkerWorkItem(const InstallerState& installer_state,
                             BrowserDistribution* dist,
                             bool set,
                             WorkItemList* work_item_list);

void AddDelegateExecuteWorkItems(const InstallerState& installer_state,
                                 const base::FilePath& target_path,
                                 const base::Version& new_version,
                                 const Product& product,
                                 WorkItemList* list);

void AddActiveSetupWorkItems(const InstallerState& installer_state,
                             const base::FilePath& setup_path,
                             const base::Version& new_version,
                             const Product& product,
                             WorkItemList* list);

void AddDeleteOldIELowRightsPolicyWorkItems(
    const InstallerState& installer_state,
    WorkItemList* install_list);

void AppendUninstallCommandLineFlags(const InstallerState& installer_state,
                                     const Product& product,
                                     CommandLine* uninstall_cmd);

void RefreshElevationPolicy();

void AddOsUpgradeWorkItems(const InstallerState& installer_state,
                           const base::FilePath& setup_path,
                           const base::Version& new_version,
                           const Product& product,
                           WorkItemList* install_list);

void AddQueryEULAAcceptanceWorkItems(const InstallerState& installer_state,
                                     const base::FilePath& setup_path,
                                     const base::Version& new_version,
                                     const Product& product,
                                     WorkItemList* work_item_list);

void AddQuickEnableChromeFrameWorkItems(const InstallerState& installer_state,
                                        WorkItemList* work_item_list);

}  

#endif  
