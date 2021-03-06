// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PDF_PDF_UNSUPPORTED_FEATURE_H_
#define CHROME_BROWSER_UI_PDF_PDF_UNSUPPORTED_FEATURE_H_

#include "base/basictypes.h"

namespace content {
class WebContents;
}

void PDFHasUnsupportedFeature(content::WebContents* web_contents);

#endif  
