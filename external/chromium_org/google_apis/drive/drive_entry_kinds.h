// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_DRIVE_ENTRY_KINDS_H_
#define GOOGLE_APIS_DRIVE_DRIVE_ENTRY_KINDS_H_

namespace google_apis {

enum DriveEntryKind {
  ENTRY_KIND_UNKNOWN,
  
  ENTRY_KIND_ITEM,
  ENTRY_KIND_SITE,
  
  ENTRY_KIND_DOCUMENT,
  ENTRY_KIND_SPREADSHEET,
  ENTRY_KIND_PRESENTATION,
  ENTRY_KIND_DRAWING,
  ENTRY_KIND_TABLE,
  ENTRY_KIND_FORM,
  
  ENTRY_KIND_EXTERNAL_APP,
  
  ENTRY_KIND_FOLDER,
  
  ENTRY_KIND_FILE,
  ENTRY_KIND_PDF,

  
  ENTRY_KIND_MAX_VALUE,
};

}  

#endif  
