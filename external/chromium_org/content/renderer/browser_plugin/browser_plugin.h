// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef  CONTENT_RENDERER_BROWSER_PLUGIN_BROWSER_PLUGIN_H_
#define  CONTENT_RENDERER_BROWSER_PLUGIN_BROWSER_PLUGIN_H_

#include "third_party/WebKit/public/web/WebPlugin.h"

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#if defined(OS_WIN)
#include "base/memory/shared_memory.h"
#endif
#include "base/values.h"
#include "content/renderer/browser_plugin/browser_plugin_backing_store.h"
#include "content/renderer/browser_plugin/browser_plugin_bindings.h"
#include "content/renderer/mouse_lock_dispatcher.h"
#include "content/renderer/render_view_impl.h"
#include "third_party/WebKit/public/web/WebCompositionUnderline.h"
#include "third_party/WebKit/public/web/WebDragStatus.h"
#include "third_party/WebKit/public/web/WebWidget.h"

struct BrowserPluginHostMsg_AutoSize_Params;
struct BrowserPluginHostMsg_ResizeGuest_Params;
struct BrowserPluginMsg_Attach_ACK_Params;
struct BrowserPluginMsg_BuffersSwapped_Params;
struct BrowserPluginMsg_UpdateRect_Params;

namespace content {

class BrowserPluginCompositingHelper;
class BrowserPluginManager;
class MockBrowserPlugin;

class CONTENT_EXPORT BrowserPlugin :
    NON_EXPORTED_BASE(public blink::WebPlugin),
    public MouseLockDispatcher::LockTarget {
 public:
  RenderViewImpl* render_view() const { return render_view_.get(); }
  int render_view_routing_id() const { return render_view_routing_id_; }
  int guest_instance_id() const { return guest_instance_id_; }
  bool attached() const { return attached_; }

  static BrowserPlugin* FromContainer(blink::WebPluginContainer* container);

  bool OnMessageReceived(const IPC::Message& msg);

  
  
  void UpdateDOMAttribute(const std::string& attribute_name,
                          const std::string& attribute_value);
  
  void RemoveDOMAttribute(const std::string& attribute_name);
  
  std::string GetDOMAttributeValue(const std::string& attribute_name) const;
  
  bool HasDOMAttribute(const std::string& attribute_name) const;

  
  std::string GetNameAttribute() const;
  
  void ParseNameAttribute();
  
  bool GetAllowTransparencyAttribute() const;
  
  
  void ParseAllowTransparencyAttribute();
  
  std::string GetSrcAttribute() const;
  
  bool ParseSrcAttribute(std::string* error_message);
  
  bool GetAutoSizeAttribute() const;
  
  void ParseAutoSizeAttribute();
  
  int GetMaxHeightAttribute() const;
  
  int GetMaxWidthAttribute() const;
  
  int GetMinHeightAttribute() const;
  
  int GetMinWidthAttribute() const;
  
  void ParseSizeContraintsChanged();
  
  std::string GetPartitionAttribute() const;
  
  
  
  bool ParsePartitionAttribute(std::string* error_message);
  
  bool CanRemovePartitionAttribute(std::string* error_message);

  bool InAutoSizeBounds(const gfx::Size& size) const;

  
  NPObject* GetContentWindow() const;

  
  bool guest_crashed() const { return guest_crashed_; }
  
  bool HasNavigated() const;
  
  bool HasGuestInstanceID() const;

  
  
  static bool AttachWindowTo(const blink::WebNode& node,
                             int window_id);

  
  void UpdateGuestFocusState();
  
  bool ShouldGuestBeFocused() const;

  
  
  void UpdateDeviceScaleFactor(float device_scale_factor);

  
  void EnableCompositing(bool enable);

  
  
  
  
  bool InBounds(const gfx::Point& point) const;

  gfx::Point ToLocalCoordinates(const gfx::Point& point) const;

  
  void OnInstanceIDAllocated(int guest_instance_id);
  
  
  
  void Attach(scoped_ptr<base::DictionaryValue> extra_params);

  
  
  void DidCommitCompositorFrame();

  
  static bool ShouldForwardToBrowserPlugin(const IPC::Message& message);

  
  virtual blink::WebPluginContainer* container() const OVERRIDE;
  virtual bool initialize(blink::WebPluginContainer* container) OVERRIDE;
  virtual void destroy() OVERRIDE;
  virtual NPObject* scriptableObject() OVERRIDE;
  virtual struct _NPP* pluginNPP() OVERRIDE;
  virtual bool supportsKeyboardFocus() const OVERRIDE;
  virtual bool supportsEditCommands() const OVERRIDE;
  virtual bool supportsInputMethod() const OVERRIDE;
  virtual bool canProcessDrag() const OVERRIDE;
  virtual void paint(
      blink::WebCanvas* canvas,
      const blink::WebRect& rect) OVERRIDE;
  virtual void updateGeometry(
      const blink::WebRect& frame_rect,
      const blink::WebRect& clip_rect,
      const blink::WebVector<blink::WebRect>& cut_outs_rects,
      bool is_visible) OVERRIDE;
  virtual void updateFocus(bool focused) OVERRIDE;
  virtual void updateVisibility(bool visible) OVERRIDE;
  virtual bool acceptsInputEvents() OVERRIDE;
  virtual bool handleInputEvent(
      const blink::WebInputEvent& event,
      blink::WebCursorInfo& cursor_info) OVERRIDE;
  virtual bool handleDragStatusUpdate(blink::WebDragStatus drag_status,
                                      const blink::WebDragData& drag_data,
                                      blink::WebDragOperationsMask mask,
                                      const blink::WebPoint& position,
                                      const blink::WebPoint& screen) OVERRIDE;
  virtual void didReceiveResponse(
      const blink::WebURLResponse& response) OVERRIDE;
  virtual void didReceiveData(const char* data, int data_length) OVERRIDE;
  virtual void didFinishLoading() OVERRIDE;
  virtual void didFailLoading(const blink::WebURLError& error) OVERRIDE;
  virtual void didFinishLoadingFrameRequest(
      const blink::WebURL& url,
      void* notify_data) OVERRIDE;
  virtual void didFailLoadingFrameRequest(
      const blink::WebURL& url,
      void* notify_data,
      const blink::WebURLError& error) OVERRIDE;
  virtual bool executeEditCommand(const blink::WebString& name) OVERRIDE;
  virtual bool executeEditCommand(const blink::WebString& name,
                                  const blink::WebString& value) OVERRIDE;
  virtual bool setComposition(
      const blink::WebString& text,
      const blink::WebVector<blink::WebCompositionUnderline>& underlines,
      int selectionStart,
      int selectionEnd) OVERRIDE;
  virtual bool confirmComposition(
      const blink::WebString& text,
      blink::WebWidget::ConfirmCompositionBehavior selectionBehavior) OVERRIDE;
  virtual void extendSelectionAndDelete(int before, int after) OVERRIDE;

  
  virtual void OnLockMouseACK(bool succeeded) OVERRIDE;
  virtual void OnMouseLockLost() OVERRIDE;
  virtual bool HandleMouseLockedInputEvent(
          const blink::WebMouseEvent& event) OVERRIDE;

 private:
  friend class base::DeleteHelper<BrowserPlugin>;
  
  friend class BrowserPluginManagerImpl;
  friend class MockBrowserPluginManager;

  
  friend class MockBrowserPlugin;

  
  
  
  
  
  BrowserPlugin(RenderViewImpl* render_view, blink::WebFrame* frame);

  virtual ~BrowserPlugin();

  int width() const { return plugin_rect_.width(); }
  int height() const { return plugin_rect_.height(); }
  gfx::Rect plugin_rect() { return plugin_rect_; }
  
  int GetAdjustedMaxHeight() const;
  
  int GetAdjustedMaxWidth() const;
  
  int GetAdjustedMinHeight() const;
  
  int GetAdjustedMinWidth() const;
  BrowserPluginManager* browser_plugin_manager() const {
    return browser_plugin_manager_.get();
  }

  
  virtual float GetDeviceScaleFactor() const;

  void ShowSadGraphic();

  
  
  void ParseAttributes();

  
  
  void TriggerEvent(const std::string& event_name,
                    std::map<std::string, base::Value*>* props);

  
  virtual base::SharedMemory* CreateDamageBuffer(
      const size_t size,
      base::SharedMemoryHandle* shared_memory_handle);
  
  void SwapDamageBuffers();

  
  
  void PopulateResizeGuestParameters(
      BrowserPluginHostMsg_ResizeGuest_Params* params,
      const gfx::Rect& view_size,
      bool needs_repaint);

  
  void PopulateAutoSizeParameters(
      BrowserPluginHostMsg_AutoSize_Params* params, bool auto_size_enabled);

  
  
  void GetDamageBufferWithSizeParams(
      BrowserPluginHostMsg_AutoSize_Params* auto_size_params,
      BrowserPluginHostMsg_ResizeGuest_Params* resize_guest_params,
      bool needs_repaint);

  
  void UpdateGuestAutoSizeState(bool auto_size_enabled);

  
  
  static bool UsesDamageBuffer(
      const BrowserPluginMsg_UpdateRect_Params& params);

  
  
  bool UsesPendingDamageBuffer(
      const BrowserPluginMsg_UpdateRect_Params& params);

  
  
  void OnAdvanceFocus(int instance_id, bool reverse);
  void OnAttachACK(int instance_id,
                   const BrowserPluginMsg_Attach_ACK_Params& ack_params);
  void OnBuffersSwapped(int instance_id,
                        const BrowserPluginMsg_BuffersSwapped_Params& params);
  void OnCompositorFrameSwapped(const IPC::Message& message);
  void OnCopyFromCompositingSurface(int instance_id,
                                    int request_id,
                                    gfx::Rect source_rect,
                                    gfx::Size dest_size);
  void OnGuestContentWindowReady(int instance_id,
                                 int content_window_routing_id);
  void OnGuestGone(int instance_id);
  void OnSetCursor(int instance_id, const WebCursor& cursor);
  void OnSetMouseLock(int instance_id, bool enable);
  void OnShouldAcceptTouchEvents(int instance_id, bool accept);
  void OnUpdatedName(int instance_id, const std::string& name);
  void OnUpdateRect(int instance_id,
                    const BrowserPluginMsg_UpdateRect_Params& params);

  
  
  int guest_instance_id_;
  
  
  bool attached_;
  base::WeakPtr<RenderViewImpl> render_view_;
  
  
  
  int render_view_routing_id_;
  blink::WebPluginContainer* container_;
  scoped_ptr<BrowserPluginBindings> bindings_;
  scoped_ptr<BrowserPluginBackingStore> backing_store_;
  scoped_ptr<base::SharedMemory> current_damage_buffer_;
  scoped_ptr<base::SharedMemory> pending_damage_buffer_;
  uint32 damage_buffer_sequence_id_;
  bool paint_ack_received_;
  gfx::Rect plugin_rect_;
  float last_device_scale_factor_;
  
  SkBitmap* sad_guest_;
  bool guest_crashed_;
  scoped_ptr<BrowserPluginHostMsg_ResizeGuest_Params> pending_resize_params_;
  bool is_auto_size_state_dirty_;
  
  gfx::Size max_auto_size_;
  std::string storage_partition_id_;
  bool persist_storage_;
  bool valid_partition_id_;
  int content_window_routing_id_;
  bool plugin_focused_;
  
  
  bool visible_;

  WebCursor cursor_;

  gfx::Size last_view_size_;
  bool before_first_navigation_;
  bool mouse_locked_;

  
  
  
  scoped_refptr<BrowserPluginManager> browser_plugin_manager_;

  
  bool compositing_enabled_;
  scoped_refptr<BrowserPluginCompositingHelper> compositing_helper_;

  
  scoped_ptr<struct _NPP> npp_;

  
  GURL embedder_frame_url_;

  
  
  base::WeakPtrFactory<BrowserPlugin> weak_ptr_factory_;

  std::vector<EditCommand> edit_commands_;

  DISALLOW_COPY_AND_ASSIGN(BrowserPlugin);
};

}  

#endif  
