// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_PLUGINS_PLUGIN_UMA_H_
#define CHROME_RENDERER_PLUGINS_PLUGIN_UMA_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "url/gurl.h"

class PluginUMAReporter {
 public:
  enum ReportType {
    MISSING_PLUGIN,
    DISABLED_PLUGIN
  };

  
  
  enum PluginType {
    WINDOWS_MEDIA_PLAYER = 0,
    SILVERLIGHT = 1,
    REALPLAYER = 2,
    JAVA = 3,
    QUICKTIME = 4,
    OTHER = 5,  
    UNSUPPORTED_MIMETYPE,
    UNSUPPORTED_EXTENSION,
    
    BROWSER_PLUGIN = 10,
    SHOCKWAVE_FLASH,
    WIDEVINE_CDM,
    
    PLUGIN_TYPE_MAX
  };

  
  class UMASender {
   public:
    virtual ~UMASender() {}
    virtual void SendPluginUMA(ReportType report_type,
                               PluginType plugin_type) = 0;
  };

  
  static PluginUMAReporter* GetInstance();

  void ReportPluginMissing(const std::string& plugin_mime_type,
                           const GURL& plugin_src);

  void ReportPluginDisabled(const std::string& plugin_mime_type,
                            const GURL& plugin_src);

 private:
  friend struct DefaultSingletonTraits<PluginUMAReporter>;
  friend class PluginUMATest;

  PluginUMAReporter();
  ~PluginUMAReporter();

  static bool CompareCStrings(const char* first, const char* second);
  bool CStringArrayContainsCString(const char** array,
                                   size_t array_size,
                                   const char* str);
  
  void ExtractFileExtension(const GURL& src, std::string* extension);

  PluginType GetPluginType(const std::string& plugin_mime_type,
                           const GURL& plugin_src);

  
  PluginType SrcToPluginType(const GURL& src);
  
  PluginType MimeTypeToPluginType(const std::string& mime_type);

  scoped_ptr<UMASender> report_sender_;

  DISALLOW_COPY_AND_ASSIGN(PluginUMAReporter);
};

#endif  
