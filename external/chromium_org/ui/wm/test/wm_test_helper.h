// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_WM_TEST_WM_TEST_HELPER_H_
#define UI_WM_TEST_WM_TEST_HELPER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/client/window_tree_client.h"

namespace aura {
class RootWindow;
class Window;
namespace client {
class DefaultActivationClient;
class DefaultCaptureClient;
class FocusClient;
}
}

namespace gfx {
class Rect;
class Size;
}

namespace views {
namespace corewm {
class CompoundEventFilter;
class InputMethodEventFilter;
}
}

namespace wm {

class WMTestHelper : public aura::client::WindowTreeClient {
 public:
  explicit WMTestHelper(const gfx::Size& default_window_size);
  virtual ~WMTestHelper();

  aura::RootWindow* root_window() { return root_window_.get(); }

  
  virtual aura::Window* GetDefaultParent(aura::Window* context,
                                         aura::Window* window,
                                         const gfx::Rect& bounds) OVERRIDE;

 private:
  scoped_ptr<aura::RootWindow> root_window_;

  
  views::corewm::CompoundEventFilter* root_window_event_filter_;

  scoped_ptr<aura::client::DefaultCaptureClient> capture_client_;
  scoped_ptr<views::corewm::InputMethodEventFilter> input_method_filter_;
  scoped_ptr<aura::client::DefaultActivationClient> activation_client_;
  scoped_ptr<aura::client::FocusClient> focus_client_;

  DISALLOW_COPY_AND_ASSIGN(WMTestHelper);
};

}  

#endif  
