// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SAFE_BROWSING_PHISHING_CLASSIFIER_H_
#define CHROME_RENDERER_SAFE_BROWSING_PHISHING_CLASSIFIER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"

namespace content {
class RenderView;
}

namespace safe_browsing {
class ClientPhishingRequest;
class FeatureExtractorClock;
class FeatureMap;
class PhishingDOMFeatureExtractor;
class PhishingTermFeatureExtractor;
class PhishingUrlFeatureExtractor;
class Scorer;

class PhishingClassifier {
 public:
  
  
  
  
  
  
  typedef base::Callback<void(const ClientPhishingRequest& )>
      DoneCallback;

  static const float kInvalidScore;

  
  
  
  
  PhishingClassifier(content::RenderView* render_view,
                     FeatureExtractorClock* clock);
  virtual ~PhishingClassifier();

  
  
  
  
  void set_phishing_scorer(const Scorer* scorer);

  
  
  bool is_ready() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void BeginClassification(const base::string16* page_text,
                                   const DoneCallback& callback);

  
  
  
  
  virtual void CancelPendingClassification();

 private:
  
  static const float kPhishyThreshold;

  
  
  void BeginFeatureExtraction();

  
  
  
  void DOMExtractionFinished(bool success);

  
  
  
  
  void TermExtractionFinished(bool success);

  
  
  
  void CheckNoPendingClassification();

  
  void RunCallback(const ClientPhishingRequest& verdict);

  
  
  void RunFailureCallback();

  
  void Clear();

  content::RenderView* render_view_;  
  const Scorer* scorer_;  
  scoped_ptr<FeatureExtractorClock> clock_;
  scoped_ptr<PhishingUrlFeatureExtractor> url_extractor_;
  scoped_ptr<PhishingDOMFeatureExtractor> dom_extractor_;
  scoped_ptr<PhishingTermFeatureExtractor> term_extractor_;

  
  scoped_ptr<FeatureMap> features_;
  const base::string16* page_text_;  
  DoneCallback done_callback_;

  
  
  base::WeakPtrFactory<PhishingClassifier> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PhishingClassifier);
};

}  

#endif  
