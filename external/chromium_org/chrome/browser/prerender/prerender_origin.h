// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_ORIGIN_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_ORIGIN_H_

namespace prerender {

enum Origin {
  
  
  ORIGIN_GWS_PRERENDER = 2,
  
  
  ORIGIN_OMNIBOX = 5,
  ORIGIN_NONE = 6,
  ORIGIN_LINK_REL_PRERENDER_SAMEDOMAIN = 7,
  ORIGIN_LINK_REL_PRERENDER_CROSSDOMAIN = 8,
  ORIGIN_LOCAL_PREDICTOR = 9,
  ORIGIN_EXTERNAL_REQUEST = 10,
  ORIGIN_INSTANT = 11,
  ORIGIN_MAX,
};

const char* NameFromOrigin(Origin origin);

}  

#endif  
