// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_PLUGIN_INSTANCE_IMPL_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_PLUGIN_INSTANCE_IMPL_H_

#include <list>
#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "cc/layers/content_layer_client.h"
#include "cc/layers/texture_layer_client.h"
#include "content/common/content_export.h"
#include "content/public/renderer/pepper_plugin_instance.h"
#include "content/renderer/mouse_lock_dispatcher.h"
#include "content/renderer/pepper/ppp_pdf.h"
#include "ppapi/c/dev/pp_cursor_type_dev.h"
#include "ppapi/c/dev/ppp_find_dev.h"
#include "ppapi/c/dev/ppp_printing_dev.h"
#include "ppapi/c/dev/ppp_selection_dev.h"
#include "ppapi/c/dev/ppp_text_input_dev.h"
#include "ppapi/c/dev/ppp_zoom_dev.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb_audio_config.h"
#include "ppapi/c/ppb_gamepad.h"
#include "ppapi/c/ppb_input_event.h"
#include "ppapi/c/ppp_graphics_3d.h"
#include "ppapi/c/ppp_input_event.h"
#include "ppapi/c/ppp_messaging.h"
#include "ppapi/c/ppp_mouse_lock.h"
#include "ppapi/c/private/ppb_content_decryptor_private.h"
#include "ppapi/c/private/ppp_instance_private.h"
#include "ppapi/shared_impl/ppb_instance_shared.h"
#include "ppapi/shared_impl/ppb_view_shared.h"
#include "ppapi/shared_impl/singleton_resource_id.h"
#include "ppapi/shared_impl/tracked_callback.h"
#include "ppapi/thunk/ppb_gamepad_api.h"
#include "ppapi/thunk/resource_creation_api.h"
#include "skia/ext/refptr.h"
#include "third_party/WebKit/public/platform/WebCanvas.h"
#include "third_party/WebKit/public/platform/WebString.h"
#include "third_party/WebKit/public/platform/WebURLLoaderClient.h"
#include "third_party/WebKit/public/platform/WebURLResponse.h"
#include "third_party/WebKit/public/web/WebPlugin.h"
#include "third_party/WebKit/public/web/WebUserGestureToken.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/gfx/rect.h"
#include "url/gurl.h"

struct PP_Point;
struct _NPP;

class SkBitmap;
class TransportDIB;

namespace blink {
class WebInputEvent;
class WebLayer;
class WebMouseEvent;
class WebPluginContainer;
class WebURLLoader;
class WebURLResponse;
struct WebCompositionUnderline;
struct WebCursorInfo;
struct WebURLError;
struct WebPrintParams;
}

namespace cc {
class TextureLayer;
}

namespace gfx {
class Range;
}

namespace ppapi {
class Resource;
struct InputEventData;
struct PPP_Instance_Combined;
}

namespace v8 {
class Isolate;
}

