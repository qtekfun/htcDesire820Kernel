// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_IMPORTER_IMPORTER_CREATOR_H_
#define CHROME_UTILITY_IMPORTER_IMPORTER_CREATOR_H_

#include "chrome/common/importer/importer_type.h"

class Importer;

namespace importer {

Importer* CreateImporterByType(ImporterType type);

}  

#endif  
