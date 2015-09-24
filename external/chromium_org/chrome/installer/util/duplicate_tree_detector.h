// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_DUPLICATE_TREE_DETECTOR_H_
#define CHROME_INSTALLER_UTIL_DUPLICATE_TREE_DETECTOR_H_

namespace base {
class FilePath;
}

namespace installer {

bool IsIdenticalFileHierarchy(const base::FilePath& src_path,
                              const base::FilePath& dest_path);

}  

#endif  
