// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMMAND_UPDATER_H_
#define CHROME_BROWSER_COMMAND_UPDATER_H_

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "ui/base/window_open_disposition.h"

class CommandObserver;
class CommandUpdaterDelegate;

class CommandUpdater {
 public:
  
  
  explicit CommandUpdater(CommandUpdaterDelegate* delegate);
  ~CommandUpdater();

  
  bool SupportsCommand(int id) const;

  
  
  bool IsCommandEnabled(int id) const;

  
  
  
  
  bool ExecuteCommand(int id);

  
  
  
  
  bool ExecuteCommandWithDisposition(int id, WindowOpenDisposition disposition);

  
  
  void AddCommandObserver(int id, CommandObserver* observer);

  
  void RemoveCommandObserver(int id, CommandObserver* observer);

  
  void RemoveCommandObserver(CommandObserver* observer);

  
  
  
  
  void UpdateCommandEnabled(int id, bool state);

 private:
  
  
  class Command;

  
  
  Command* GetCommand(int id, bool create);

  
  CommandUpdaterDelegate* delegate_;

  
  typedef base::hash_map<int, Command*> CommandMap;
  CommandMap commands_;

  DISALLOW_COPY_AND_ASSIGN(CommandUpdater);
};

#endif  
