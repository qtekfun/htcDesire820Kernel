// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_METRO_DRIVER_IME_TEXT_STORE_H_
#define WIN8_METRO_DRIVER_IME_TEXT_STORE_H_

#include <atlbase.h>
#include <atlcom.h>
#include <initguid.h>
#include <inputscope.h>
#include <msctf.h>

#include <deque>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "base/win/scoped_comptr.h"
#include "ui/metro_viewer/ime_types.h"

namespace metro_driver {

class TextStoreDelegate;

class ATL_NO_VTABLE TextStore
    : public CComObjectRootEx<CComMultiThreadModel>,
      public ITextStoreACP,
      public ITfContextOwnerCompositionSink,
      public ITfTextEditSink {
 public:
  virtual ~TextStore();

  BEGIN_COM_MAP(TextStore)
    COM_INTERFACE_ENTRY(ITextStoreACP)
    COM_INTERFACE_ENTRY(ITfContextOwnerCompositionSink)
    COM_INTERFACE_ENTRY(ITfTextEditSink)
  END_COM_MAP()

  
  STDMETHOD(AdviseSink)(REFIID iid, IUnknown* unknown, DWORD mask) OVERRIDE;
  STDMETHOD(FindNextAttrTransition)(LONG acp_start,
                                    LONG acp_halt,
                                    ULONG num_filter_attributes,
                                    const TS_ATTRID* filter_attributes,
                                    DWORD flags,
                                    LONG* acp_next,
                                    BOOL* found,
                                    LONG* found_offset) OVERRIDE;
  STDMETHOD(GetACPFromPoint)(TsViewCookie view_cookie,
                             const POINT* point,
                             DWORD flags,
                             LONG* acp) OVERRIDE;
  STDMETHOD(GetActiveView)(TsViewCookie* view_cookie) OVERRIDE;
  STDMETHOD(GetEmbedded)(LONG acp_pos,
                         REFGUID service,
                         REFIID iid,
                         IUnknown** unknown) OVERRIDE;
  STDMETHOD(GetEndACP)(LONG* acp) OVERRIDE;
  STDMETHOD(GetFormattedText)(LONG acp_start,
                              LONG acp_end,
                              IDataObject** data_object) OVERRIDE;
  STDMETHOD(GetScreenExt)(TsViewCookie view_cookie, RECT* rect) OVERRIDE;
  STDMETHOD(GetSelection)(ULONG selection_index,
                          ULONG selection_buffer_size,
                          TS_SELECTION_ACP* selection_buffer,
                          ULONG* fetched_count) OVERRIDE;
  STDMETHOD(GetStatus)(TS_STATUS* pdcs) OVERRIDE;
  STDMETHOD(GetText)(LONG acp_start,
                     LONG acp_end,
                     wchar_t* text_buffer,
                     ULONG text_buffer_size,
                     ULONG* text_buffer_copied,
                     TS_RUNINFO* run_info_buffer,
                     ULONG run_info_buffer_size,
                     ULONG* run_info_buffer_copied,
                     LONG* next_acp) OVERRIDE;
  STDMETHOD(GetTextExt)(TsViewCookie view_cookie,
                        LONG acp_start,
                        LONG acp_end,
                        RECT* rect,
                        BOOL* clipped) OVERRIDE;
  STDMETHOD(GetWnd)(TsViewCookie view_cookie, HWND* window_handle) OVERRIDE;
  STDMETHOD(InsertEmbedded)(DWORD flags,
                            LONG acp_start,
                            LONG acp_end,
                            IDataObject* data_object,
                            TS_TEXTCHANGE* change) OVERRIDE;
  STDMETHOD(InsertEmbeddedAtSelection)(DWORD flags,
                                       IDataObject* data_object,
                                       LONG* acp_start,
                                       LONG* acp_end,
                                       TS_TEXTCHANGE* change) OVERRIDE;
  STDMETHOD(InsertTextAtSelection)(DWORD flags,
                                   const wchar_t* text_buffer,
                                   ULONG text_buffer_size,
                                   LONG* acp_start,
                                   LONG* acp_end,
                                   TS_TEXTCHANGE* text_change) OVERRIDE;
  STDMETHOD(QueryInsert)(LONG acp_test_start,
                         LONG acp_test_end,
                         ULONG text_size,
                         LONG* acp_result_start,
                         LONG* acp_result_end) OVERRIDE;
  STDMETHOD(QueryInsertEmbedded)(const GUID* service,
                                 const FORMATETC* format,
                                 BOOL* insertable) OVERRIDE;
  STDMETHOD(RequestAttrsAtPosition)(LONG acp_pos,
                                    ULONG attribute_buffer_size,
                                    const TS_ATTRID* attribute_buffer,
                                    DWORD flags) OVERRIDE;
  STDMETHOD(RequestAttrsTransitioningAtPosition)(
      LONG acp_pos,
      ULONG attribute_buffer_size,
      const TS_ATTRID* attribute_buffer,
      DWORD flags) OVERRIDE;
  STDMETHOD(RequestLock)(DWORD lock_flags, HRESULT* result) OVERRIDE;
  STDMETHOD(RequestSupportedAttrs)(DWORD flags,
                                   ULONG attribute_buffer_size,
                                   const TS_ATTRID* attribute_buffer) OVERRIDE;
  STDMETHOD(RetrieveRequestedAttrs)(ULONG attribute_buffer_size,
                                    TS_ATTRVAL* attribute_buffer,
                                    ULONG* attribute_buffer_copied) OVERRIDE;
  STDMETHOD(SetSelection)(ULONG selection_buffer_size,
                          const TS_SELECTION_ACP* selection_buffer) OVERRIDE;
  STDMETHOD(SetText)(DWORD flags,
                     LONG acp_start,
                     LONG acp_end,
                     const wchar_t* text_buffer,
                     ULONG text_buffer_size,
                     TS_TEXTCHANGE* text_change) OVERRIDE;
  STDMETHOD(UnadviseSink)(IUnknown* unknown) OVERRIDE;

  
  STDMETHOD(OnStartComposition)(ITfCompositionView* composition_view,
                                BOOL* ok) OVERRIDE;
  STDMETHOD(OnUpdateComposition)(ITfCompositionView* composition_view,
                                 ITfRange* range) OVERRIDE;
  STDMETHOD(OnEndComposition)(ITfCompositionView* composition_view) OVERRIDE;

  
  STDMETHOD(OnEndEdit)(ITfContext* context, TfEditCookie read_only_edit_cookie,
                       ITfEditRecord* edit_record) OVERRIDE;

  
  bool CancelComposition();

  
  bool ConfirmComposition();

  
  void SendOnLayoutChange();

  
  static scoped_refptr<TextStore> Create(
      HWND window_handle,
      const std::vector<InputScope>& input_scopes,
      TextStoreDelegate* delegate);

 private:
  friend CComObject<TextStore>;
  TextStore();

  void Initialize(HWND window_handle,
                  ITfCategoryMgr* category_manager,
                  ITfDisplayAttributeMgr* display_attribute_manager,
                  ITfInputScope* input_scope,
                  TextStoreDelegate* delegate);

  
  bool HasReadLock() const;

  
  bool HasReadWriteLock() const;

  
  bool GetDisplayAttribute(TfGuidAtom guid_atom,
                           TF_DISPLAYATTRIBUTE* attribute);

  
  bool GetCompositionStatus(
      ITfContext* context,
      const TfEditCookie read_only_edit_cookie,
      uint32* committed_size,
      std::vector<metro_viewer::UnderlineInfo>* undelines);

  
  base::win::ScopedComPtr<ITextStoreACPSink> text_store_acp_sink_;

  
  DWORD text_store_acp_sink_mask_;

  
  HWND window_handle_;

  
  
  
  
  string16 string_buffer_;
  uint32 committed_size_;

  
  
  
  
  
  uint32 selection_start_;
  uint32 selection_end_;

  
  
  
  
  
  
  
  
  
  std::vector<metro_viewer::UnderlineInfo> underlines_;

  
  
  bool edit_flag_;

  
  
  
  
  DWORD current_lock_type_;

  
  std::deque<DWORD> lock_queue_;

  
  
  base::win::ScopedComPtr<ITfCategoryMgr> category_manager_;
  base::win::ScopedComPtr<ITfDisplayAttributeMgr> display_attribute_manager_;

  
  base::win::ScopedComPtr<ITfInputScope> input_scope_;

  
  TextStoreDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(TextStore);
};

}  

#endif  
