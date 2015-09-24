// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UNDO_UNDO_MANAGER_UTILS_H_
#define CHROME_BROWSER_UNDO_UNDO_MANAGER_UTILS_H_

#include "base/basictypes.h"

class UndoManager;


class ScopedSuspendUndoTracking {
 public:
  explicit ScopedSuspendUndoTracking(UndoManager* undo_manager);
  ~ScopedSuspendUndoTracking();

 private:
  UndoManager* undo_manager_;

  DISALLOW_COPY_AND_ASSIGN(ScopedSuspendUndoTracking);
};


class ScopedGroupingAction {
 public:
  explicit ScopedGroupingAction(UndoManager* undo_manager);
  ~ScopedGroupingAction();

 private:
  UndoManager* undo_manager_;

  DISALLOW_COPY_AND_ASSIGN(ScopedGroupingAction);
};

#endif  
