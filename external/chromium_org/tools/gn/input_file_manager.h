// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_INPUT_FILE_MANAGER_H_
#define TOOLS_GN_INPUT_FILE_MANAGER_H_

#include <set>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "tools/gn/build_settings.h"
#include "tools/gn/input_file.h"
#include "tools/gn/parse_tree.h"
#include "tools/gn/settings.h"

class Err;
class LocationRange;
class ParseNode;
class Token;

class InputFileManager : public base::RefCountedThreadSafe<InputFileManager> {
 public:
  
  
  typedef base::Callback<void(const ParseNode*)> FileLoadCallback;

  InputFileManager();

  
  
  
  
  
  
  
  
  bool AsyncLoadFile(const LocationRange& origin,
                     const BuildSettings* build_settings,
                     const SourceFile& file_name,
                     const FileLoadCallback& callback,
                     Err* err);

  
  
  
  const ParseNode* SyncLoadFile(const LocationRange& origin,
                                const BuildSettings* build_settings,
                                const SourceFile& file_name,
                                Err* err);

  int GetInputFileCount() const;

  
  void GetAllPhysicalInputFileNames(std::vector<base::FilePath>* result) const;

 private:
  friend class base::RefCountedThreadSafe<InputFileManager>;

  struct InputFileData {
    InputFileData(const SourceFile& file_name);
    ~InputFileData();

    
    InputFile file;

    bool loaded;

    bool sync_invocation;

    
    
    std::vector<FileLoadCallback> scheduled_callbacks;

    
    
    
    scoped_ptr<base::WaitableEvent> completion_event;

    std::vector<Token> tokens;

    
    scoped_ptr<ParseNode> parsed_root;
  };

  virtual ~InputFileManager();

  void BackgroundLoadFile(const LocationRange& origin,
                          const BuildSettings* build_settings,
                          const SourceFile& name,
                          InputFile* file);

  
  bool LoadFile(const LocationRange& origin,
                const BuildSettings* build_settings,
                const SourceFile& name,
                InputFile* file,
                Err* err);

  mutable base::Lock lock_;

  
  typedef base::hash_map<SourceFile, InputFileData*> InputFileMap;
  InputFileMap input_files_;

  DISALLOW_COPY_AND_ASSIGN(InputFileManager);
};

#endif  
