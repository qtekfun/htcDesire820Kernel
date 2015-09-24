// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNCABLE_MODEL_TYPE_PAYLOAD_MAP_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_MODEL_TYPE_PAYLOAD_MAP_H_
#pragma once

#include <map>
#include <string>

#include "chrome/browser/sync/engine/model_safe_worker.h"
#include "chrome/browser/sync/syncable/model_type.h"

class DictionaryValue;

namespace syncable {

typedef std::map<ModelType, std::string> ModelTypePayloadMap;


ModelTypePayloadMap ModelTypePayloadMapFromBitSet(
    const ModelTypeBitSet& model_types,
    const std::string& payload);

ModelTypePayloadMap ModelTypePayloadMapFromRoutingInfo(
    const browser_sync::ModelSafeRoutingInfo& routes,
    const std::string& payload);

DictionaryValue* ModelTypePayloadMapToValue(
    const ModelTypePayloadMap& model_type_payloads);

void CoalescePayloads(ModelTypePayloadMap* original,
                      const ModelTypePayloadMap& update);

}  

#endif  
