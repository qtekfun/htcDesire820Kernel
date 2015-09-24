// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_BACKGROUND_PRINTING_MANAGER_H_
#define CHROME_BROWSER_PRINTING_BACKGROUND_PRINTING_MANAGER_H_

#include <map>
#include <set>

#include "base/compiler_specific.h"
#include "base/threading/non_thread_safe.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace content {
class RenderProcessHost;
class WebContents;
}

namespace printing {

class BackgroundPrintingManager : public base::NonThreadSafe,
                                  public content::NotificationObserver {
 public:
  typedef std::set<content::WebContents*> WebContentsSet;

  BackgroundPrintingManager();
  virtual ~BackgroundPrintingManager();

  
  
  
  void OwnPrintPreviewDialog(content::WebContents* preview_dialog);

  
  bool HasPrintPreviewDialog(content::WebContents* preview_dialog);

  
  WebContentsSet::const_iterator begin();
  WebContentsSet::const_iterator end();

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void OnRendererProcessClosed(content::RenderProcessHost* rph);
  void OnPrintJobReleased(content::WebContents* preview_contents);
  void OnWebContentsDestroyed(content::WebContents* preview_contents);

  
  void DeletePreviewContents(content::WebContents* preview_contents);

  
  
  bool HasSharedRenderProcessHost(const WebContentsSet& set,
                                  content::WebContents* preview_contents);

  
  
  WebContentsSet printing_contents_set_;

  
  
  WebContentsSet printing_contents_pending_deletion_set_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundPrintingManager);
};

}  

#endif  
