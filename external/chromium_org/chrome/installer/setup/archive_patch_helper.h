// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_SETUP_ARCHIVE_PATCH_HELPER_H_
#define CHROME_INSTALLER_SETUP_ARCHIVE_PATCH_HELPER_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"

namespace installer {

class ArchivePatchHelper {
 public:
  
  
  
  ArchivePatchHelper(const base::FilePath& working_directory,
                     const base::FilePath& compressed_archive,
                     const base::FilePath& patch_source,
                     const base::FilePath& target);

  ~ArchivePatchHelper();

  
  
  
  
  static bool UncompressAndPatch(const base::FilePath& working_directory,
                                 const base::FilePath& compressed_archive,
                                 const base::FilePath& patch_source,
                                 const base::FilePath& target);

  
  
  
  bool Uncompress(base::FilePath* last_uncompressed_file);

  
  
  bool EnsemblePatch();

  
  
  bool BinaryPatch();

  const base::FilePath& compressed_archive() const {
    return compressed_archive_;
  }
  void set_patch_source(const base::FilePath& patch_source) {
    patch_source_ = patch_source;
  }
  const base::FilePath& patch_source() const {
    return patch_source_;
  }
  const base::FilePath& target() const {
    return target_;
  }

  
  const base::FilePath& last_uncompressed_file() const {
    return last_uncompressed_file_;
  }
  void set_last_uncompressed_file(
      const base::FilePath& last_uncompressed_file) {
    last_uncompressed_file_ = last_uncompressed_file;
  }

 private:
  base::FilePath working_directory_;
  base::FilePath compressed_archive_;
  base::FilePath patch_source_;
  base::FilePath target_;
  base::FilePath last_uncompressed_file_;

  DISALLOW_COPY_AND_ASSIGN(ArchivePatchHelper);
};

}  

#endif  
