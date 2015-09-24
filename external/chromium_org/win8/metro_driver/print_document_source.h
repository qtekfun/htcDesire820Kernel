// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_METRO_DRIVER_PRINT_DOCUMENT_SOURCE_H_
#define CHROME_BROWSER_UI_METRO_DRIVER_PRINT_DOCUMENT_SOURCE_H_

#include <documentsource.h>
#include <printpreview.h>
#include <windows.graphics.printing.h>

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/waitable_event.h"
#include "win8/metro_driver/winrt_utils.h"

#ifndef __IPrintPreviewDxgiPackageTarget_FWD_DEFINED__
typedef IPrintPreviewDXGIPackageTarget IPrintPreviewDxgiPackageTarget;
#endif


namespace base {
class Lock;
};  

namespace metro_driver {

class PrintDocumentSource
    : public mswr::RuntimeClass<
          mswr::RuntimeClassFlags<mswr::WinRtClassicComMix>,
          wingfx::Printing::IPrintDocumentSource,
          IPrintDocumentPageSource,
          IPrintPreviewPageCollection> {
 public:
  
  
  struct DirectXContext {
    DirectXContext() {}
    DirectXContext(ID3D11Device1* device_3d,
                   ID2D1Factory1* factory_2d,
                   ID2D1Device* device_2d,
                   ID2D1DeviceContext* context_2d,
                   IWICImagingFactory2* factory_wic)
        : d3d_device(device_3d),
          d2d_factory(factory_2d),
          d2d_device(device_2d),
          d2d_context(context_2d),
          wic_factory(factory_wic) {
    }
    DirectXContext(const DirectXContext& other)
        : d3d_device(other.d3d_device),
          d2d_factory(other.d2d_factory),
          d2d_device(other.d2d_device),
          d2d_context(other.d2d_context),
          wic_factory(other.wic_factory) {
    }
    mswr::ComPtr<ID3D11Device1> d3d_device;
    mswr::ComPtr<ID2D1Factory1> d2d_factory;
    mswr::ComPtr<ID2D1Device> d2d_device;
    mswr::ComPtr<ID2D1DeviceContext> d2d_context;
    mswr::ComPtr<IWICImagingFactory2> wic_factory;
  };

  
  explicit PrintDocumentSource();
  HRESULT RuntimeClassInitialize(const DirectXContext& directx_context,
                                 base::Lock* parent_lock);
  
  void Abort();

  
  STDMETHOD(GetPreviewPageCollection) (
      IPrintDocumentPackageTarget* package_target,
      IPrintPreviewPageCollection** page_collection);
  STDMETHOD(MakeDocument)(IInspectable* options,
                          IPrintDocumentPackageTarget* package_target);

  
  STDMETHOD(Paginate)(uint32 page, IInspectable* options);
  STDMETHOD(MakePage)(uint32 desired_page, float width, float height);

  
  void ResetDpi(float dpi);

  
  void SetPageCount(size_t page_count);

  
  
  void AddPage(size_t page_number, IStream* metafile_stream);

 private:
  
  HRESULT PrintPage(ID2D1PrintControl* print_control,
                    ID2D1GdiMetafile* metafile_stream,
                    D2D1_SIZE_F pageSize);

  
  
  size_t WaitAndGetPageCount();

  
  
  HRESULT WaitAndGetPage(size_t page_number,
                         ID2D1GdiMetafile** metafile_stream);

  DirectXContext directx_context_;

  
  std::vector<mswr::ComPtr<IStream>> pages_;

  
  
  
  
  
  std::vector<scoped_ptr<base::ConditionVariable> > pages_ready_state_;

  
  
  
  
  base::WaitableEvent page_count_ready_;

  
  
  mswr::ComPtr<IPrintPreviewDxgiPackageTarget> dxgi_preview_target_;

  
  
  base::Lock* parent_lock_;

  
  
  
  
  float height_;
  float width_;

  
  float dpi_;

  
  
  bool aborted_;

  
  bool using_old_preview_interface_;
};

}  

#endif  
