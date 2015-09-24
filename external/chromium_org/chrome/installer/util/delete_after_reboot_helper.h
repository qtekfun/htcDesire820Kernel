// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_DELETE_AFTER_REBOOT_HELPER_H_
#define CHROME_INSTALLER_UTIL_DELETE_AFTER_REBOOT_HELPER_H_

#include <string>
#include <vector>

#include <windows.h>

namespace base {
class FilePath;
}

extern const wchar_t kSessionManagerKey[];
extern const wchar_t kPendingFileRenameOps[];

typedef std::pair<std::wstring, std::wstring> PendingMove;

bool ScheduleFileSystemEntityForDeletion(const base::FilePath& path);

bool ScheduleDirectoryForDeletion(const base::FilePath& dir_name);

bool RemoveFromMovesPendingReboot(const base::FilePath& directory);

HRESULT GetPendingMovesValue(std::vector<PendingMove>* pending_moves);

bool MatchPendingDeletePath(const base::FilePath& short_form_needle,
                            const base::FilePath& reg_path);

HRESULT MultiSZBytesToStringArray(const char* buffer, size_t byte_count,
                                  std::vector<PendingMove>* value);

void StringArrayToMultiSZBytes(const std::vector<PendingMove>& strings,
                               std::vector<char>* buffer);

base::FilePath GetShortPathName(const base::FilePath& path);

#endif  
