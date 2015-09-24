// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_MEDIA_WEB_DATA_SOURCE_H_
#define WEBKIT_GLUE_MEDIA_WEB_DATA_SOURCE_H_

#include "media/base/filters.h"
#include "media/base/pipeline_status.h"

namespace webkit_glue {

class WebDataSource : public media::DataSource {
 public:
  WebDataSource();
  virtual ~WebDataSource();

  
  
  virtual void Initialize(const std::string& url,
                          media::PipelineStatusCallback* callback) = 0;

  
  
  
  
  
  virtual void CancelInitialize() = 0;

  
  
  
  virtual bool HasSingleOrigin() = 0;

  
  
  
  
  virtual void Abort() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(WebDataSource);
};

typedef Callback1<WebDataSource*>::Type WebDataSourceBuildObserverHack;

}  

#endif  
