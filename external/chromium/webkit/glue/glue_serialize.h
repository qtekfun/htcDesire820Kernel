// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_GLUE_SERIALIZE_H_
#define WEBKIT_GLUE_GLUE_SERIALIZE_H_

#include <string>
#include "third_party/WebKit/Source/WebKit/chromium/public/WebHistoryItem.h"

namespace webkit_glue {

std::string HistoryItemToString(
    const WebKit::WebHistoryItem& item);
WebKit::WebHistoryItem HistoryItemFromString(
    const std::string& serialized_item);

void HistoryItemToVersionedString(
    const WebKit::WebHistoryItem& item, int version,
    std::string* serialized_item);

}  

#endif  
