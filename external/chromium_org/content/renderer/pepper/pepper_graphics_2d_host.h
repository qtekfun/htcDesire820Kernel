// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_GRAPHICS_2D_HOST_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_GRAPHICS_2D_HOST_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "ppapi/c/dev/ppb_graphics_2d_dev.h"
#include "ppapi/c/ppb_graphics_2d.h"
#include "ppapi/host/host_message_context.h"
#include "ppapi/host/resource_host.h"
#include "third_party/WebKit/public/platform/WebCanvas.h"
#include "ui/gfx/point.h"
#include "ui/gfx/size.h"

namespace cc {
class SingleReleaseCallback;
class TextureMailbox;
}

namespace gfx {
class Rect;
}

namespace ppapi {
struct ViewData;
}

namespace content {
  
class PepperPluginInstanceImpl;
class PPB_ImageData_Impl;
class RendererPpapiHost;

class CONTENT_EXPORT PepperGraphics2DHost
    : public ppapi::host::ResourceHost,
      public base::SupportsWeakPtr<PepperGraphics2DHost> {
 public:
  static PepperGraphics2DHost* Create(
      RendererPpapiHost* host,
      PP_Instance instance,
      PP_Resource resource,
      const PP_Size& size,
      PP_Bool is_always_opaque,
      scoped_refptr<PPB_ImageData_Impl> backing_store);

  virtual ~PepperGraphics2DHost();

  
  virtual int32_t OnResourceMessageReceived(
      const IPC::Message& msg,
      ppapi::host::HostMessageContext* context) OVERRIDE;
  virtual bool IsGraphics2DHost() OVERRIDE;

  bool ReadImageData(PP_Resource image,
                     const PP_Point* top_left);
  
  
  
  
  bool BindToInstance(PepperPluginInstanceImpl* new_instance);
  
  void Paint(blink::WebCanvas* canvas,
             const gfx::Rect& plugin_rect,
             const gfx::Rect& paint_rect);

  bool PrepareTextureMailbox(
      cc::TextureMailbox* mailbox,
      scoped_ptr<cc::SingleReleaseCallback>* release_callback);
  void AttachedToNewLayer();

  
  
  void ViewInitiatedPaint();
  void ViewFlushedPaint();

  void DidChangeView(const ppapi::ViewData& view_data);

  void SetScale(float scale);
  float GetScale() const;
  bool IsAlwaysOpaque() const;
  PPB_ImageData_Impl* ImageData();
  gfx::Size Size() const;

  
  
  gfx::PointF GetResizeScale() const;
  
  gfx::Point plugin_offset() const { return plugin_offset_; }

 private:
  PepperGraphics2DHost(RendererPpapiHost* host,
                       PP_Instance instance,
                       PP_Resource resource);

  bool Init(int width,
            int height,
            bool is_always_opaque,
            scoped_refptr<PPB_ImageData_Impl> backing_store);

  int32_t OnHostMsgPaintImageData(ppapi::host::HostMessageContext* context,
                                  const ppapi::HostResource& image_data,
                                  const PP_Point& top_left,
                                  bool src_rect_specified,
                                  const PP_Rect& src_rect);
  int32_t OnHostMsgScroll(ppapi::host::HostMessageContext* context,
                          bool clip_specified,
                          const PP_Rect& clip,
                          const PP_Point& amount);
  int32_t OnHostMsgReplaceContents(ppapi::host::HostMessageContext* context,
                                   const ppapi::HostResource& image_data);
  int32_t OnHostMsgFlush(ppapi::host::HostMessageContext* context,
                         const ppapi::ViewData& view_data);
  int32_t OnHostMsgSetScale(ppapi::host::HostMessageContext* context,
                            float scale);
  int32_t OnHostMsgSetOffset(ppapi::host::HostMessageContext* context,
                             const PP_Point& offset);
  int32_t OnHostMsgSetResizeMode(ppapi::host::HostMessageContext* context,
                                 PP_Graphics2D_Dev_ResizeMode resize_mode);
  int32_t OnHostMsgReadImageData(ppapi::host::HostMessageContext* context,
                                 PP_Resource image,
                                 const PP_Point& top_left);

  
  
  int32_t Flush(PP_Resource* old_image_data,
                const gfx::Size& flushed_plugin_size);

  
  
  
  
  
  void ExecutePaintImageData(PPB_ImageData_Impl* image,
                             int x, int y,
                             const gfx::Rect& src_rect,
                             gfx::Rect* invalidated_rect);
  void ExecuteScroll(const gfx::Rect& clip, int dx, int dy,
                     gfx::Rect* invalidated_rect);
  void ExecuteReplaceContents(PPB_ImageData_Impl* image,
                              gfx::Rect* invalidated_rect,
                              PP_Resource* old_image_data);

  void SendFlushAck();

  
  
  void SendOffscreenFlushAck();

  
  void ScheduleOffscreenFlushAck();

  
  bool HasPendingFlush() const;

  
  
  
  
  
  static bool ConvertToLogicalPixels(float scale,
                                     gfx::Rect* op_rect,
                                     gfx::Point* delta);

  RendererPpapiHost* renderer_ppapi_host_;

  scoped_refptr<PPB_ImageData_Impl> image_data_;

  
  
  PepperPluginInstanceImpl* bound_instance_;

  
  struct QueuedOperation;
  typedef std::vector<QueuedOperation> OperationQueue;
  OperationQueue queued_operations_;

  
  bool need_flush_ack_;

  
  
  
  bool offscreen_flush_pending_;

  
  
  bool is_always_opaque_;

  
  
  float scale_;

  ppapi::host::ReplyMessageContext flush_reply_context_;

  bool is_running_in_process_;

  bool texture_mailbox_modified_;
  bool is_using_texture_layer_;

  
  
  gfx::Point plugin_offset_;

  
  
  
  
  
  gfx::Size flushed_plugin_size_;
  
  gfx::Size current_plugin_size_;

  PP_Graphics2D_Dev_ResizeMode resize_mode_;

  friend class PepperGraphics2DHostTest;
  DISALLOW_COPY_AND_ASSIGN(PepperGraphics2DHost);
};

}  

#endif  
