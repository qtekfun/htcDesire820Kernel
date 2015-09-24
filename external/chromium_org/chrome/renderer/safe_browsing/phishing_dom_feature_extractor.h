// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SAFE_BROWSING_PHISHING_DOM_FEATURE_EXTRACTOR_H_
#define CHROME_RENDERER_SAFE_BROWSING_PHISHING_DOM_FEATURE_EXTRACTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "third_party/WebKit/public/web/WebDocument.h"

class GURL;

namespace blink {
class WebElement;
}

namespace content {
class RenderView;
}

namespace safe_browsing {
class FeatureExtractorClock;
class FeatureMap;

class PhishingDOMFeatureExtractor {
 public:
  
  
  typedef base::Callback<void(bool)> DoneCallback;

  
  
  
  
  
  PhishingDOMFeatureExtractor(content::RenderView* render_view,
                              FeatureExtractorClock* clock);
  ~PhishingDOMFeatureExtractor();

  
  
  
  
  
  
  
  void ExtractFeatures(FeatureMap* features, const DoneCallback& done_callback);

  
  
  
  void CancelPendingExtraction();

 private:
  struct FrameData;
  struct PageFeatureState;

  
  
  static const int kMaxTimePerChunkMs;

  
  
  
  static const int kClockCheckGranularity;

  
  
  static const int kMaxTotalTimeMs;

  
  
  
  
  void ExtractFeaturesWithTimeout();

  
  
  
  
  void HandleLink(const blink::WebElement& element);
  void HandleForm(const blink::WebElement& element);
  void HandleImage(const blink::WebElement& element);
  void HandleInput(const blink::WebElement& element);
  void HandleScript(const blink::WebElement& element);

  
  
  
  void CheckNoPendingExtraction();

  
  void RunCallback(bool success);

  
  void Clear();

  
  
  void ResetFrameData();

  
  
  blink::WebDocument GetNextDocument();

  
  
  
  bool IsExternalDomain(const GURL& url, std::string* domain) const;

  
  
  
  void InsertFeatures();

  
  content::RenderView* render_view_;

  
  FeatureExtractorClock* clock_;

  
  FeatureMap* features_;  
  DoneCallback done_callback_;

  
  
  blink::WebDocument cur_document_;

  
  
  scoped_ptr<FrameData> cur_frame_data_;

  
  
  scoped_ptr<PageFeatureState> page_feature_state_;

  
  
  base::WeakPtrFactory<PhishingDOMFeatureExtractor> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PhishingDOMFeatureExtractor);
};

}  

#endif  
