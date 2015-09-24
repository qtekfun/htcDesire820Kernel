// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_ACCESSIBILITY_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_ACCESSIBILITY_H_
#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <oleacc.h>

#include "base/basictypes.h"
#include "base/win/scoped_comptr.h"

class AutocompleteEditViewWin;

class ATL_NO_VTABLE AutocompleteAccessibility
  : public CComObjectRootEx<CComMultiThreadModel>,
    public IDispatchImpl<IAccessible, &IID_IAccessible, &LIBID_Accessibility> {
 public:
  BEGIN_COM_MAP(AutocompleteAccessibility)
    COM_INTERFACE_ENTRY2(IDispatch, IAccessible)
    COM_INTERFACE_ENTRY(IAccessible)
  END_COM_MAP()

  AutocompleteAccessibility() {}
  ~AutocompleteAccessibility() {}

  HRESULT Initialize(const AutocompleteEditViewWin* edit_box);

  

  
  STDMETHODIMP get_accChildCount(LONG* child_count);

  
  STDMETHODIMP get_accChild(VARIANT var_child, IDispatch** disp_child);

  
  STDMETHODIMP get_accParent(IDispatch** disp_parent);

  
  STDMETHODIMP accNavigate(LONG nav_dir, VARIANT start, VARIANT* end);

  
  STDMETHODIMP get_accFocus(VARIANT* focus_child);

  
  STDMETHODIMP get_accName(VARIANT var_id, BSTR* name);

  
  STDMETHODIMP get_accDescription(VARIANT var_id, BSTR* desc);

  
  STDMETHODIMP get_accValue(VARIANT var_id, BSTR* value);

  
  STDMETHODIMP get_accState(VARIANT var_id, VARIANT* state);

  
  STDMETHODIMP get_accRole(VARIANT var_id, VARIANT* role);

  
  STDMETHODIMP get_accDefaultAction(VARIANT var_id, BSTR* default_action);

  
  STDMETHODIMP accLocation(LONG* x_left, LONG* y_top, LONG* width, LONG* height,
                           VARIANT var_id);

  
  STDMETHODIMP accHitTest(LONG x_left, LONG y_top, VARIANT* child);

  
  STDMETHODIMP get_accKeyboardShortcut(VARIANT var_id, BSTR* access_key);

  

  
  STDMETHODIMP accDoDefaultAction(VARIANT var_id);

  
  STDMETHODIMP get_accSelection(VARIANT* selected);
  STDMETHODIMP accSelect(LONG flags_sel, VARIANT var_id);

  
  STDMETHODIMP get_accHelp(VARIANT var_id, BSTR* help);
  STDMETHODIMP get_accHelpTopic(BSTR* help_file, VARIANT var_id,
                                LONG* topic_id);

  
  STDMETHODIMP put_accName(VARIANT var_id, BSTR put_name);
  STDMETHODIMP put_accValue(VARIANT var_id, BSTR put_val);

 protected:
  
  
  
  base::win::ScopedComPtr<IAccessible> default_accessibility_server_;

 private:
  const AutocompleteEditViewWin* edit_box_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteAccessibility);
};

#endif  
