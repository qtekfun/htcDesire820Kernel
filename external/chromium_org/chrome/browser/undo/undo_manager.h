// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UNDO_UNDO_MANAGER_H_
#define CHROME_BROWSER_UNDO_UNDO_MANAGER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"

class UndoOperation;


class UndoGroup {
 public:
  UndoGroup();
  ~UndoGroup();

  void AddOperation(scoped_ptr<UndoOperation> operation);
  const std::vector<UndoOperation*>& undo_operations() {
    return operations_.get();
  }
  void Undo();

 private:
  ScopedVector<UndoOperation> operations_;

  DISALLOW_COPY_AND_ASSIGN(UndoGroup);
};


class UndoManager {
 public:
  UndoManager();
  ~UndoManager();

  
  void Undo();
  void Redo();

  size_t undo_count() const { return undo_actions_.size(); }
  size_t redo_count() const { return redo_actions_.size(); }

  void AddUndoOperation(scoped_ptr<UndoOperation> operation);

  
  void StartGroupingActions();
  void EndGroupingActions();

  
  
  void SuspendUndoTracking();
  void ResumeUndoTracking();
  bool IsUndoTrakingSuspended() const;

  
  
  std::vector<UndoOperation*> GetAllUndoOperations() const;

  
  
  void RemoveAllOperations();

 private:
  void Undo(bool* performing_indicator,
            ScopedVector<UndoGroup>* active_undo_group);
  bool is_user_action() const { return !performing_undo_ && !performing_redo_; }

  
  void AddUndoGroup(UndoGroup* new_undo_group);

  
  
  ScopedVector<UndoGroup>* GetActiveUndoGroup();

  
  ScopedVector<UndoGroup> undo_actions_;
  ScopedVector<UndoGroup> redo_actions_;

  
  int group_actions_count_;

  
  scoped_ptr<UndoGroup> pending_grouped_action_;

  
  int undo_suspended_count_;

  
  
  bool performing_undo_;
  bool performing_redo_;

  DISALLOW_COPY_AND_ASSIGN(UndoManager);
};

#endif  
