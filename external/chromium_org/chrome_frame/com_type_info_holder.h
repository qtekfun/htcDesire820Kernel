// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_COM_TYPE_INFO_HOLDER_H_
#define CHROME_FRAME_COM_TYPE_INFO_HOLDER_H_

#include <map>
#include <ocidl.h>  

#include "base/synchronization/lock.h"
#include "base/win/scoped_comptr.h"

#define NO_VTABLE __declspec(novtable)

namespace com_util {

class NameToDispIdCache {
 public:
  typedef uint32 HashType;

  bool Lookup(HashType hash, DISPID* dispid) const;
  void Add(HashType hash, DISPID dispid);

  
  
  
  static HashType Hash(const wchar_t* name);

 protected:
  typedef std::map<HashType, DISPID> DispidMap;
  DispidMap map_;
  mutable base::Lock lock_;
};

class TypeInfoNameCache {
 public:
  
  
  HRESULT Initialize(const IID& iid);

  
  
  HRESULT GetIDsOfNames(OLECHAR** names, uint32 count, DISPID* dispids);

  
  HRESULT Invoke(IDispatch* p, DISPID dispid, WORD flags, DISPPARAMS* params,
                 VARIANT* result, EXCEPINFO* excepinfo, UINT* arg_err);

  inline ITypeInfo* CopyTypeInfo() {
    ITypeInfo* ti = type_info_.get();
    if (ti)
      ti->AddRef();
    return ti;
  }

 protected:
  base::win::ScopedComPtr<ITypeInfo> type_info_;
  NameToDispIdCache cache_;
};

class TypeInfoCache {
 public:
  TypeInfoCache() {
  }

  ~TypeInfoCache();

  
  
  TypeInfoNameCache* Lookup(const IID* iid);

  
  static TypeInfoCache* Singleton();

 protected:
  typedef std::map<const IID*, TypeInfoNameCache*> CacheMap;
  base::Lock lock_;
  CacheMap cache_;
};

template <const IID& iid>
class TypeInfoHolder {
 public:
  TypeInfoHolder() : type_info_(NULL) {
  }

  bool EnsureTI() {
    if (!type_info_)
      type_info_ = TypeInfoCache::Singleton()->Lookup(&iid);
    return type_info_ != NULL;
  }

  HRESULT GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** info) {
    if (EnsureTI()) {
      *info = type_info_->CopyTypeInfo();
      return S_OK;
    }

    return E_UNEXPECTED;
  }

  HRESULT GetIDsOfNames(REFIID riid, OLECHAR** names, UINT count, LCID lcid,
                        DISPID* dispids) {
    if (!EnsureTI())
      return E_UNEXPECTED;
    return type_info_->GetIDsOfNames(names, count, dispids);
  }

  HRESULT Invoke(IDispatch* p, DISPID dispid, REFIID riid, LCID lcid,
                 WORD flags, DISPPARAMS* params, VARIANT* result,
                 EXCEPINFO* excepinfo, UINT* arg_err) {
    if (!EnsureTI())
      return E_UNEXPECTED;

    return type_info_->Invoke(p, dispid, flags, params, result, excepinfo,
                              arg_err);
  }

 protected:
  TypeInfoNameCache* type_info_;
};

template <class T, const IID& iid = __uuidof(T)>
class NO_VTABLE IDispatchImpl : public T {
 public:
  STDMETHOD(GetTypeInfoCount)(UINT* count) {
    if (count == NULL)
      return E_POINTER;
    *count = 1;
    return S_OK;
  }

  STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo** pptinfo) {
    return type_info_.GetTypeInfo(itinfo, lcid, pptinfo);
  }

  STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* names, UINT count,
                           LCID lcid, DISPID* dispids) {
    return type_info_.GetIDsOfNames(riid, names, count, lcid, dispids);
  }
  STDMETHOD(Invoke)(DISPID dispid, REFIID riid, LCID lcid, WORD flags,
                    DISPPARAMS* params, VARIANT* result, EXCEPINFO* excepinfo,
                    UINT* arg_err) {
    return type_info_.Invoke(static_cast<IDispatch*>(this), dispid, riid, lcid,
                             flags, params, result, excepinfo, arg_err);
  }

 protected:
  TypeInfoHolder<iid> type_info_;
};

template <const CLSID& class_id, const IID& source_iid>
class NO_VTABLE IProvideClassInfo2Impl : public IProvideClassInfo2 {
 public:
  STDMETHOD(GetClassInfo)(ITypeInfo** pptinfo) {
    return type_info_.GetTypeInfo(0, LANG_NEUTRAL, pptinfo);
  }

  STDMETHOD(GetGUID)(DWORD guid_kind, GUID* guid) {
    if (guid == NULL || guid_kind != GUIDKIND_DEFAULT_SOURCE_DISP_IID)
      return E_INVALIDARG;

    *guid = source_iid;

    return S_OK;
  }

 protected:
  TypeInfoHolder<class_id> type_info_;
};

}  

#endif  
