// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef SYNC_SYNCABLE_NIGORI_UTIL_H_
#define SYNC_SYNCABLE_NIGORI_UTIL_H_

#include "base/compiler_specific.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/protocol/nigori_specifics.pb.h"

namespace sync_pb {
class EntitySpecifics;
}

namespace syncer {
class Cryptographer;

namespace syncable {

const char kEncryptedString[] = "encrypted";

class BaseTransaction;
class Entry;
class MutableEntry;
class WriteTransaction;

SYNC_EXPORT_PRIVATE bool VerifyUnsyncedChangesAreEncrypted(
    BaseTransaction* const trans,
    ModelTypeSet encrypted_types);

bool ProcessUnsyncedChangesForEncryption(
    WriteTransaction* const trans);

bool EntryNeedsEncryption(ModelTypeSet encrypted_types,
                          const Entry& entry);

SYNC_EXPORT_PRIVATE bool SpecificsNeedsEncryption(
    ModelTypeSet encrypted_types,
    const sync_pb::EntitySpecifics& specifics);

SYNC_EXPORT_PRIVATE bool VerifyDataTypeEncryptionForTest(
    BaseTransaction* const trans,
    ModelType type,
    bool is_encrypted) WARN_UNUSED_RESULT;

bool UpdateEntryWithEncryption(
    BaseTransaction* const trans,
    const sync_pb::EntitySpecifics& new_specifics,
    MutableEntry* entry);

SYNC_EXPORT_PRIVATE void UpdateNigoriFromEncryptedTypes(
    ModelTypeSet encrypted_types,
    bool encrypt_everything,
    sync_pb::NigoriSpecifics* nigori);

ModelTypeSet GetEncryptedTypesFromNigori(
    const sync_pb::NigoriSpecifics& nigori);

}  
}  

#endif  
