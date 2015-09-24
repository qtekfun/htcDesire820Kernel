// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_TEST_ALTERNATE_VERSION_GENERATOR_H_
#define CHROME_INSTALLER_TEST_ALTERNATE_VERSION_GENERATOR_H_

#include <string>

namespace base {
class FilePath;
class Version;
}

namespace upgrade_test {

enum Direction {
  PREVIOUS_VERSION,
  NEXT_VERSION
};

bool GenerateAlternateVersion(const base::FilePath& original_installer_path,
                              const base::FilePath& target_path,
                              Direction direction,
                              std::wstring* original_version,
                              std::wstring* new_version);

bool GenerateAlternatePEFileVersion(const base::FilePath& original_file,
                                    const base::FilePath& target_file,
                                    Direction direction);

bool GenerateSpecificPEFileVersion(const base::FilePath& original_file,
                                   const base::FilePath& target_file,
                                   const base::Version& version);

}  

#endif  
