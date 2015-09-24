// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_ZLIB_GOOGLE_ZIP_H_
#define THIRD_PARTY_ZLIB_GOOGLE_ZIP_H_

#include "base/callback.h"
#include "base/files/file_path.h"

namespace zip {

typedef base::Callback<bool(const base::FilePath&)> FilterCallback;
bool ZipWithFilterCallback(const base::FilePath& src_dir,
                           const base::FilePath& dest_file,
                           const FilterCallback& filter_cb);

bool Zip(const base::FilePath& src_dir, const base::FilePath& dest_file,
         bool include_hidden_files);

#if defined(OS_POSIX)
bool ZipFiles(const base::FilePath& src_dir,
              const std::vector<base::FilePath>& src_relative_paths,
              int dest_fd);
#endif  

bool Unzip(const base::FilePath& zip_file, const base::FilePath& dest_dir);

}  

#endif  
