// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_SCOPE_H_
#define TOOLS_GN_SCOPE_H_

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "tools/gn/err.h"
#include "tools/gn/pattern.h"
#include "tools/gn/source_dir.h"
#include "tools/gn/value.h"

class FunctionCallNode;
class ImportManager;
class ParseNode;
class Settings;
class TargetManager;

class Scope {
 public:
  typedef base::hash_map<base::StringPiece, Value> KeyValueMap;

  
  
  
  class ProgrammaticProvider {
   public:
    ProgrammaticProvider(Scope* scope) : scope_(scope) {
      scope_->AddProvider(this);
    }
    ~ProgrammaticProvider() {
      scope_->RemoveProvider(this);
    }

    
    
    virtual const Value* GetProgrammaticValue(
        const base::StringPiece& ident) = 0;

   protected:
    Scope* scope_;
  };

  
  Scope(const Settings* settings);

  
  Scope(Scope* parent);
  Scope(const Scope* parent);

  ~Scope();

  const Settings* settings() const { return settings_; }

  
  
  Scope* mutable_containing() { return mutable_containing_; }
  const Scope* mutable_containing() const { return mutable_containing_; }
  const Scope* const_containing() const { return const_containing_; }
  const Scope* containing() const {
    return mutable_containing_ ? mutable_containing_ : const_containing_;
  }

  
  
  
  
  const Value* GetValue(const base::StringPiece& ident,
                        bool counts_as_used);
  const Value* GetValue(const base::StringPiece& ident) const;

  
  
  
  
  
  
  // as reading the variable, since we assume it will be written to.
  Value* GetValueForcedToCurrentScope(const base::StringPiece& ident,
                                      const ParseNode* set_node);

  
  
  
  Value* SetValue(const base::StringPiece& ident,
                  const Value& v,
                  const ParseNode* set_node);

  
  
  
  
  bool AddTemplate(const std::string& name, const FunctionCallNode* decl);
  const FunctionCallNode* GetTemplate(const std::string& name) const;

  
  void MarkUsed(const base::StringPiece& ident);
  void MarkUnused(const base::StringPiece& ident);

  
  
  
  
  
  bool IsSetButUnused(const base::StringPiece& ident) const;

  
  
  bool CheckForUnusedVars(Err* err) const;

  
  
  void GetCurrentScopeValues(KeyValueMap* output) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool NonRecursiveMergeTo(Scope* dest,
                           const ParseNode* node_for_err,
                           const char* desc_for_err,
                           Err* err) const;

  
  
  Scope* MakeTargetDefaults(const std::string& target_type);

  
  
  const Scope* GetTargetDefaults(const std::string& target_type) const;

  
  const PatternList* GetSourcesAssignmentFilter() const;
  void set_sources_assignment_filter(
      scoped_ptr<PatternList> f) {
    sources_assignment_filter_ = f.Pass();
  }

  
  
  
  
  
  
  
  void SetProcessingBuildConfig();
  void ClearProcessingBuildConfig();
  bool IsProcessingBuildConfig() const;

  
  
  
  void SetProcessingImport();
  void ClearProcessingImport();
  bool IsProcessingImport() const;

  
  
  
  const SourceDir& GetSourceDir() const;
  void set_source_dir(const SourceDir& d) { source_dir_ = d; }

  
  
  
  
  
  
  
  
  
  
  void SetProperty(const void* key, void* value);
  void* GetProperty(const void* key, const Scope** found_on_scope) const;

 private:
  friend class ProgrammaticProvider;

  struct Record {
    Record() : used(false) {}
    Record(const Value& v) : used(false), value(v) {}

    bool used;  
    Value value;
  };

  void AddProvider(ProgrammaticProvider* p);
  void RemoveProvider(ProgrammaticProvider* p);

  
  
  
  
  
  
  const Scope* const_containing_;
  Scope* mutable_containing_;

  const Settings* settings_;

  
  
  unsigned mode_flags_;

  typedef base::hash_map<base::StringPiece, Record> RecordMap;
  RecordMap values_;

  
  
  
  typedef base::hash_map<std::string, Scope*> NamedScopeMap;
  NamedScopeMap target_defaults_;

  
  
  scoped_ptr<PatternList> sources_assignment_filter_;

  
  
  typedef std::map<std::string, const FunctionCallNode*> TemplateMap;
  TemplateMap templates_;

  
  typedef std::map<const void*, void*> PropertyMap;
  PropertyMap properties_;

  typedef std::set<ProgrammaticProvider*> ProviderSet;
  ProviderSet programmatic_providers_;

  SourceDir source_dir_;

  DISALLOW_COPY_AND_ASSIGN(Scope);
};

#endif  
