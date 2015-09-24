// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_THEME_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_THEME_MODEL_ASSOCIATOR_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/sync/glue/model_associator.h"
#include "chrome/browser/sync/syncable/model_type.h"

class ProfileSyncService;

namespace browser_sync {

class UnrecoverableErrorHandler;

class ThemeModelAssociator : public AssociatorInterface {
 public:
  explicit ThemeModelAssociator(ProfileSyncService* sync_service);
  virtual ~ThemeModelAssociator();

  
  static syncable::ModelType model_type() { return syncable::THEMES; }

  
  virtual bool AssociateModels();
  virtual bool DisassociateModels();
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes);
  virtual void AbortAssociation() {
    
    
  }
  virtual bool CryptoReadyIfNecessary();

 private:
  ProfileSyncService* sync_service_;

  DISALLOW_COPY_AND_ASSIGN(ThemeModelAssociator);
};

}  

#endif  
