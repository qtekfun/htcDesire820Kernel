// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SURFACE_ACCELERATED_SURFACE_WIN_H_
#define UI_SURFACE_ACCELERATED_SURFACE_WIN_H_

#include <d3d9.h>

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/single_thread_task_runner.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "base/time/time.h"
#include "base/win/scoped_comptr.h"
#include "ui/events/latency_info.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/surface/surface_export.h"

class PresentThread;

namespace gfx {
class Rect;
}

namespace media {
class VideoFrame;
}

class SURFACE_EXPORT AcceleratedPresenter
    : public base::RefCountedThreadSafe<AcceleratedPresenter> {
 public:
  typedef base::Callback<void(bool,
                              base::TimeTicks,
                              base::TimeDelta,
                              const ui::LatencyInfo&)> CompletionTask;

  explicit AcceleratedPresenter(gfx::PluginWindowHandle window);

  static void SetAdapterLUID(uint64 adapter_luid);

  
  
  
  static scoped_refptr<AcceleratedPresenter> GetForWindow(
      gfx::PluginWindowHandle window);

  
  
  
  
  void AsyncPresentAndAcknowledge(
      const gfx::Size& size,
      int64 surface_handle,
      const ui::LatencyInfo& latency_info,
      const CompletionTask& completion_task);

  
  
  bool IsSwapChainInitialized() const;

  
  
  void Suspend();

  
  void WasHidden();

  
  void SetIsSessionLocked(bool locked);

  
  void ReleaseSurface();

  
  void Present(HDC dc);
  void AsyncCopyTo(const gfx::Rect& src_subrect,
                   const gfx::Size& dst_size,
                   const base::Callback<void(bool, const SkBitmap&)>& callback);
  void AsyncCopyToVideoFrame(
      const gfx::Rect& src_subrect,
      const scoped_refptr<media::VideoFrame>& target,
      const base::Callback<void(bool)>& callback);
  void Invalidate();

  
  
  void ResetPresentThread(PresentThread* present_thread);

 private:
  friend class base::RefCountedThreadSafe<AcceleratedPresenter>;

  ~AcceleratedPresenter();

  
  
  void DoPresentAndAcknowledge(
      const gfx::Size& size,
      int64 surface_handle,
      const ui::LatencyInfo& latency_info,
      const CompletionTask& completion_task);
  void DoSuspend();
  void DoPresent(const base::Closure& composite_task);
  void DoReleaseSurface();
  void DoCopyToAndAcknowledge(
      const gfx::Rect& src_subrect,
      const gfx::Size& dst_size,
      scoped_refptr<base::SingleThreadTaskRunner> callback_runner,
      const base::Callback<void(bool, const SkBitmap&)>& callback);
  void DoCopyToVideoFrameAndAcknowledge(
      const gfx::Rect& src_subrect,
      const scoped_refptr<media::VideoFrame>& target,
      const scoped_refptr<base::SingleThreadTaskRunner>& callback_runner,
      const base::Callback<void(bool)>& callback);
  bool DoCopyToYUV(const gfx::Rect& src_subrect,
                   const scoped_refptr<media::VideoFrame>& frame);
  bool DoCopyToARGB(const gfx::Rect& src_subrect,
                    const gfx::Size& dst_size,
                    SkBitmap* bitmap);

  void PresentWithGDI(HDC dc);
  gfx::Size GetWindowSize();

  
  
  
  
  bool CheckDirect3DWillWork();

  
  PresentThread* const present_thread_;

  
  gfx::PluginWindowHandle window_;

  
  base::WaitableEvent event_;

  
  
  
  gfx::Size quantized_size_;

  
  
  
  gfx::Size present_size_;

  
  base::win::ScopedComPtr<IDirect3DTexture9> source_texture_;

  
  
  base::win::ScopedComPtr<IDirect3DSwapChain9> swap_chain_;

  
  
  bool hidden_;

  
  
  bool do_present_with_GDI_;

  
  bool is_session_locked_;

  
  
  
  
  gfx::Size last_window_size_;
  base::Time last_window_resize_time_;

  ui::LatencyInfo latency_info_;

  DISALLOW_COPY_AND_ASSIGN(AcceleratedPresenter);
};

class SURFACE_EXPORT AcceleratedSurface {
 public:
  AcceleratedSurface(gfx::PluginWindowHandle window);
  ~AcceleratedSurface();

  
  void Present(HDC dc);

  
  
  bool IsReadyForCopy() const;

  
  
  void AsyncCopyTo(const gfx::Rect& src_subrect,
                   const gfx::Size& dst_size,
                   const base::Callback<void(bool, const SkBitmap&)>& callback);

  
  
  void AsyncCopyToVideoFrame(
      const gfx::Rect& src_subrect,
      const scoped_refptr<media::VideoFrame>& target,
      const base::Callback<void(bool)>& callback);

  
  
  
  void Suspend();

  
  void WasHidden();

  
  void SetIsSessionLocked(bool locked);

 private:
  const scoped_refptr<AcceleratedPresenter> presenter_;
  DISALLOW_COPY_AND_ASSIGN(AcceleratedSurface);
};

#endif  
