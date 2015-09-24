// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_BUILDER_H_
#define TOOLS_GN_BUILDER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "tools/gn/builder_record.h"
#include "tools/gn/label.h"
#include "tools/gn/label_ptr.h"

class Config;
class Err;
class Loader;
class ParseNode;

class Builder : public base::RefCountedThreadSafe<Builder> {
 public:
  typedef base::Callback<void(const Item*)> ResolvedCallback;

  Builder(Loader* loader);

  
  
  void set_resolved_callback(const ResolvedCallback& cb) {
    resolved_callback_ = cb;
  }

  Loader* loader() const { return loader_; }

  void ItemDefined(scoped_ptr<Item> item);

  
  const Item* GetItem(const Label& label) const;
  const Toolchain* GetToolchain(const Label& label) const;

  std::vector<const BuilderRecord*> GetAllRecords() const;

  
  std::vector<const Target*> GetAllResolvedTargets() const;

  
  
  const BuilderRecord* GetRecord(const Label& label) const;
  BuilderRecord* GetRecord(const Label& label);

  
  bool CheckForBadItems(Err* err) const;

 private:
  friend class base::RefCountedThreadSafe<Builder>;

  virtual ~Builder();

  bool TargetDefined(BuilderRecord* record, Err* err);

  
  
  
  
  
  
  BuilderRecord* GetOrCreateRecordOfType(const Label& label,
                                         const ParseNode* request_from,
                                         BuilderRecord::ItemType type,
                                         Err* err);

  
  
  
  
  
  BuilderRecord* GetResolvedRecordOfType(const Label& label,
                                         const ParseNode* request_from,
                                         BuilderRecord::ItemType type,
                                         Err* err);

  bool AddDeps(BuilderRecord* record,
               const LabelConfigVector& configs,
               Err* err);
  bool AddDeps(BuilderRecord* record,
               const LabelTargetVector& targets,
               Err* err);
  bool AddToolchainDep(BuilderRecord* record,
                       const Target* target,
                       Err* err);

  
  
  
  
  
  
  
  
  
  
  void RecursiveSetShouldGenerate(BuilderRecord* record, bool force);

  void ScheduleItemLoadIfNecessary(BuilderRecord* record);

  
  
  bool ResolveItem(BuilderRecord* record, Err* err);

  
  
  
  bool ResolveDeps(LabelTargetVector* deps, Err* err);
  bool ResolveConfigs(LabelConfigVector* configs, Err* err);
  bool ResolveForwardDependentConfigs(Target* target, Err* err);

  
  
  
  std::string CheckForCircularDependencies(
      const std::vector<const BuilderRecord*>& bad_records) const;

  
  Loader* loader_;

  
  typedef base::hash_map<Label, BuilderRecord*> RecordMap;
  RecordMap records_;

  ResolvedCallback resolved_callback_;

  DISALLOW_COPY_AND_ASSIGN(Builder);
};

#endif  
