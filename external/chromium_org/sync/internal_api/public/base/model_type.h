// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef SYNC_INTERNAL_API_PUBLIC_BASE_MODEL_TYPE_H_
#define SYNC_INTERNAL_API_PUBLIC_BASE_MODEL_TYPE_H_

#include <set>
#include <string>

#include "base/logging.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/enum_set.h"

namespace base {
class ListValue;
class StringValue;
class Value;
}

namespace sync_pb {
class EntitySpecifics;
class SyncEntity;
}

namespace syncer {


enum ModelType {
  
  
  
  
  UNSPECIFIED,
  
  
  TOP_LEVEL_FOLDER,

  
  
  
  
  
  
  
  
  BOOKMARKS,
  FIRST_USER_MODEL_TYPE = BOOKMARKS,  
  FIRST_REAL_MODEL_TYPE = FIRST_USER_MODEL_TYPE,

  
  PREFERENCES,
  
  PASSWORDS,
  
  AUTOFILL_PROFILE,
  
  AUTOFILL,
  
  THEMES,
  
  TYPED_URLS,
  
  EXTENSIONS,
  
  SEARCH_ENGINES,
  
  SESSIONS,
  
  APPS,
  
  APP_SETTINGS,
  
  EXTENSION_SETTINGS,
  
  APP_NOTIFICATIONS,
  
  HISTORY_DELETE_DIRECTIVES,
  
  SYNCED_NOTIFICATIONS,
  
  DICTIONARY,
  
  FAVICON_IMAGES,
  
  FAVICON_TRACKING,
  
  
  PRIORITY_PREFERENCES,
  
  MANAGED_USER_SETTINGS,
  
  
  
  MANAGED_USERS,
  
  ARTICLES,
  
  APP_LIST,

  
  
  
  

  
  
  PROXY_TABS,

  FIRST_PROXY_TYPE = PROXY_TABS,
  LAST_PROXY_TYPE = PROXY_TABS,

  LAST_USER_MODEL_TYPE = PROXY_TABS,

  
  
  NIGORI,
  FIRST_CONTROL_MODEL_TYPE = NIGORI,
  
  DEVICE_INFO,
  
  EXPERIMENTS,
  LAST_CONTROL_MODEL_TYPE = EXPERIMENTS,

  LAST_REAL_MODEL_TYPE = LAST_CONTROL_MODEL_TYPE,

  
  
  
  
  
  

  MODEL_TYPE_COUNT,
};

typedef EnumSet<ModelType, FIRST_REAL_MODEL_TYPE, LAST_REAL_MODEL_TYPE>
    ModelTypeSet;
typedef EnumSet<ModelType, UNSPECIFIED, LAST_REAL_MODEL_TYPE>
    FullModelTypeSet;

inline ModelType ModelTypeFromInt(int i) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, MODEL_TYPE_COUNT);
  return static_cast<ModelType>(i);
}

SYNC_EXPORT void AddDefaultFieldValue(ModelType datatype,
                                      sync_pb::EntitySpecifics* specifics);

SYNC_EXPORT_PRIVATE ModelType GetModelType(
    const sync_pb::SyncEntity& sync_entity);

SYNC_EXPORT ModelType GetModelTypeFromSpecifics(
    const sync_pb::EntitySpecifics& specifics);

SYNC_EXPORT ModelTypeSet ProtocolTypes();

SYNC_EXPORT ModelTypeSet UserTypes();

SYNC_EXPORT ModelTypeSet UserSelectableTypes();
SYNC_EXPORT bool IsUserSelectableType(ModelType model_type);

SYNC_EXPORT_PRIVATE ModelTypeSet EncryptableUserTypes();

SYNC_EXPORT ModelTypeSet PriorityUserTypes();

SYNC_EXPORT ModelTypeSet ProxyTypes();

SYNC_EXPORT ModelTypeSet ControlTypes();

SYNC_EXPORT bool IsControlType(ModelType model_type);

SYNC_EXPORT ModelTypeSet CoreTypes();
SYNC_EXPORT ModelTypeSet PriorityCoreTypes();

SYNC_EXPORT_PRIVATE ModelType GetModelTypeFromSpecificsFieldNumber(
    int field_number);

SYNC_EXPORT int GetSpecificsFieldNumberFromModelType(
    ModelType model_type);

FullModelTypeSet ToFullModelTypeSet(ModelTypeSet in);


SYNC_EXPORT const char* ModelTypeToString(ModelType model_type);

SYNC_EXPORT int ModelTypeToHistogramInt(ModelType model_type);

SYNC_EXPORT_PRIVATE base::StringValue* ModelTypeToValue(ModelType model_type);

SYNC_EXPORT_PRIVATE ModelType ModelTypeFromValue(const base::Value& value);

SYNC_EXPORT ModelType ModelTypeFromString(
    const std::string& model_type_string);

SYNC_EXPORT std::string ModelTypeSetToString(ModelTypeSet model_types);

SYNC_EXPORT base::ListValue* ModelTypeSetToValue(ModelTypeSet model_types);

SYNC_EXPORT ModelTypeSet ModelTypeSetFromValue(const base::ListValue& value);

SYNC_EXPORT std::string ModelTypeToRootTag(ModelType type);

bool RealModelTypeToNotificationType(ModelType model_type,
                                     std::string* notification_type);

SYNC_EXPORT bool NotificationTypeToRealModelType(
    const std::string& notification_type,
    ModelType* model_type);

SYNC_EXPORT bool IsRealDataType(ModelType model_type);

SYNC_EXPORT bool IsActOnceDataType(ModelType model_type);

}  

#endif  
