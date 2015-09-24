// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_HOST_H_
#define CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_HOST_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_callback_factory.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/safe_browsing/safe_browsing_service.h"
#include "content/browser/tab_contents/tab_contents_observer.h"
#include "googleurl/src/gurl.h"

class TabContents;

namespace safe_browsing {

class ClientSideDetectionService;

class ClientSideDetectionHost : public TabContentsObserver {
 public:
  
  
  explicit ClientSideDetectionHost(TabContents* tab);
  virtual ~ClientSideDetectionHost();

  
  virtual bool OnMessageReceived(const IPC::Message& message);

  
  
  
  virtual void DidNavigateMainFramePostCommit(
      const NavigationController::LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params);

 private:
  friend class ClientSideDetectionHostTest;
  class ShouldClassifyUrlRequest;
  friend class ShouldClassifyUrlRequest;

  
  void OnDetectedPhishingSite(const std::string& verdict);

  
  
  
  void MaybeShowPhishingWarning(GURL phishing_url, bool is_phishing);

  
  
  void set_client_side_detection_service(ClientSideDetectionService* service);

  
  
  void set_safe_browsing_service(SafeBrowsingService* service);

  
  ClientSideDetectionService* csd_service_;
  
  scoped_refptr<SafeBrowsingService> sb_service_;
  
  
  scoped_refptr<ShouldClassifyUrlRequest> classification_request_;

  base::ScopedCallbackFactory<ClientSideDetectionHost> cb_factory_;

  DISALLOW_COPY_AND_ASSIGN(ClientSideDetectionHost);
};

}  

#endif  
