// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SAFE_BROWSING_PHISHING_CLASSIFIER_DELEGATE_H_
#define CHROME_RENDERER_SAFE_BROWSING_PHISHING_CLASSIFIER_DELEGATE_H_

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/renderer/render_process_observer.h"
#include "content/public/renderer/render_view_observer.h"
#include "url/gurl.h"

namespace safe_browsing {
class ClientPhishingRequest;
class PhishingClassifier;
class Scorer;

class PhishingClassifierFilter : public content::RenderProcessObserver {
 public:
  static PhishingClassifierFilter* Create();
  virtual ~PhishingClassifierFilter();

  virtual bool OnControlMessageReceived(const IPC::Message& message) OVERRIDE;

 private:
  PhishingClassifierFilter();
  void OnSetPhishingModel(const std::string& model);

  DISALLOW_COPY_AND_ASSIGN(PhishingClassifierFilter);
};

class PhishingClassifierDelegate : public content::RenderViewObserver {
 public:
  
  
  
  static PhishingClassifierDelegate* Create(content::RenderView* render_view,
                                            PhishingClassifier* classifier);
  virtual ~PhishingClassifierDelegate();

  
  
  void SetPhishingScorer(const safe_browsing::Scorer* scorer);

  
  
  
  
  
  void PageCaptured(base::string16* page_text, bool preliminary_capture);

  

  
  
  
  
  virtual void DidCommitProvisionalLoad(blink::WebFrame* frame,
                                        bool is_new_navigation) OVERRIDE;

 private:
  friend class PhishingClassifierDelegateTest;

  PhishingClassifierDelegate(content::RenderView* render_view,
                             PhishingClassifier* classifier);

  enum CancelClassificationReason {
    NAVIGATE_AWAY,
    NAVIGATE_WITHIN_PAGE,
    PAGE_RECAPTURED,
    SHUTDOWN,
    NEW_PHISHING_SCORER,
    CANCEL_CLASSIFICATION_MAX  
  };

  
  void CancelPendingClassification(CancelClassificationReason reason);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  
  
  
  
  void OnStartPhishingDetection(const GURL& url);

  
  void ClassificationDone(const ClientPhishingRequest& verdict);

  
  GURL GetToplevelUrl();

  
  void MaybeStartClassification();

  
  
  scoped_ptr<PhishingClassifier> classifier_;

  
  
  GURL last_url_received_from_browser_;

  
  
  GURL last_finished_load_url_;

  
  
  
  
  content::PageTransition last_main_frame_transition_;

  
  
  
  GURL last_url_sent_to_classifier_;

  
  
  
  
  
  base::string16 classifier_page_text_;

  
  
  
  bool have_page_text_;

  
  bool is_classifying_;

  DISALLOW_COPY_AND_ASSIGN(PhishingClassifierDelegate);
};

}  

#endif  
