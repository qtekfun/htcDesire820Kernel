// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNC_SETUP_FLOW_H_
#define CHROME_BROWSER_SYNC_SYNC_SETUP_FLOW_H_
#pragma once

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/time.h"
#include "chrome/browser/sync/profile_sync_service.h"
#include "chrome/browser/sync/sync_setup_wizard.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/ui/webui/html_dialog_ui.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/gfx/native_widget_types.h"

class SyncSetupFlowHandler;
class SyncSetupFlowContainer;

struct SyncConfiguration {
  SyncConfiguration();
  ~SyncConfiguration();

  bool sync_everything;
  syncable::ModelTypeSet data_types;
  bool use_secondary_passphrase;
  std::string secondary_passphrase;
};

class SyncSetupFlow {
 public:
  virtual ~SyncSetupFlow();

  
  
  
  static SyncSetupFlow* Run(ProfileSyncService* service,
                            SyncSetupFlowContainer* container,
                            SyncSetupWizard::State start,
                            SyncSetupWizard::State end);

  
  static void GetArgsForGaiaLogin(
      const ProfileSyncService* service,
      DictionaryValue* args);

  
  static void GetArgsForConfigure(
      ProfileSyncService* service,
      DictionaryValue* args);

  
  static void GetArgsForEnterPassphrase(
      bool tried_creating_explicit_passphrase,
      bool tried_setting_explicit_passphrase,
      DictionaryValue* args);

  void AttachSyncSetupHandler(SyncSetupFlowHandler* handler);

  
  void Advance(SyncSetupWizard::State advance_state);

  
  
  void Focus();

  void OnUserSubmittedAuth(const std::string& username,
                           const std::string& password,
                           const std::string& captcha,
                           const std::string& access_code);

  void OnUserConfigured(const SyncConfiguration& configuration);

  
  
  void OnPassphraseEntry(const std::string& passphrase);

  
  void OnPassphraseCancel();

  
  
  
  void OnFirstPassphraseEntry(const std::string& option,
                              const std::string& passphrase);

  void OnGoToDashboard();

  void OnDialogClosed(const std::string& json_retval);

 private:
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest, InitialStepLogin);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest, ChooseDataTypesSetsPrefs);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest, DialogCancelled);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest, InvalidTransitions);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest, FullSuccessfulRunSetsPref);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest, AbortedByPendingClear);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest, DiscreteRunGaiaLogin);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest, DiscreteRunChooseDataTypes);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest,
                           DiscreteRunChooseDataTypesAbortedByPendingClear);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest, EnterPassphraseRequired);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupWizardTest, PassphraseMigration);

  
  SyncSetupFlow(SyncSetupWizard::State start_state,
                SyncSetupWizard::State end_state,
                const std::string& args,
                SyncSetupFlowContainer* container,
                ProfileSyncService* service);

  
  
  
  bool ShouldAdvance(SyncSetupWizard::State state);

  void ActivateState(SyncSetupWizard::State state);

  SyncSetupFlowContainer* container_;  
  std::string dialog_start_args_;  

  SyncSetupWizard::State current_state_;
  SyncSetupWizard::State end_state_;  

  
  base::TimeTicks login_start_time_;

  
  SyncSetupFlowHandler* flow_handler_;

  
  ProfileSyncService* service_;

  
  
  bool tried_creating_explicit_passphrase_;
  bool tried_setting_explicit_passphrase_;

  DISALLOW_COPY_AND_ASSIGN(SyncSetupFlow);
};

class SyncSetupFlowContainer {
 public:
  SyncSetupFlowContainer() : flow_(NULL) { }
  void set_flow(SyncSetupFlow* flow) {
    DCHECK(!flow_ || !flow);
    flow_ = flow;
  }

  SyncSetupFlow* get_flow() { return flow_; }
 private:
  SyncSetupFlow* flow_;

  DISALLOW_COPY_AND_ASSIGN(SyncSetupFlowContainer);
};

#endif  
