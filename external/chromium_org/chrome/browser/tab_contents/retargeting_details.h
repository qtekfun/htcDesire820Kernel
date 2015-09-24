// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_RETARGETING_DETAILS_H_
#define CHROME_BROWSER_TAB_CONTENTS_RETARGETING_DETAILS_H_

#include "url/gurl.h"

namespace content {
class WebContents;
}

struct RetargetingDetails {
  
  content::WebContents* source_web_contents;

  
  int64 source_frame_id;

  
  GURL target_url;

  
  content::WebContents* target_web_contents;

  
  bool not_yet_in_tabstrip;
};

#endif  
