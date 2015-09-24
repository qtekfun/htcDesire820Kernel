// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_CONSTANTS_H_
#define EXTENSIONS_COMMON_CONSTANTS_H_

#include "base/files/file_path.h"

namespace extensions {

extern const char kExtensionScheme[];

extern const base::FilePath::CharType kManifestFilename[];

extern const base::FilePath::CharType kLocaleFolder[];

extern const base::FilePath::CharType kMessagesFilename[];

extern const base::FilePath::CharType kPlatformSpecificFolder[];

extern const char kInstallDirectoryName[];

extern const char kTempExtensionName[];

extern const char kDecodedImagesFilename[];

extern const char kDecodedMessageCatalogsFilename[];

extern const char kGeneratedBackgroundPageFilename[];

extern const char kModulesDir[];

extern const base::FilePath::CharType kExtensionFileExtension[];

extern const base::FilePath::CharType kExtensionKeyFileExtension[];

extern const int kDefaultUpdateFrequencySeconds;

extern const char kLocalAppSettingsDirectoryName[];

extern const char kLocalExtensionSettingsDirectoryName[];

extern const char kSyncAppSettingsDirectoryName[];

extern const char kSyncExtensionSettingsDirectoryName[];

extern const char kManagedSettingsDirectoryName[];

extern const char kStateStoreName[];

extern const char kRulesStoreName[];

}  

#endif  
