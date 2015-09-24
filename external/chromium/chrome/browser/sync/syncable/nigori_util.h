// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SYNC_SYNCABLE_NIGORI_UTIL_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_NIGORI_UTIL_H_
#pragma once

#include "chrome/browser/sync/protocol/nigori_specifics.pb.h"
#include "chrome/browser/sync/syncable/model_type.h"

namespace browser_sync {
class Cryptographer;
}

namespace syncable {

class BaseTransaction;
class ReadTransaction;
class WriteTransaction;

ModelTypeSet GetEncryptedDataTypes(BaseTransaction* const trans);

ModelTypeSet GetEncryptedDataTypesFromNigori(
    const sync_pb::NigoriSpecifics& nigori);

void FillNigoriEncryptedTypes(const ModelTypeSet& types,
    sync_pb::NigoriSpecifics* nigori);

bool VerifyUnsyncedChangesAreEncrypted(
    BaseTransaction* const trans,
    const ModelTypeSet& encrypted_types);

bool ProcessUnsyncedChangesForEncryption(
    WriteTransaction* const trans,
    const syncable::ModelTypeSet& encrypted_types,
    browser_sync::Cryptographer* cryptographer);

bool VerifyDataTypeEncryption(BaseTransaction* const trans,
                              ModelType type,
                              bool is_encrypted);

}  

#endif  
