// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEB_MODAL_WEB_CONTENTS_MODAL_DIALOG_HOST_H_
#define COMPONENTS_WEB_MODAL_WEB_CONTENTS_MODAL_DIALOG_HOST_H_

#include "components/web_modal/modal_dialog_host.h"

namespace gfx {
class Size;
}

namespace web_modal {

class WebContentsModalDialogHost : public ModalDialogHost {
 public:
  virtual ~WebContentsModalDialogHost();

  
  virtual gfx::Size GetMaximumDialogSize() = 0;
};

}  

#endif  
