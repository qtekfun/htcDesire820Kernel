// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_PLUGIN_STRING_STREAM_H_
#define CONTENT_CHILD_PLUGIN_STRING_STREAM_H_

#include "content/child/npapi/plugin_stream.h"

class GURL;

namespace content {

class PluginInstance;

class PluginStringStream : public PluginStream {
 public:
  
  
  
  PluginStringStream(PluginInstance* instance,
                     const GURL& url,
                     bool notify_needed,
                     void* notify_data);

  
  void SendToPlugin(const std::string& data,
                    const std::string& mime_type);

 private:
  virtual ~PluginStringStream();

  DISALLOW_COPY_AND_ASSIGN(PluginStringStream);
};


}  

#endif 
