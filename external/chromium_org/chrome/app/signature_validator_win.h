// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_APP_SIGNATURE_VALIDATOR_WIN_H_
#define CHROME_APP_SIGNATURE_VALIDATOR_WIN_H_

#include <string>
#include <vector>

namespace base {
class FilePath;
}

bool VerifyAuthenticodeSignature(const base::FilePath& signed_file);

bool VerifySignerIsGoogle(const base::FilePath& signed_file,
                          const std::string& subject_name,
                          const std::vector<std::string>& expected_hashes);

#endif  
