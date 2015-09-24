// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_STATS_COLLECTION_CONTROLLER_H_
#define CONTENT_RENDERER_STATS_COLLECTION_CONTROLLER_H_

#include "base/basictypes.h"
#include "gin/wrappable.h"

namespace blink {
class WebFrame;
}

namespace content {

class StatsCollectionController
    : public gin::Wrappable<StatsCollectionController> {
 public:
  static gin::WrapperInfo kWrapperInfo;

  static void Install(blink::WebFrame* frame);

 private:
  StatsCollectionController();
  virtual ~StatsCollectionController();

  
  std::string GetHistogram(const std::string& histogram_name);

  
  
  std::string GetBrowserHistogram(const std::string& histogram_name);

  
  std::string GetTabLoadTiming();

  DISALLOW_COPY_AND_ASSIGN(StatsCollectionController);
};

}  

#endif  
