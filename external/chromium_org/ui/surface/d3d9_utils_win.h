// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_SURFACE_D3D9_UTILS_WIN_H_
#define UI_SURFACE_D3D9_UTILS_WIN_H_

#include <d3d9.h>

#include "base/basictypes.h"
#include "base/win/scoped_comptr.h"
#include "ui/surface/surface_export.h"

namespace base {
class ScopedNativeLibrary;
}

namespace gfx {
class Size;
}

namespace ui_surface_d3d9_utils {

SURFACE_EXPORT
bool LoadD3D9(base::ScopedNativeLibrary* storage);

SURFACE_EXPORT
bool CreateDevice(const base::ScopedNativeLibrary& d3d_module,
                  uint64 adapter_luid,
                  D3DDEVTYPE device_type,
                  uint32 presentation_interval,
                  IDirect3DDevice9Ex** device);

// identified by its surface handle. The resulting texture is written into
SURFACE_EXPORT
bool OpenSharedTexture(IDirect3DDevice9* device,
                       int64 surface_handle,
                       const gfx::Size& size,
                       IDirect3DTexture9** opened_texture);

SURFACE_EXPORT
bool CreateOrReuseLockableSurface(
    IDirect3DDevice9* device,
    const gfx::Size& size,
    base::win::ScopedComPtr<IDirect3DSurface9>* surface);

SURFACE_EXPORT
bool CreateOrReuseRenderTargetTexture(
    IDirect3DDevice9* device,
    const gfx::Size& size,
    base::win::ScopedComPtr<IDirect3DTexture9>* texture,
    IDirect3DSurface9** render_target);

SURFACE_EXPORT
gfx::Size GetSize(IDirect3DTexture9* texture);

SURFACE_EXPORT
gfx::Size GetSize(IDirect3DSurface9* surface);

}  

#endif  
