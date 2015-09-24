// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_EDIT_CONTROLLER_H_
#define CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_EDIT_CONTROLLER_H_

#include "base/strings/string16.h"
#include "content/public/common/page_transition_types.h"
#include "ui/base/window_open_disposition.h"
#include "url/gurl.h"

class CommandUpdater;
class InstantController;
class ToolbarModel;

namespace content {
class WebContents;
}

namespace gfx {
class Image;
}


class OmniboxEditController {
 public:
  void OnAutocompleteAccept(const GURL& destination_url,
                            WindowOpenDisposition disposition,
                            content::PageTransition transition);

  
  
  virtual void Update(const content::WebContents* contents) = 0;

  
  
  
  virtual void OnChanged() = 0;

  
  virtual void OnSetFocus() = 0;

  
  virtual InstantController* GetInstant() = 0;

  
  virtual content::WebContents* GetWebContents() = 0;

  virtual ToolbarModel* GetToolbarModel() = 0;
  virtual const ToolbarModel* GetToolbarModel() const = 0;

 protected:
  explicit OmniboxEditController(CommandUpdater* command_updater);
  virtual ~OmniboxEditController();

  CommandUpdater* command_updater() { return command_updater_; }
  GURL destination_url() const { return destination_url_; }
  WindowOpenDisposition disposition() const { return disposition_; }
  content::PageTransition transition() const { return transition_; }

 private:
  CommandUpdater* command_updater_;

  
  GURL destination_url_;
  WindowOpenDisposition disposition_;
  content::PageTransition transition_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxEditController);
};

#endif  
