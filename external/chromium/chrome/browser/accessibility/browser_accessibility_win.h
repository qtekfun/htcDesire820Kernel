// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_WIN_H_
#define CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_WIN_H_
#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <oleacc.h>

#include <vector>

#include "chrome/browser/accessibility/browser_accessibility.h"
#include "ia2_api_all.h"  
#include "ISimpleDOMDocument.h"  
#include "ISimpleDOMNode.h"  
#include "ISimpleDOMText.h"  
#include "webkit/glue/webaccessibility.h"

class BrowserAccessibilityManagerWin;

using webkit_glue::WebAccessibility;

class BrowserAccessibilityWin
    : public BrowserAccessibility,
      public CComObjectRootEx<CComMultiThreadModel>,
      public IDispatchImpl<IAccessible2, &IID_IAccessible2,
                           &LIBID_IAccessible2Lib>,
      public IAccessibleImage,
      public IAccessibleText,
      public IServiceProvider,
      public ISimpleDOMDocument,
      public ISimpleDOMNode,
      public ISimpleDOMText {
 public:
  BEGIN_COM_MAP(BrowserAccessibilityWin)
    COM_INTERFACE_ENTRY2(IDispatch, IAccessible2)
    COM_INTERFACE_ENTRY2(IAccessible, IAccessible2)
    COM_INTERFACE_ENTRY(IAccessible2)
    COM_INTERFACE_ENTRY(IAccessibleImage)
    COM_INTERFACE_ENTRY(IAccessibleText)
    COM_INTERFACE_ENTRY(IServiceProvider)
    COM_INTERFACE_ENTRY(ISimpleDOMDocument)
    COM_INTERFACE_ENTRY(ISimpleDOMNode)
    COM_INTERFACE_ENTRY(ISimpleDOMText)
  END_COM_MAP()

  BrowserAccessibilityWin();

  virtual ~BrowserAccessibilityWin();

  
  
  
  virtual void Initialize();
  virtual void NativeAddReference();
  virtual void NativeReleaseReference();

  
  
  

  
  STDMETHODIMP accDoDefaultAction(VARIANT var_id);

  
  STDMETHODIMP accHitTest(LONG x_left, LONG y_top, VARIANT* child);

  
  STDMETHODIMP accLocation(LONG* x_left,
                           LONG* y_top,
                           LONG* width,
                           LONG* height,
                           VARIANT var_id);

  
  STDMETHODIMP accNavigate(LONG nav_dir, VARIANT start, VARIANT* end);

  
  STDMETHODIMP get_accChild(VARIANT var_child, IDispatch** disp_child);

  
  STDMETHODIMP get_accChildCount(LONG* child_count);

  
  STDMETHODIMP get_accDefaultAction(VARIANT var_id, BSTR* default_action);

  
  STDMETHODIMP get_accDescription(VARIANT var_id, BSTR* desc);

  
  STDMETHODIMP get_accFocus(VARIANT* focus_child);

  
  STDMETHODIMP get_accHelp(VARIANT var_id, BSTR* help);

  
  STDMETHODIMP get_accKeyboardShortcut(VARIANT var_id, BSTR* access_key);

  
  STDMETHODIMP get_accName(VARIANT var_id, BSTR* name);

  
  STDMETHODIMP get_accParent(IDispatch** disp_parent);

  
  STDMETHODIMP get_accRole(VARIANT var_id, VARIANT* role);

  
  STDMETHODIMP get_accState(VARIANT var_id, VARIANT* state);

  
  STDMETHODIMP get_accValue(VARIANT var_id, BSTR* value);

  
  STDMETHODIMP accSelect(LONG flags_sel, VARIANT var_id);

  STDMETHODIMP get_accHelpTopic(BSTR* help_file,
                                VARIANT var_id,
                                LONG* topic_id);

  STDMETHODIMP get_accSelection(VARIANT* selected);

  
  STDMETHODIMP put_accName(VARIANT var_id, BSTR put_name) {
    return E_NOTIMPL;
  }
  STDMETHODIMP put_accValue(VARIANT var_id, BSTR put_val) {
    return E_NOTIMPL;
  }

  
  
  

  
  STDMETHODIMP role(LONG* role);

  
  STDMETHODIMP get_states(AccessibleStates* states);

  
  
  STDMETHODIMP get_attributes(BSTR* attributes);

  
  
  STDMETHODIMP get_uniqueID(LONG* unique_id);

  
  STDMETHODIMP get_windowHandle(HWND* window_handle);

  
  STDMETHODIMP get_indexInParent(LONG* index_in_parent);

  
  STDMETHODIMP get_extendedRole(BSTR* extended_role) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_nRelations(LONG* n_relations) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_relation(LONG relation_index,
                            IAccessibleRelation** relation) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_relations(LONG max_relations,
                             IAccessibleRelation** relations,
                             LONG* n_relations) {
    return E_NOTIMPL;
  }
  STDMETHODIMP scrollTo(enum IA2ScrollType scroll_type) {
    return E_NOTIMPL;
  }
  STDMETHODIMP scrollToPoint(enum IA2CoordinateType coordinate_type,
                             LONG x,
                             LONG y) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_groupPosition(LONG* group_level,
                                 LONG* similar_items_in_group,
                                 LONG* position_in_group) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_localizedExtendedRole(BSTR* localized_extended_role) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_nExtendedStates(LONG* n_extended_states) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_extendedStates(LONG max_extended_states,
                                  BSTR** extended_states,
                                  LONG* n_extended_states) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_localizedExtendedStates(LONG max_localized_extended_states,
                                           BSTR** localized_extended_states,
                                           LONG* n_localized_extended_states) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_locale(IA2Locale* locale) {
    return E_NOTIMPL;
  }

  
  
  

  STDMETHODIMP get_description(BSTR* description);

  STDMETHODIMP get_imagePosition(enum IA2CoordinateType coordinate_type,
                                 LONG* x, LONG* y);

  STDMETHODIMP get_imageSize(LONG* height, LONG* width);

  
  
  

  STDMETHODIMP get_nCharacters(LONG* n_characters);

  STDMETHODIMP get_caretOffset(LONG* offset);

  STDMETHODIMP get_nSelections(LONG* n_selections);

  STDMETHODIMP get_selection(LONG selection_index,
                             LONG* start_offset,
                             LONG* end_offset);

  STDMETHODIMP get_text(LONG start_offset, LONG end_offset, BSTR* text);

  STDMETHODIMP get_textAtOffset(LONG offset,
                                enum IA2TextBoundaryType boundary_type,
                                LONG* start_offset, LONG* end_offset,
                                BSTR* text);

  STDMETHODIMP get_textBeforeOffset(LONG offset,
                                    enum IA2TextBoundaryType boundary_type,
                                    LONG* start_offset, LONG* end_offset,
                                    BSTR* text);

  STDMETHODIMP get_textAfterOffset(LONG offset,
                                   enum IA2TextBoundaryType boundary_type,
                                   LONG* start_offset, LONG* end_offset,
                                   BSTR* text);

  
  STDMETHODIMP addSelection(LONG start_offset, LONG end_offset) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_attributes(LONG offset, LONG* start_offset, LONG* end_offset,
                              BSTR* text_attributes) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_characterExtents(LONG offset,
                                    enum IA2CoordinateType coord_type,
                                    LONG* x, LONG* y,
                                    LONG* width, LONG* height) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_offsetAtPoint(LONG x, LONG y,
                                 enum IA2CoordinateType coord_type,
                                 LONG* offset) {
    return E_NOTIMPL;
  }
  STDMETHODIMP removeSelection(LONG selection_index) {
    return E_NOTIMPL;
  }
  STDMETHODIMP setCaretOffset(LONG offset) {
    return E_NOTIMPL;
  }
  STDMETHODIMP setSelection(LONG selection_index,
                            LONG start_offset,
                            LONG end_offset) {
    return E_NOTIMPL;
  }
  STDMETHODIMP scrollSubstringTo(LONG start_index,
                                 LONG end_index,
                                 enum IA2ScrollType scroll_type) {
    return E_NOTIMPL;
  }
  STDMETHODIMP scrollSubstringToPoint(LONG start_index, LONG end_index,
                                      enum IA2CoordinateType coordinate_type,
                                      LONG x, LONG y) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_newText(IA2TextSegment* new_text) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_oldText(IA2TextSegment* old_text) {
    return E_NOTIMPL;
  }

  
  
  

  STDMETHODIMP get_URL(BSTR* url);

  STDMETHODIMP get_title(BSTR* title);

  STDMETHODIMP get_mimeType(BSTR* mime_type);

  STDMETHODIMP get_docType(BSTR* doc_type);

  STDMETHODIMP get_nameSpaceURIForID(
      short name_space_id, BSTR *name_space_uri) {
    return E_NOTIMPL;
  }
  STDMETHODIMP put_alternateViewMediaTypes(
      BSTR *comma_separated_media_types) {
    return E_NOTIMPL;
  }

  
  
  

  STDMETHODIMP get_nodeInfo(
      BSTR* node_name,
      short* name_space_id,
      BSTR* node_value,
      unsigned int* num_children,
      unsigned int* unique_id,
      unsigned short* node_type);

  STDMETHODIMP get_attributes(
      unsigned short max_attribs,
      BSTR* attrib_names,
      short* name_space_id,
      BSTR* attrib_values,
      unsigned short* num_attribs);

  STDMETHODIMP get_attributesForNames(
      unsigned short num_attribs,
      BSTR* attrib_names,
      short* name_space_id,
      BSTR* attrib_values);

  STDMETHODIMP get_computedStyle(
      unsigned short max_style_properties,
      boolean use_alternate_view,
      BSTR *style_properties,
      BSTR *style_values,
      unsigned short *num_style_properties);

  STDMETHODIMP get_computedStyleForProperties(
      unsigned short num_style_properties,
      boolean use_alternate_view,
      BSTR* style_properties,
      BSTR* style_values);

  STDMETHODIMP scrollTo(boolean placeTopLeft);

  STDMETHODIMP get_parentNode(ISimpleDOMNode** node);

  STDMETHODIMP get_firstChild(ISimpleDOMNode** node);

  STDMETHODIMP get_lastChild(ISimpleDOMNode** node);

  STDMETHODIMP get_previousSibling(ISimpleDOMNode** node);

  STDMETHODIMP get_nextSibling(ISimpleDOMNode** node);

  STDMETHODIMP get_childAt(
      unsigned int child_index,
      ISimpleDOMNode** node);

  STDMETHODIMP get_innerHTML(BSTR* innerHTML) {
    return E_NOTIMPL;
  }

  STDMETHODIMP get_localInterface(void** local_interface) {
    return E_NOTIMPL;
  }

  STDMETHODIMP get_language(BSTR* language) {
    return E_NOTIMPL;
  }

  
  
  

  STDMETHODIMP get_domText(BSTR* dom_text);

  STDMETHODIMP get_clippedSubstringBounds(
      unsigned int start_index,
      unsigned int end_index,
      int* x,
      int* y,
      int* width,
      int* height) {
    return E_NOTIMPL;
  }

  STDMETHODIMP get_unclippedSubstringBounds(
      unsigned int start_index,
      unsigned int end_index,
      int* x,
      int* y,
      int* width,
      int* height) {
    return E_NOTIMPL;
  }

  STDMETHODIMP scrollToSubstring(
      unsigned int start_index,
      unsigned int end_index)  {
    return E_NOTIMPL;
  }

  STDMETHODIMP get_fontFamily(BSTR *font_family)  {
    return E_NOTIMPL;
  }

  
  
  

  STDMETHODIMP QueryService(REFGUID guidService, REFIID riid, void** object);

  
  
  

  HRESULT WINAPI InternalQueryInterface(void* this_ptr,
                                        const _ATL_INTMAP_ENTRY* entries,
                                        REFIID iid,
                                        void** object);

 private:
  
  
  
  BrowserAccessibilityWin* NewReference();

  
  
  
  
  
  BrowserAccessibilityWin* GetTargetFromChildID(const VARIANT& var_id);

  
  
  void InitRoleAndState();

  
  
  
  HRESULT GetAttributeAsBstr(
      WebAccessibility::Attribute attribute, BSTR* value_bstr);

  
  string16 Escape(const string16& str);

  
  
  const string16& TextForIAccessibleText();

  
  
  
  LONG FindBoundary(const string16& text,
                    IA2TextBoundaryType boundary,
                    LONG start_offset,
                    LONG direction);

  
  int32 ia_role_;
  int32 ia_state_;

  
  int32 ia2_role_;
  int32 ia2_state_;

  
  friend class BrowserAccessibility;

  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityWin);
};

#endif  
