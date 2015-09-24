// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_PATCHER_OPERATION_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_PATCHER_OPERATION_H_

#include <string>
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/component_updater/component_unpacker.h"

namespace base {

class FilePath;
class DictionaryValue;

}  

class ComponentInstaller;
class ComponentPatcher;

class DeltaUpdateOp {
 public:

  DeltaUpdateOp();
  virtual ~DeltaUpdateOp();

  
  
  
  ComponentUnpacker::Error Run(
      base::DictionaryValue* command_args,
      const base::FilePath& input_dir,
      const base::FilePath& unpack_dir,
      ComponentPatcher* patcher,
      ComponentInstaller* installer,
      int* error);

 protected:
  std::string output_sha256_;
  base::FilePath output_abs_path_;

 private:
  ComponentUnpacker::Error CheckHash();

  
  
  
  virtual ComponentUnpacker::Error DoParseArguments(
      base::DictionaryValue* command_args,
      const base::FilePath& input_dir,
      ComponentInstaller* installer) = 0;

  
  
  
  virtual ComponentUnpacker::Error DoRun(ComponentPatcher* patcher,
                                         int* error) = 0;

  DISALLOW_COPY_AND_ASSIGN(DeltaUpdateOp);
};

class DeltaUpdateOpCopy : public DeltaUpdateOp {
 public:
  DeltaUpdateOpCopy();

 private:
  
  virtual ComponentUnpacker::Error DoParseArguments(
      base::DictionaryValue* command_args,
      const base::FilePath& input_dir,
      ComponentInstaller* installer) OVERRIDE;

  virtual ComponentUnpacker::Error DoRun(ComponentPatcher* patcher,
                                         int* error) OVERRIDE;

  base::FilePath input_abs_path_;

  DISALLOW_COPY_AND_ASSIGN(DeltaUpdateOpCopy);
};

class DeltaUpdateOpCreate : public DeltaUpdateOp {
 public:
  DeltaUpdateOpCreate();

 private:
  
  virtual ComponentUnpacker::Error DoParseArguments(
      base::DictionaryValue* command_args,
      const base::FilePath& input_dir,
      ComponentInstaller* installer) OVERRIDE;

  virtual ComponentUnpacker::Error DoRun(ComponentPatcher* patcher,
                                         int* error) OVERRIDE;

  base::FilePath patch_abs_path_;

  DISALLOW_COPY_AND_ASSIGN(DeltaUpdateOpCreate);
};

class DeltaUpdateOpPatchBsdiff : public DeltaUpdateOp {
 public:
  DeltaUpdateOpPatchBsdiff();

 private:
  
  virtual ComponentUnpacker::Error DoParseArguments(
      base::DictionaryValue* command_args,
      const base::FilePath& input_dir,
      ComponentInstaller* installer) OVERRIDE;

  virtual ComponentUnpacker::Error DoRun(ComponentPatcher* patcher,
                                         int* error) OVERRIDE;

  base::FilePath patch_abs_path_;
  base::FilePath input_abs_path_;

  DISALLOW_COPY_AND_ASSIGN(DeltaUpdateOpPatchBsdiff);
};

class DeltaUpdateOpPatchCourgette : public DeltaUpdateOp {
 public:
  DeltaUpdateOpPatchCourgette();

 private:
  
  virtual ComponentUnpacker::Error DoParseArguments(
      base::DictionaryValue* command_args,
      const base::FilePath& input_dir,
      ComponentInstaller* installer) OVERRIDE;

  virtual ComponentUnpacker::Error DoRun(ComponentPatcher* patcher,
                                         int* error) OVERRIDE;

  base::FilePath patch_abs_path_;
  base::FilePath input_abs_path_;

  DISALLOW_COPY_AND_ASSIGN(DeltaUpdateOpPatchCourgette);
};

DeltaUpdateOp* CreateDeltaUpdateOp(base::DictionaryValue* command);

#endif  
