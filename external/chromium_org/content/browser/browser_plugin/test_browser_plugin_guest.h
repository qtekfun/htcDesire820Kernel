// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_BROWSER_PLUGIN_TEST_BROWSER_PLUGIN_GUEST_H_
#define CONTENT_BROWSER_BROWSER_PLUGIN_TEST_BROWSER_PLUGIN_GUEST_H_

#include "base/compiler_specific.h"
#include "content/browser/browser_plugin/browser_plugin_guest.h"
#include "content/public/test/test_utils.h"
#include "ui/gfx/size.h"

namespace content {

class RenderProcessHost;
class RenderViewHost;
class WebContentsImpl;

class TestBrowserPluginGuest : public BrowserPluginGuest {
 public:
  TestBrowserPluginGuest(int instance_id, WebContentsImpl* web_contents);
  virtual ~TestBrowserPluginGuest();

  WebContentsImpl* web_contents() const;

  
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual void OnHandleInputEvent(int instance_id,
                                  const gfx::Rect& guest_window_rect,
                                  const blink::WebInputEvent* event) OVERRIDE;
  virtual void OnSetFocus(int instance_id, bool focused) OVERRIDE;
  virtual void OnTakeFocus(bool reverse) OVERRIDE;
  virtual void SetDamageBuffer(
      const BrowserPluginHostMsg_ResizeGuest_Params& params) OVERRIDE;
  virtual void DidStopLoading(RenderViewHost* render_view_host) OVERRIDE;
  virtual void OnImeCancelComposition() OVERRIDE;

  
  virtual void WasHidden() OVERRIDE;

  
  
  
  void WaitForUpdateRectMsg();
  void ResetUpdateRectCount();
  
  void WaitForDamageBufferWithSize(const gfx::Size& size);
  
  void WaitForFocus();
  
  void WaitForBlur();
  
  void WaitForAdvanceFocus();
  
  void WaitUntilHidden();
  
  void WaitForExit();
  
  void WaitForInput();
  
  void WaitForLoadStop();
  
  void WaitForViewSize(const gfx::Size& view_size);
  
  void WaitForImeCancel();

  void set_guest_hang_timeout(const base::TimeDelta& timeout) {
    guest_hang_timeout_ = timeout;
  }

 private:
  
  virtual void SendMessageToEmbedder(IPC::Message* msg) OVERRIDE;

  int update_rect_count_;
  int damage_buffer_call_count_;
  bool exit_observed_;
  bool focus_observed_;
  bool blur_observed_;
  bool advance_focus_observed_;
  bool was_hidden_observed_;
  bool set_damage_buffer_observed_;
  bool input_observed_;
  bool load_stop_observed_;
  bool ime_cancel_observed_;
  gfx::Size last_view_size_observed_;
  gfx::Size expected_auto_view_size_;

  
  bool waiting_for_damage_buffer_with_size_;
  gfx::Size expected_damage_buffer_size_;
  gfx::Size last_damage_buffer_size_;

  scoped_refptr<MessageLoopRunner> send_message_loop_runner_;
  scoped_refptr<MessageLoopRunner> crash_message_loop_runner_;
  scoped_refptr<MessageLoopRunner> focus_message_loop_runner_;
  scoped_refptr<MessageLoopRunner> blur_message_loop_runner_;
  scoped_refptr<MessageLoopRunner> advance_focus_message_loop_runner_;
  scoped_refptr<MessageLoopRunner> was_hidden_message_loop_runner_;
  scoped_refptr<MessageLoopRunner> damage_buffer_message_loop_runner_;
  scoped_refptr<MessageLoopRunner> input_message_loop_runner_;
  scoped_refptr<MessageLoopRunner> load_stop_message_loop_runner_;
  scoped_refptr<MessageLoopRunner> auto_view_size_message_loop_runner_;
  scoped_refptr<MessageLoopRunner> ime_cancel_message_loop_runner_;

  DISALLOW_COPY_AND_ASSIGN(TestBrowserPluginGuest);
};

}  

#endif  
