// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_EXTENSION_SYNC_TRAITS_H_
#define CHROME_BROWSER_SYNC_GLUE_EXTENSION_SYNC_TRAITS_H_
#pragma once

#include "chrome/browser/sync/syncable/model_type.h"

class Extension;
struct UninstalledExtensionInfo;

namespace sync_api {
class BaseNode;
class WriteNode;
}  

namespace sync_pb {
class EntitySpecifics;
class ExtensionSpecifics;
}  

namespace browser_sync {

typedef bool (*IsValidAndSyncablePredicate)(const Extension&);

typedef bool (*ShouldHandleExtensionUninstallPredicate)(
    const UninstalledExtensionInfo&);

typedef const sync_pb::ExtensionSpecifics& (*ExtensionSpecificsGetter)(
    const sync_api::BaseNode&);

typedef void (*ExtensionSpecificsSetter)(
    const sync_pb::ExtensionSpecifics&, sync_api::WriteNode*);

typedef bool (*ExtensionSpecificsEntityGetter)(
    const sync_pb::EntitySpecifics&, sync_pb::ExtensionSpecifics*);

struct ExtensionSyncTraits {
  ExtensionSyncTraits(
      syncable::ModelType model_type,
      IsValidAndSyncablePredicate is_valid_and_syncable,
      ShouldHandleExtensionUninstallPredicate
          should_handle_extension_uninstall,
      const char* root_node_tag,
      ExtensionSpecificsGetter extension_specifics_getter,
      ExtensionSpecificsSetter extension_specifics_setter,
      ExtensionSpecificsEntityGetter extension_specifics_entity_getter);
  ~ExtensionSyncTraits();

  
  const syncable::ModelType model_type;
  
  
  const IsValidAndSyncablePredicate is_valid_and_syncable;
  
  const ShouldHandleExtensionUninstallPredicate
      should_handle_extension_uninstall;
  
  const char* const root_node_tag;
  
  
  const ExtensionSpecificsGetter extension_specifics_getter;
  
  const ExtensionSpecificsSetter extension_specifics_setter;
  
  
  ExtensionSpecificsEntityGetter extension_specifics_entity_getter;
};

ExtensionSyncTraits GetExtensionSyncTraits();

ExtensionSyncTraits GetAppSyncTraits();

}  

#endif  
