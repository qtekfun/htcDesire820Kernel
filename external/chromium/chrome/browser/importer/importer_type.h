// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_IMPORTER_TYPE_H_
#define CHROME_BROWSER_IMPORTER_IMPORTER_TYPE_H_
#pragma once

#include "build/build_config.h"

class Importer;

namespace importer {

enum ImporterType {
  NONE_IMPORTER = -1,
#if defined(OS_WIN)
  MS_IE = 0,
#endif
  FIREFOX2 = 1,
  FIREFOX3 = 2,  
#if defined(OS_MACOSX)
  SAFARI = 3,
#endif
  GOOGLE_TOOLBAR5 = 4,
  
  BOOKMARKS_HTML = 5
};

Importer* CreateImporterByType(ImporterType type);

}  

#endif  
