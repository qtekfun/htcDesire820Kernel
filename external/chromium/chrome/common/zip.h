// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_ZIP_H_
#define CHROME_COMMON_ZIP_H_
#pragma once

class FilePath;

bool Zip(const FilePath& src_dir, const FilePath& dest_file,
         bool include_hidden_files);

bool Unzip(const FilePath& zip_file, const FilePath& dest_dir);

#endif  
