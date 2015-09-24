// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_LOADER_H_
#define TOOLS_GN_LOADER_H_

#include <map>
#include <set>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "tools/gn/label.h"
#include "tools/gn/scope.h"

namespace base {
class MessageLoop;
}

class BuildSettings;
class Settings;
class SourceFile;
class Toolchain;

class Loader : public base::RefCountedThreadSafe<Loader> {
 public:
  Loader();

  
  
  
  
  virtual void Load(const SourceFile& file,
                    const Label& toolchain_name) = 0;

  
  
  virtual void ToolchainLoaded(const Toolchain* toolchain) = 0;

  
  virtual Label GetDefaultToolchain() const = 0;

  
  
  virtual const Settings* GetToolchainSettings(const Label& label) = 0;

  
  
  void Load(const Label& label);

  
  static SourceFile BuildFileForLabel(const Label& label);

  
  
  
  
  static const void* kDefaultToolchainKey;

 protected:
  friend class base::RefCountedThreadSafe<Loader>;
  virtual ~Loader();
};

class LoaderImpl : public Loader {
 public:
  
  typedef base::Callback<bool(const LocationRange&,
                              const BuildSettings*,
                              const SourceFile&,
                              const base::Callback<void(const ParseNode*)>&,
                              Err*)> AsyncLoadFileCallback;

  LoaderImpl(const BuildSettings* build_settings);

  
  virtual void Load(const SourceFile& file,
                    const Label& toolchain_name) OVERRIDE;
  virtual void ToolchainLoaded(const Toolchain* toolchain) OVERRIDE;
  virtual Label GetDefaultToolchain() const OVERRIDE;
  virtual const Settings* GetToolchainSettings(const Label& label) OVERRIDE;

  
  
  
  void set_main_loop(base::MessageLoop* loop) { main_loop_ = loop; }

  
  
  
  void set_complete_callback(const base::Closure& cb) {
    complete_callback_ = cb;
  }

  
  void set_async_load_file(const AsyncLoadFileCallback& cb) {
    async_load_file_ = cb;
  }

  const Label& default_toolchain_label() const {
    return default_toolchain_label_;
  }

 private:
  struct LoadID;
  struct ToolchainRecord;

  virtual ~LoaderImpl();

  
  void ScheduleLoadFile(const Settings* settings,
                        const SourceFile& file);
  void ScheduleLoadBuildConfig(
      Settings* settings,
      const Scope::KeyValueMap& toolchain_overrides);

  
  
  void BackgroundLoadFile(const Settings* settings,
                          const SourceFile& file_name,
                          const ParseNode* root);
  void BackgroundLoadBuildConfig(
      Settings* settings,
      const Scope::KeyValueMap& toolchain_overrides,
      const ParseNode* root);

  
  
  void DidLoadFile();

  
  
  
  
  
  
  void DidLoadBuildConfig(const Label& label);

  
  
  void DecrementPendingLoads();

  
  bool AsyncLoadFile(const LocationRange& origin,
                     const BuildSettings* build_settings,
                     const SourceFile& file_name,
                     const base::Callback<void(const ParseNode*)>& callback,
                     Err* err);

  base::MessageLoop* main_loop_;

  int pending_loads_;
  base::Closure complete_callback_;

  
  
  AsyncLoadFileCallback async_load_file_;

  typedef std::set<LoadID> LoadIDSet;
  LoadIDSet invocations_;

  const BuildSettings* build_settings_;
  Label default_toolchain_label_;

  
  
  typedef std::map<Label, ToolchainRecord*> ToolchainRecordMap;
  ToolchainRecordMap toolchain_records_;
};

#endif  
