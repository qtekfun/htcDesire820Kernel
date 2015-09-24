// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THUMBNAILS_THUMBNAILING_CONTEXT_H_
#define CHROME_BROWSER_THUMBNAILS_THUMBNAILING_CONTEXT_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/thumbnails/thumbnail_service.h"
#include "chrome/common/thumbnail_score.h"
#include "content/public/browser/web_contents.h"
#include "ui/gfx/size.h"

namespace thumbnails {

enum ClipResult {
  
  CLIP_RESULT_UNPROCESSED,
  
  CLIP_RESULT_SOURCE_IS_SMALLER,
  
  CLIP_RESULT_MUCH_WIDER_THAN_TALL,
  
  CLIP_RESULT_WIDER_THAN_TALL,
  
  CLIP_RESULT_TALLER_THAN_WIDE,
  
  CLIP_RESULT_NOT_CLIPPED,
  
  CLIP_RESULT_SOURCE_SAME_AS_TARGET,
};

struct ThumbnailingContext : base::RefCountedThreadSafe<ThumbnailingContext> {
  ThumbnailingContext(content::WebContents* web_contents,
                      ThumbnailService* receiving_service,
                      bool load_interrupted);

  
  static ThumbnailingContext* CreateThumbnailingContextForTest() {
    return new ThumbnailingContext();
  }

  scoped_refptr<ThumbnailService> service;
  GURL url;
  ClipResult clip_result;
  gfx::Size requested_copy_size;
  ThumbnailScore score;

 private:
  ThumbnailingContext();
  ~ThumbnailingContext();

  friend class base::RefCountedThreadSafe<ThumbnailingContext>;
};

}

#endif  
