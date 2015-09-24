// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SYNC_SYNCABLE_MODEL_TYPE_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_MODEL_TYPE_H_
#pragma once

#include <bitset>
#include <set>
#include <string>

#include "base/logging.h"
#include "base/time.h"

class ListValue;
class StringValue;

namespace sync_pb {
class EntitySpecifics;
class SyncEntity;
}

namespace syncable {

enum ModelType {
  
  
  
  
  UNSPECIFIED,
  
  
  TOP_LEVEL_FOLDER,

  
  
  
  
  
  
  
  
  BOOKMARKS,
  FIRST_REAL_MODEL_TYPE = BOOKMARKS,  

  
  PREFERENCES,
  
  PASSWORDS,
    
  AUTOFILL_PROFILE,
  
  AUTOFILL,

  
  THEMES,
  
  TYPED_URLS,
  
  EXTENSIONS,
  
  NIGORI,
  
  SESSIONS,
  
  APPS,

  MODEL_TYPE_COUNT,
};

typedef std::bitset<MODEL_TYPE_COUNT> ModelTypeBitSet;
typedef std::set<ModelType> ModelTypeSet;

inline ModelType ModelTypeFromInt(int i) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, MODEL_TYPE_COUNT);
  return static_cast<ModelType>(i);
}

void AddDefaultExtensionValue(syncable::ModelType datatype,
                              sync_pb::EntitySpecifics* specifics);

ModelType GetModelType(const sync_pb::SyncEntity& sync_entity);

ModelType GetModelTypeFromSpecifics(const sync_pb::EntitySpecifics& specifics);

ModelType GetModelTypeFromExtensionFieldNumber(int field_number);

int GetExtensionFieldNumberFromModelType(ModelType model_type);

std::string ModelTypeToString(ModelType model_type);

StringValue* ModelTypeToValue(ModelType model_type);

std::string ModelTypeSetToString(const ModelTypeSet& model_types);

ModelType ModelTypeFromString(const std::string& model_type_string);

bool ModelTypeBitSetFromString(
    const std::string& model_type_bitset_string,
    ModelTypeBitSet* model_types);

ModelTypeBitSet ModelTypeBitSetFromSet(const ModelTypeSet& set);

ListValue* ModelTypeBitSetToValue(const ModelTypeBitSet& model_types);

ListValue* ModelTypeSetToValue(const ModelTypeSet& model_types);

std::string ModelTypeToRootTag(ModelType type);

void PostTimeToTypeHistogram(ModelType model_type, base::TimeDelta time);

bool RealModelTypeToNotificationType(ModelType model_type,
                                     std::string* notification_type);

bool NotificationTypeToRealModelType(const std::string& notification_type,
                                     ModelType* model_type);

}  

#endif  
