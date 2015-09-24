// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_PATCHER_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_PATCHER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/component_updater/component_unpacker.h"

namespace base {
class FilePath;
}

class ComponentInstaller;

class ComponentPatcher {
 public:
  
  enum PatchType {
    kPatchTypeUnknown,
    kPatchTypeCourgette,
    kPatchTypeBsdiff,
  };

  virtual ComponentUnpacker::Error Patch(PatchType patch_type,
                                         const base::FilePath& input_file,
                                         const base::FilePath& patch_file,
                                         const base::FilePath& output_file,
                                         int* error) = 0;
  virtual ~ComponentPatcher() {}
};

class ComponentPatcherCrossPlatform : public ComponentPatcher {
 public:
  ComponentPatcherCrossPlatform();
  virtual ComponentUnpacker::Error Patch(PatchType patch_type,
                                         const base::FilePath& input_file,
                                         const base::FilePath& patch_file,
                                         const base::FilePath& output_file,
                                         int* error) OVERRIDE;
 private:
  DISALLOW_COPY_AND_ASSIGN(ComponentPatcherCrossPlatform);
};

// The non-differential files are written into the |unpack_dir| directory.
ComponentUnpacker::Error DifferentialUpdatePatch(
    const base::FilePath& input_dir,
    const base::FilePath& unpack_dir,
    ComponentPatcher* component_patcher,
    ComponentInstaller* installer,
    int* error);

#endif  
