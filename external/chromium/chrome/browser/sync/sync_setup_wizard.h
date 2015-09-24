// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNC_SETUP_WIZARD_H_
#define CHROME_BROWSER_SYNC_SYNC_SETUP_WIZARD_H_
#pragma once

#include "base/basictypes.h"

class SyncSetupFlow;
class SyncSetupFlowContainer;
class SyncSetupFlowHandler;

class ProfileSyncService;

class SyncSetupWizard {
 public:
  enum State {
    
    GAIA_LOGIN = 0,
    
    
    GAIA_SUCCESS,
    
    SYNC_EVERYTHING,
    
    
    
    
    
    
    
    CONFIGURE,
    
    ENTER_PASSPHRASE,
    
    
    PASSPHRASE_MIGRATION,
    
    
    FATAL_ERROR,
    
    
    SETUP_ABORTED_BY_PENDING_CLEAR,
    
    SETTING_UP,
    
    
    
    
    DONE_FIRST_TIME,
    
    DONE
  };

  explicit SyncSetupWizard(ProfileSyncService* service);
  ~SyncSetupWizard();

  
  
  
  
  
  void Step(State advance_state);

  
  
  bool IsVisible() const;

  
  
  void Focus();

  SyncSetupFlow* AttachSyncSetupHandler(SyncSetupFlowHandler* handler);

 private:
  
  
  
  
  static State GetEndStateForDiscreteRun(State start_state);

  
  static bool IsTerminalState(State state);

  ProfileSyncService* service_;

  SyncSetupFlowContainer* flow_container_;

  SyncSetupFlowHandler* flow_handler_;

  DISALLOW_COPY_AND_ASSIGN(SyncSetupWizard);
};

#endif  
