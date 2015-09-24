// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_CONDITION_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_CONDITION_H_

namespace prerender {

class PrerenderCondition {
 public:
  virtual ~PrerenderCondition() {}
  virtual bool CanPrerender() const = 0;
};

}  

#endif  
