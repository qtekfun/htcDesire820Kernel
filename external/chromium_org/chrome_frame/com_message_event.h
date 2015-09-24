// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_COM_MESSAGE_EVENT_H_
#define CHROME_FRAME_COM_MESSAGE_EVENT_H_

#include <atlbase.h>
#include <atlcom.h>
#include <mshtml.h>  

#include "base/basictypes.h"
#include "base/win/scoped_comptr.h"

class ComMessageEvent
    : public CComObjectRootEx<CComSingleThreadModel>,
      public IDispatch {
 public:
  ComMessageEvent();
  ~ComMessageEvent();

BEGIN_COM_MAP(ComMessageEvent)
  COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

  
  
  
  
  typedef enum {
    DISPID_MESSAGE_EVENT_DATA = 201,
    DISPID_MESSAGE_EVENT_ORIGIN,
    DISPID_MESSAGE_EVENT_LAST_EVENT_ID,
    DISPID_MESSAGE_EVENT_SOURCE,
    DISPID_MESSAGE_EVENT_MESSAGE_PORT,
    DISPID_MESSAGE_EVENT_TYPE
  } MessageEventDispIds;

  
  
  HRESULT GetStringProperty(WORD flags, const wchar_t* value, VARIANT* result);

  STDMETHOD(GetTypeInfoCount)(UINT* info);
  STDMETHOD(GetTypeInfo)(UINT which_info, LCID lcid, ITypeInfo** type_info);
  STDMETHOD(GetIDsOfNames)(REFIID iid, LPOLESTR* names, UINT count_names,
                           LCID lcid, DISPID* dispids);
  STDMETHOD(Invoke)(DISPID dispid, REFIID iid, LCID lcid, WORD flags,
                    DISPPARAMS* params, VARIANT* result, EXCEPINFO* excepinfo,
                    UINT* arg_err);

  
  
  bool Initialize(IOleContainer* container, const std::string& message,
                  const std::string& origin, const std::string& event_type);

 protected:
  
  
  
  
  base::win::ScopedComPtr<IHTMLEventObj> basic_event_;
  std::string message_;
  std::string origin_;
  std::string type_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ComMessageEvent);
};

#endif  
