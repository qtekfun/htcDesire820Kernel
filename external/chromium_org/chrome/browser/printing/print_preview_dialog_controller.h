// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_PREVIEW_DIALOG_CONTROLLER_H_
#define CHROME_BROWSER_PRINTING_PRINT_PREVIEW_DIALOG_CONTROLLER_H_

#include <map>

#include "base/memory/ref_counted.h"
#include "chrome/browser/sessions/session_id.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class GURL;

namespace content {
struct LoadCommittedDetails;
class RenderProcessHost;
class WebContents;
}

namespace printing {

class PrintPreviewDialogController
    : public base::RefCounted<PrintPreviewDialogController>,
      public content::NotificationObserver {
 public:
  PrintPreviewDialogController();

  static PrintPreviewDialogController* GetInstance();

  
  
  static void PrintPreview(content::WebContents* initiator);

  
  
  content::WebContents* GetOrCreatePreviewDialog(
      content::WebContents* initiator);

  
  
  
  content::WebContents* GetPrintPreviewForContents(
      content::WebContents* contents) const;

  
  
  content::WebContents* GetInitiator(content::WebContents* preview_dialog);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  static bool IsPrintPreviewDialog(content::WebContents* contents);

  
  static bool IsPrintPreviewURL(const GURL& url);

  
  void EraseInitiatorInfo(content::WebContents* preview_dialog);

  bool is_creating_print_preview_dialog() const {
    return is_creating_print_preview_dialog_;
  }

 private:
  friend class base::RefCounted<PrintPreviewDialogController>;

  
  
  
  typedef std::map<content::WebContents*, content::WebContents*>
      PrintPreviewDialogMap;

  virtual ~PrintPreviewDialogController();

  
  
  void OnRendererProcessClosed(content::RenderProcessHost* rph);

  
  
  void OnWebContentsDestroyed(content::WebContents* contents);

  
  
  void OnNavEntryCommitted(content::WebContents* contents,
                           content::LoadCommittedDetails* details);

  
  content::WebContents* CreatePrintPreviewDialog(
      content::WebContents* initiator);

  
  
  void SaveInitiatorTitle(content::WebContents* preview_dialog);

  
  void AddObservers(content::WebContents* contents);
  void RemoveObservers(content::WebContents* contents);

  
  void RemoveInitiator(content::WebContents* initiator);
  void RemovePreviewDialog(content::WebContents* preview_dialog);

  
  PrintPreviewDialogMap preview_dialog_map_;

  
  content::NotificationRegistrar registrar_;

  
  
  bool waiting_for_new_preview_page_;

  
  
  bool is_creating_print_preview_dialog_;

  DISALLOW_COPY_AND_ASSIGN(PrintPreviewDialogController);
};

}  

#endif  
