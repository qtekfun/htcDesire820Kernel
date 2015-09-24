// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_VIDEO_CAPTURE_WIN_FILTER_BASE_WIN_H_
#define MEDIA_VIDEO_CAPTURE_WIN_FILTER_BASE_WIN_H_

#define NO_DSHOW_STRSAFE
#include <dshow.h>

#include "base/memory/ref_counted.h"
#include "base/win/scoped_comptr.h"

namespace media {

class FilterBase
    : public IBaseFilter,
      public base::RefCounted<FilterBase> {
 public:
  FilterBase();
  virtual ~FilterBase();

  
  virtual size_t NoOfPins() = 0;
  
  virtual IPin* GetPin(int index) = 0;

  
  STDMETHOD(QueryInterface)(REFIID id, void** object_ptr);
  STDMETHOD_(ULONG, AddRef)();
  STDMETHOD_(ULONG, Release)();

  
  STDMETHOD(EnumPins)(IEnumPins** enum_pins);

  STDMETHOD(FindPin)(LPCWSTR id, IPin** pin);

  STDMETHOD(QueryFilterInfo)(FILTER_INFO* info);

  STDMETHOD(JoinFilterGraph)(IFilterGraph* graph, LPCWSTR name);

  STDMETHOD(QueryVendorInfo)(LPWSTR* vendor_info);

  
  STDMETHOD(Stop)();

  STDMETHOD(Pause)();

  STDMETHOD(Run)(REFERENCE_TIME start);

  STDMETHOD(GetState)(DWORD msec_timeout, FILTER_STATE* state);

  STDMETHOD(SetSyncSource)(IReferenceClock* clock);

  STDMETHOD(GetSyncSource)(IReferenceClock** clock);

  
  STDMETHOD(GetClassID)(CLSID* class_id) = 0;

 private:
  FILTER_STATE state_;
  base::win::ScopedComPtr<IFilterGraph> owning_graph_;

  DISALLOW_COPY_AND_ASSIGN(FilterBase);
};

}  

#endif  
