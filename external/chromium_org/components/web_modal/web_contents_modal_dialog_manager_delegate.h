// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEB_MODAL_WEB_CONTENTS_MODAL_DIALOG_MANAGER_DELEGATE_H_
#define COMPONENTS_WEB_MODAL_WEB_CONTENTS_MODAL_DIALOG_MANAGER_DELEGATE_H_

namespace content {
class WebContents;
}

namespace gfx {
class Point;
}

namespace web_modal {

class WebContentsModalDialogHost;

class WebContentsModalDialogManagerDelegate {
 public:
  
  
  
  
  virtual void SetWebContentsBlocked(content::WebContents* web_contents,
                                     bool blocked);

  
  
  virtual WebContentsModalDialogHost* GetWebContentsModalDialogHost();

  
  virtual bool IsWebContentsVisible(content::WebContents* web_contents);

 protected:
  virtual ~WebContentsModalDialogManagerDelegate();
};

}  

#endif  
