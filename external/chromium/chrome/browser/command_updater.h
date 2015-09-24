// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMMAND_UPDATER_H_
#define CHROME_BROWSER_COMMAND_UPDATER_H_
#pragma once

#include "base/basictypes.h"
#include "base/hash_tables.h"

class CommandUpdater {
 public:
  
  
  class CommandUpdaterDelegate {
   public:
    
    virtual void ExecuteCommand(int id) = 0;

   protected:
    virtual ~CommandUpdaterDelegate();
  };

  
  
  explicit CommandUpdater(CommandUpdaterDelegate* handler);
  virtual ~CommandUpdater();

  
  bool SupportsCommand(int id) const;

  
  
  bool IsCommandEnabled(int id) const;

  
  
  
  void ExecuteCommand(int id);

  
  
  class CommandObserver {
   public:
    
    
    virtual void EnabledStateChangedForCommand(int id, bool enabled) = 0;

   protected:
    virtual ~CommandObserver();
  };

  
  
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

  CommandUpdater();
  DISALLOW_COPY_AND_ASSIGN(CommandUpdater);
};

#endif  
