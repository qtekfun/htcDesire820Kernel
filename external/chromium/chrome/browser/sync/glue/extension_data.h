// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_EXTENSION_DATA_H_
#define CHROME_BROWSER_SYNC_GLUE_EXTENSION_DATA_H_
#pragma once


#include <map>

#include "chrome/browser/sync/protocol/extension_specifics.pb.h"

namespace browser_sync {

class ExtensionData {
 public:
  enum Source {
    CLIENT,
    SERVER,
  };

  
  
  
  static ExtensionData FromData(
      Source source, const sync_pb::ExtensionSpecifics& data);

  ~ExtensionData();

  

  
  
  const sync_pb::ExtensionSpecifics& merged_data() const;

  
  
  bool NeedsUpdate(Source source) const;

  
  
  
  void SetData(
      Source source, bool merge_user_properties,
      const sync_pb::ExtensionSpecifics& data);

  
  
  void ResolveData(Source source);

 private:
  typedef std::map<Source, sync_pb::ExtensionSpecifics> SourceDataMap;
  SourceDataMap source_data_;
  sync_pb::ExtensionSpecifics merged_data_;

  
  ExtensionData();
};

}  

#endif  