namespace content {

class ContentDecryptorDelegate;
class FullscreenContainer;
class MessageChannel;
class PepperGraphics2DHost;
class PluginModule;
class PluginObject;
class PPB_Graphics3D_Impl;
class PPB_ImageData_Impl;
class PPB_URLLoader_Impl;
class RenderFrameImpl;
class RenderViewImpl;

class CONTENT_EXPORT PepperPluginInstanceImpl
    : public base::RefCounted<PepperPluginInstanceImpl>,
      public NON_EXPORTED_BASE(PepperPluginInstance),
      public ppapi::PPB_Instance_Shared,
      public NON_EXPORTED_BASE(cc::TextureLayerClient) {
 public:
  
  
  
  
  static PepperPluginInstanceImpl* Create(
      RenderFrameImpl* render_frame,
      PluginModule* module,
      blink::WebPluginContainer* container,
      const GURL& plugin_url);
  RenderFrameImpl* render_frame() const { return render_frame_; }
  PluginModule* module() const { return module_.get(); }
  MessageChannel& message_channel() { return *message_channel_; }

  blink::WebPluginContainer* container() const { return container_; }

  
  
  PP_Instance pp_instance() const { return pp_instance_; }

  ppapi::thunk::ResourceCreationAPI& resource_creation() {
    return *resource_creation_.get();
  }

  
  
  
  
  void Delete();

  
  bool is_deleted() const;

  
  void Paint(blink::WebCanvas* canvas,
             const gfx::Rect& plugin_rect,
             const gfx::Rect& paint_rect);

  
  
  
  
  void InvalidateRect(const gfx::Rect& rect);

  
  
  
  void ScrollRect(int dx, int dy, const gfx::Rect& rect);

  
  
  void CommitBackingTexture();

  
  void InstanceCrashed();

  
  bool full_frame() const { return full_frame_; }
  const ppapi::ViewData& view_data() const { return view_data_; }

  
  bool Initialize(const std::vector<std::string>& arg_names,
                  const std::vector<std::string>& arg_values,
                  bool full_frame);
  bool HandleDocumentLoad(const blink::WebURLResponse& response);
  bool HandleInputEvent(const blink::WebInputEvent& event,
                        blink::WebCursorInfo* cursor_info);
  PP_Var GetInstanceObject();
  void ViewChanged(const gfx::Rect& position, const gfx::Rect& clip,
                   const std::vector<gfx::Rect>& cut_outs_rects);

  
  bool HandleCompositionStart(const base::string16& text);
  bool HandleCompositionUpdate(
      const base::string16& text,
      const std::vector<blink::WebCompositionUnderline>& underlines,
      int selection_start,
      int selection_end);
  bool HandleCompositionEnd(const base::string16& text);
  bool HandleTextInput(const base::string16& text);

  
  ui::TextInputType text_input_type() const { return text_input_type_; }
  gfx::Rect GetCaretBounds() const;
  bool IsPluginAcceptingCompositionEvents() const;
  void GetSurroundingText(base::string16* text, gfx::Range* range) const;

  
  void SetWebKitFocus(bool has_focus);
  void SetContentAreaFocus(bool has_focus);

  
  void PageVisibilityChanged(bool is_visible);

  
  
  
  void ViewInitiatedPaint();
  void ViewFlushedPaint();

  
  
  
  
  bool GetBitmapForOptimizedPluginPaint(
      const gfx::Rect& paint_bounds,
      TransportDIB** dib,
      gfx::Rect* dib_bounds,
      gfx::Rect* clip,
      float* scale_factor);

  
  void AddPluginObject(PluginObject* plugin_object);
  void RemovePluginObject(PluginObject* plugin_object);

  base::string16 GetSelectedText(bool html);
  base::string16 GetLinkAtPosition(const gfx::Point& point);
  void RequestSurroundingText(size_t desired_number_of_characters);
  void Zoom(double factor, bool text_only);
  bool StartFind(const base::string16& search_text,
                 bool case_sensitive,
                 int identifier);
  void SelectFindResult(bool forward);
  void StopFind();

  bool SupportsPrintInterface();
  bool IsPrintScalingDisabled();
  int PrintBegin(const blink::WebPrintParams& print_params);
  bool PrintPage(int page_number, blink::WebCanvas* canvas);
  void PrintEnd();

  bool CanRotateView();
  void RotateView(blink::WebPlugin::RotationType type);

  
  
  
  

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool FlashIsFullscreenOrPending();

  
  
  void UpdateFlashFullscreenState(bool flash_fullscreen);

  FullscreenContainer* fullscreen_container() const {
    return fullscreen_container_;
  }

  

  
  
  
  
  
  
  
  
  
  bool IsFullscreenOrPending();

  bool flash_fullscreen() const { return flash_fullscreen_; }

  
  
  
  
  
  bool SetFullscreen(bool fullscreen);

  
  void HandleMessage(PP_Var message);

  
  bool IsProcessingUserGesture();

  
  
  blink::WebUserGestureToken CurrentUserGestureToken();

  
  void OnLockMouseACK(bool succeeded);
  
  void OnMouseLockLost();
  
  void HandleMouseLockedInputEvent(const blink::WebMouseEvent& event);

  
  
  void SimulateInputEvent(const ppapi::InputEventData& input_event);

  
  
  bool SimulateIMEEvent(const ppapi::InputEventData& input_event);
  void SimulateImeSetCompositionEvent(
      const ppapi::InputEventData& input_event);

  
  
  
  
  blink::WebURLLoaderClient* document_loader() const {
    return document_loader_;
  }
  void set_document_loader(blink::WebURLLoaderClient* loader) {
    document_loader_ = loader;
  }

  ContentDecryptorDelegate* GetContentDecryptorDelegate();

  
  virtual RenderView* GetRenderView() OVERRIDE;
  virtual blink::WebPluginContainer* GetContainer() OVERRIDE;
  virtual v8::Isolate* GetIsolate() const OVERRIDE;
  virtual ppapi::VarTracker* GetVarTracker() OVERRIDE;
  virtual const GURL& GetPluginURL() OVERRIDE;
  virtual base::FilePath GetModulePath() OVERRIDE;
  virtual PP_Resource CreateImage(gfx::ImageSkia* source_image,
                                  float scale) OVERRIDE;
  virtual PP_ExternalPluginResult SwitchToOutOfProcessProxy(
      const base::FilePath& file_path,
      ppapi::PpapiPermissions permissions,
      const IPC::ChannelHandle& channel_handle,
      base::ProcessId plugin_pid,
      int plugin_child_id) OVERRIDE;
  virtual void SetAlwaysOnTop(bool on_top) OVERRIDE;
  virtual bool IsFullPagePlugin() OVERRIDE;
  virtual bool FlashSetFullscreen(bool fullscreen, bool delay_report) OVERRIDE;
  virtual bool IsRectTopmost(const gfx::Rect& rect) OVERRIDE;
  virtual int32_t Navigate(const ppapi::URLRequestInfoData& request,
                           const char* target,
                           bool from_user_action) OVERRIDE;
  virtual int MakePendingFileRefRendererHost(
      const base::FilePath& path) OVERRIDE;
  virtual void SetEmbedProperty(PP_Var key, PP_Var value) OVERRIDE;

  
  virtual PP_Bool BindGraphics(PP_Instance instance,
                               PP_Resource device) OVERRIDE;
  virtual PP_Bool IsFullFrame(PP_Instance instance) OVERRIDE;
  virtual const ppapi::ViewData* GetViewData(PP_Instance instance) OVERRIDE;
  virtual PP_Bool FlashIsFullscreen(PP_Instance instance) OVERRIDE;
  virtual PP_Var GetWindowObject(PP_Instance instance) OVERRIDE;
  virtual PP_Var GetOwnerElementObject(PP_Instance instance) OVERRIDE;
  virtual PP_Var ExecuteScript(PP_Instance instance,
                               PP_Var script,
                               PP_Var* exception) OVERRIDE;
  virtual uint32_t GetAudioHardwareOutputSampleRate(PP_Instance instance)
      OVERRIDE;
  virtual uint32_t GetAudioHardwareOutputBufferSize(PP_Instance instance)
      OVERRIDE;
  virtual PP_Var GetDefaultCharSet(PP_Instance instance) OVERRIDE;
  virtual void NumberOfFindResultsChanged(PP_Instance instance,
                                          int32_t total,
                                          PP_Bool final_result) OVERRIDE;
  virtual void SelectedFindResultChanged(PP_Instance instance,
                                         int32_t index) OVERRIDE;
  virtual PP_Bool IsFullscreen(PP_Instance instance) OVERRIDE;
  virtual PP_Bool SetFullscreen(PP_Instance instance,
                                PP_Bool fullscreen) OVERRIDE;
  virtual PP_Bool GetScreenSize(PP_Instance instance, PP_Size* size)
      OVERRIDE;
  virtual ppapi::Resource* GetSingletonResource(PP_Instance instance,
      ppapi::SingletonResourceID id) OVERRIDE;
  virtual int32_t RequestInputEvents(PP_Instance instance,
                                     uint32_t event_classes) OVERRIDE;
  virtual int32_t RequestFilteringInputEvents(PP_Instance instance,
                                              uint32_t event_classes) OVERRIDE;
  virtual void ClearInputEventRequest(PP_Instance instance,
                                      uint32_t event_classes) OVERRIDE;
  virtual void ZoomChanged(PP_Instance instance, double factor) OVERRIDE;
  virtual void ZoomLimitsChanged(PP_Instance instance,
                                 double minimum_factor,
                                 double maximum_factor) OVERRIDE;
  virtual void PostMessage(PP_Instance instance, PP_Var message) OVERRIDE;
  virtual PP_Bool SetCursor(PP_Instance instance,
                            PP_MouseCursor_Type type,
                            PP_Resource image,
                            const PP_Point* hot_spot) OVERRIDE;
  virtual int32_t LockMouse(
      PP_Instance instance,
      scoped_refptr<ppapi::TrackedCallback> callback) OVERRIDE;
  virtual void UnlockMouse(PP_Instance instance) OVERRIDE;
  virtual void SetTextInputType(PP_Instance instance,
                                PP_TextInput_Type type) OVERRIDE;
  virtual void UpdateCaretPosition(PP_Instance instance,
                                   const PP_Rect& caret,
                                   const PP_Rect& bounding_box) OVERRIDE;
  virtual void CancelCompositionText(PP_Instance instance) OVERRIDE;
  virtual void SelectionChanged(PP_Instance instance) OVERRIDE;
  virtual void UpdateSurroundingText(PP_Instance instance,
                                     const char* text,
                                     uint32_t caret,
                                     uint32_t anchor) OVERRIDE;
  virtual PP_Var ResolveRelativeToDocument(
      PP_Instance instance,
      PP_Var relative,
      PP_URLComponents_Dev* components) OVERRIDE;
  virtual PP_Bool DocumentCanRequest(PP_Instance instance, PP_Var url) OVERRIDE;
  virtual PP_Bool DocumentCanAccessDocument(PP_Instance instance,
                                            PP_Instance target) OVERRIDE;
  virtual PP_Var GetDocumentURL(PP_Instance instance,
                                PP_URLComponents_Dev* components) OVERRIDE;
  virtual PP_Var GetPluginInstanceURL(
      PP_Instance instance,
      PP_URLComponents_Dev* components) OVERRIDE;
  virtual PP_Var GetPluginReferrerURL(
      PP_Instance instance,
      PP_URLComponents_Dev* components) OVERRIDE;

  
  virtual void SessionCreated(PP_Instance instance,
                              uint32_t session_id,
                              PP_Var web_session_id_var) OVERRIDE;
  virtual void SessionMessage(PP_Instance instance,
                              uint32_t session_id,
                              PP_Var message,
                              PP_Var destination_url) OVERRIDE;
  virtual void SessionReady(PP_Instance instance, uint32_t session_id) OVERRIDE;
  virtual void SessionClosed(PP_Instance instance,
                             uint32_t session_id) OVERRIDE;
  virtual void SessionError(PP_Instance instance,
                            uint32_t session_id,
                            int32_t media_error,
                            int32_t system_code) OVERRIDE;
  virtual void DeliverBlock(PP_Instance instance,
                            PP_Resource decrypted_block,
                            const PP_DecryptedBlockInfo* block_info) OVERRIDE;
  virtual void DecoderInitializeDone(PP_Instance instance,
                                     PP_DecryptorStreamType decoder_type,
                                     uint32_t request_id,
                                     PP_Bool success) OVERRIDE;
  virtual void DecoderDeinitializeDone(PP_Instance instance,
                                       PP_DecryptorStreamType decoder_type,
                                       uint32_t request_id) OVERRIDE;
  virtual void DecoderResetDone(PP_Instance instance,
                                PP_DecryptorStreamType decoder_type,
                                uint32_t request_id) OVERRIDE;
  virtual void DeliverFrame(PP_Instance instance,
                            PP_Resource decrypted_frame,
                            const PP_DecryptedFrameInfo* frame_info) OVERRIDE;
  virtual void DeliverSamples(
      PP_Instance instance,
      PP_Resource audio_frames,
      const PP_DecryptedSampleInfo* sample_info) OVERRIDE;

  
  
  
  
  
  PP_ExternalPluginResult ResetAsProxied(scoped_refptr<PluginModule> module);

  
  
  
  
  
  bool IsValidInstanceOf(PluginModule* module);

  
  
  struct _NPP* instanceNPP();

  
  virtual unsigned PrepareTexture() OVERRIDE;
  virtual bool PrepareTextureMailbox(
      cc::TextureMailbox* mailbox,
      scoped_ptr<cc::SingleReleaseCallback>* release_callback,
      bool use_shared_memory) OVERRIDE;

 private:
  friend class base::RefCounted<PepperPluginInstanceImpl>;
  friend class PpapiUnittest;

  
  virtual ~PepperPluginInstanceImpl();

  
  
  class ExternalDocumentLoader : public blink::WebURLLoaderClient {
   public:
    ExternalDocumentLoader();
    virtual ~ExternalDocumentLoader();

    void ReplayReceivedData(WebURLLoaderClient* document_loader);

    
    virtual void didReceiveData(blink::WebURLLoader* loader,
                                const char* data,
                                int data_length,
                                int encoded_data_length);
    virtual void didFinishLoading(blink::WebURLLoader* loader,
                                  double finish_time);
    virtual void didFail(blink::WebURLLoader* loader,
                         const blink::WebURLError& error);

   private:
    std::list<std::string> data_;
    bool finished_loading_;
    scoped_ptr<blink::WebURLError> error_;
  };

  
  
  class GamepadImpl : public ppapi::thunk::PPB_Gamepad_API,
                      public ppapi::Resource {
   public:
    GamepadImpl();
    
    virtual ppapi::thunk::PPB_Gamepad_API* AsPPB_Gamepad_API() OVERRIDE;
    virtual void Sample(PP_Instance instance,
                        PP_GamepadsSampleData* data) OVERRIDE;
   private:
    virtual ~GamepadImpl();
  };

  
  
  
  
  PepperPluginInstanceImpl(RenderFrameImpl* render_frame,
                           PluginModule* module,
                           ppapi::PPP_Instance_Combined* instance_interface,
                           blink::WebPluginContainer* container,
                           const GURL& plugin_url);

  bool LoadFindInterface();
  bool LoadInputEventInterface();
  bool LoadMessagingInterface();
  bool LoadMouseLockInterface();
  bool LoadPdfInterface();
  bool LoadPrintInterface();
  bool LoadPrivateInterface();
  bool LoadSelectionInterface();
  bool LoadTextInputInterface();
  bool LoadZoomInterface();

  
  
  bool PluginHasFocus() const;
  void SendFocusChangeNotification();

  void UpdateTouchEventRequest();

  
  bool IsAcceptingWheelEvents() const;

  void ScheduleAsyncDidChangeView();
  void SendAsyncDidChangeView();
  void SendDidChangeView();

  
  
  void ReportGeometry();

  
  
  
  bool GetPreferredPrintOutputFormat(PP_PrintOutputFormat_Dev* format);
  bool PrintPDFOutput(PP_Resource print_output, blink::WebCanvas* canvas);

  
  
  
  
  
  
  
  void UpdateLayer();

  
  bool PrintPageHelper(PP_PrintPageNumberRange_Dev* page_ranges,
                       int num_ranges,
                       blink::WebCanvas* canvas);

  void DoSetCursor(blink::WebCursorInfo* cursor);

  
  bool SendCompositionEventToPlugin(
      PP_InputEvent_Type type,
      const base::string16& text);
  bool SendCompositionEventWithUnderlineInformationToPlugin(
      PP_InputEvent_Type type,
      const base::string16& text,
      const std::vector<blink::WebCompositionUnderline>& underlines,
      int selection_start,
      int selection_end);

  
  void RequestInputEventsHelper(uint32_t event_classes);

  
  
  bool CanAccessMainFrame() const;

  
  
  bool IsViewAccelerated();

  
  
  void KeepSizeAttributesBeforeFullscreen();
  void SetSizeAttributesForFullscreen();
  void ResetSizeAttributesAfterFullscreen();

  bool IsMouseLocked();
  bool LockMouse();
  MouseLockDispatcher* GetMouseLockDispatcher();
  MouseLockDispatcher::LockTarget* GetOrCreateLockTargetAdapter();
  void UnSetAndDeleteLockTargetAdapter();

  void DidDataFromWebURLResponse(
      const blink::WebURLResponse& response,
      int pending_host_id,
      const ppapi::URLResponseInfoData& data);

  RenderFrameImpl* render_frame_;
  scoped_refptr<PluginModule> module_;
  scoped_ptr<ppapi::PPP_Instance_Combined> instance_interface_;
  
  
  
  scoped_refptr<PluginModule> original_module_;
  scoped_ptr<ppapi::PPP_Instance_Combined> original_instance_interface_;

  PP_Instance pp_instance_;

  
  blink::WebPluginContainer* container_;
  scoped_refptr<cc::TextureLayer> texture_layer_;
  scoped_ptr<blink::WebLayer> web_layer_;
  bool layer_bound_to_fullscreen_;
  bool layer_is_hardware_;

  
  GURL plugin_url_;

  
  
  bool full_frame_;

  
  ppapi::ViewData view_data_;
  
  
  ppapi::ViewData last_sent_view_data_;

  
  
  
  bool sent_initial_did_change_view_;

  
  scoped_refptr<PPB_Graphics3D_Impl> bound_graphics_3d_;
  PepperGraphics2DHost* bound_graphics_2d_platform_;

  
  
  
  
  bool has_webkit_focus_;
  bool has_content_area_focus_;

  
  int find_identifier_;

  
  scoped_ptr<ppapi::thunk::ResourceCreationAPI> resource_creation_;

  
  
  const PPP_Find_Dev* plugin_find_interface_;
  const PPP_InputEvent* plugin_input_event_interface_;
  const PPP_Messaging* plugin_messaging_interface_;
  const PPP_MouseLock* plugin_mouse_lock_interface_;
  const PPP_Pdf* plugin_pdf_interface_;
  const PPP_Instance_Private* plugin_private_interface_;
  const PPP_Selection_Dev* plugin_selection_interface_;
  const PPP_TextInput_Dev* plugin_textinput_interface_;
  const PPP_Zoom_Dev* plugin_zoom_interface_;

  
  
  
  bool checked_for_plugin_input_event_interface_;
  bool checked_for_plugin_messaging_interface_;
  bool checked_for_plugin_pdf_interface_;

  
  
  PP_PrintSettings_Dev current_print_settings_;
#if defined(OS_MACOSX)
  
  
  
  scoped_refptr<PPB_ImageData_Impl> last_printed_page_;
#endif  
  
  
  
  
  
  
  
  
  
  skia::RefPtr<blink::WebCanvas> canvas_;
  
  std::vector<PP_PrintPageNumberRange_Dev> ranges_;

  scoped_refptr<ppapi::Resource> gamepad_impl_;

  
  const PPP_Printing_Dev* plugin_print_interface_;

  
  const PPP_Graphics3D* plugin_graphics_3d_interface_;

  
  scoped_ptr<blink::WebCursorInfo> cursor_;

  
  
  bool always_on_top_;
  
  
  
  std::vector<gfx::Rect> cut_outs_rects_;

  

  
  
  
  FullscreenContainer* fullscreen_container_;

  
  
  
  bool flash_fullscreen_;

  

  
  
  
  
  bool desired_fullscreen_state_;

  
  
  
  blink::WebString width_before_fullscreen_;
  blink::WebString height_before_fullscreen_;
  blink::WebString border_before_fullscreen_;
  blink::WebString style_before_fullscreen_;
  gfx::Size screen_size_for_fullscreen_;

  
  
  scoped_ptr<MessageChannel> message_channel_;

  
  SkBitmap* sad_plugin_;

  typedef std::set<PluginObject*> PluginObjectSet;
  PluginObjectSet live_plugin_objects_;

  
  
  uint32_t input_event_mask_;
  uint32_t filtered_input_event_mask_;

  
  ui::TextInputType text_input_type_;
  gfx::Rect text_input_caret_;
  gfx::Rect text_input_caret_bounds_;
  bool text_input_caret_set_;

  
  std::string surrounding_text_;
  size_t selection_caret_;
  size_t selection_anchor_;

  scoped_refptr<ppapi::TrackedCallback> lock_mouse_callback_;

  
  
  PP_TimeTicks pending_user_gesture_;
  blink::WebUserGestureToken pending_user_gesture_token_;

  
  
  std::vector<std::string> argn_;
  std::vector<std::string> argv_;

  
  blink::WebURLLoaderClient* document_loader_;
  
  blink::WebURLResponse external_document_response_;
  scoped_ptr<ExternalDocumentLoader> external_document_loader_;
  bool external_document_load_;

  
  
  scoped_ptr<ContentDecryptorDelegate> content_decryptor_delegate_;

  
  
  scoped_ptr<struct _NPP> npp_;

  
  
  v8::Isolate* isolate_;

  scoped_ptr<MouseLockDispatcher::LockTarget> lock_target_;

  bool is_deleted_;

  
  
  
  
  
  base::WeakPtrFactory<PepperPluginInstanceImpl> view_change_weak_ptr_factory_;
  base::WeakPtrFactory<PepperPluginInstanceImpl> weak_factory_;

  friend class PpapiPluginInstanceTest;
  DISALLOW_COPY_AND_ASSIGN(PepperPluginInstanceImpl);
};

}  

#endif  
