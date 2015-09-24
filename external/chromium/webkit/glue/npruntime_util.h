// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_NPRUNTIME_UTIL_H_
#define WEBKIT_GLUE_NPRUNTIME_UTIL_H_

#include "third_party/npapi/bindings/npruntime.h"

class Pickle;

namespace webkit_glue {

bool SerializeNPIdentifier(NPIdentifier identifier, Pickle* pickle);
bool DeserializeNPIdentifier(const Pickle& pickle, void** pickle_iter,
                             NPIdentifier* identifier);

}  

#endif  
