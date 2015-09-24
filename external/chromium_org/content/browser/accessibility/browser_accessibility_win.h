// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_WIN_H_
#define CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_WIN_H_

#include <atlbase.h>
#include <atlcom.h>
#include <oleacc.h>
#include <UIAutomationCore.h>

#include <vector>

#include "base/compiler_specific.h"
#include "content/browser/accessibility/browser_accessibility.h"
#include "content/common/content_export.h"
#include "third_party/iaccessible2/ia2_api_all.h"
#include "third_party/isimpledom/ISimpleDOMDocument.h"
#include "third_party/isimpledom/ISimpleDOMNode.h"
#include "third_party/isimpledom/ISimpleDOMText.h"

namespace ui {
enum TextBoundaryDirection;
enum TextBoundaryType;
}

namespace content {
class BrowserAccessibilityRelation;

class __declspec(uuid("562072fe-3390-43b1-9e2c-dd4118f5ac79"))
BrowserAccessibilityWin
    : public BrowserAccessibility,
      public CComObjectRootEx<CComMultiThreadModel>,
      public IDispatchImpl<IAccessible2, &IID_IAccessible2,
                           &LIBID_IAccessible2Lib>,
      public IAccessibleApplication,
      public IAccessibleHyperlink,
      public IAccessibleHypertext,
      public IAccessibleImage,
      public IAccessibleTable,
      public IAccessibleTable2,
      public IAccessibleTableCell,
      public IAccessibleValue,
      public IServiceProvider,
      public ISimpleDOMDocument,
      public ISimpleDOMNode,
      public ISimpleDOMText,
      public IAccessibleEx,
      public IRawElementProviderSimple {
 public:
  BEGIN_COM_MAP(BrowserAccessibilityWin)
    COM_INTERFACE_ENTRY2(IDispatch, IAccessible2)
    COM_INTERFACE_ENTRY2(IAccessible, IAccessible2)
    COM_INTERFACE_ENTRY2(IAccessibleText, IAccessibleHypertext)
    COM_INTERFACE_ENTRY(IAccessible2)
    COM_INTERFACE_ENTRY(IAccessibleApplication)
    COM_INTERFACE_ENTRY(IAccessibleHyperlink)
    COM_INTERFACE_ENTRY(IAccessibleHypertext)
    COM_INTERFACE_ENTRY(IAccessibleImage)
    COM_INTERFACE_ENTRY(IAccessibleTable)
    COM_INTERFACE_ENTRY(IAccessibleTable2)
    COM_INTERFACE_ENTRY(IAccessibleTableCell)
    COM_INTERFACE_ENTRY(IAccessibleValue)
    COM_INTERFACE_ENTRY(IServiceProvider)
    COM_INTERFACE_ENTRY(ISimpleDOMDocument)
    COM_INTERFACE_ENTRY(ISimpleDOMNode)
    COM_INTERFACE_ENTRY(ISimpleDOMText)
    COM_INTERFACE_ENTRY(IAccessibleEx)
    COM_INTERFACE_ENTRY(IRawElementProviderSimple)
  END_COM_MAP()

  
  
  
  CONTENT_EXPORT static const char16 kEmbeddedCharacter[];

  
  
  static std::map<int32, base::string16> role_string_map;
  static std::map<int32, base::string16> state_string_map;

  CONTENT_EXPORT BrowserAccessibilityWin();

  CONTENT_EXPORT virtual ~BrowserAccessibilityWin();

  
  
  LONG unique_id_win() const { return unique_id_win_; }

  
  
  
  CONTENT_EXPORT virtual void PreInitialize() OVERRIDE;
  CONTENT_EXPORT virtual void PostInitialize() OVERRIDE;
  CONTENT_EXPORT virtual void NativeAddReference() OVERRIDE;
  CONTENT_EXPORT virtual void NativeReleaseReference() OVERRIDE;
  CONTENT_EXPORT virtual bool IsNative() const OVERRIDE;
  CONTENT_EXPORT virtual void SetLocation(const gfx::Rect& new_location)
      OVERRIDE;

  
  
  

  
  CONTENT_EXPORT STDMETHODIMP accDoDefaultAction(VARIANT var_id);

  
  CONTENT_EXPORT STDMETHODIMP accHitTest(LONG x_left, LONG y_top,
                                         VARIANT* child);

  
  CONTENT_EXPORT STDMETHODIMP accLocation(LONG* x_left,
                                          LONG* y_top,
                                          LONG* width,
                                          LONG* height,
                                          VARIANT var_id);

  
  CONTENT_EXPORT STDMETHODIMP accNavigate(LONG nav_dir, VARIANT start,
                                          VARIANT* end);

  
  CONTENT_EXPORT STDMETHODIMP get_accChild(VARIANT var_child,
                                           IDispatch** disp_child);

  
  CONTENT_EXPORT STDMETHODIMP get_accChildCount(LONG* child_count);

  
  CONTENT_EXPORT STDMETHODIMP get_accDefaultAction(VARIANT var_id,
                                                   BSTR* default_action);

  
  CONTENT_EXPORT STDMETHODIMP get_accDescription(VARIANT var_id, BSTR* desc);

  
  CONTENT_EXPORT STDMETHODIMP get_accFocus(VARIANT* focus_child);

  
  CONTENT_EXPORT STDMETHODIMP get_accHelp(VARIANT var_id, BSTR* heflp);

  
  CONTENT_EXPORT STDMETHODIMP get_accKeyboardShortcut(VARIANT var_id,
                                                      BSTR* access_key);

  
  CONTENT_EXPORT STDMETHODIMP get_accName(VARIANT var_id, BSTR* name);

  
  CONTENT_EXPORT STDMETHODIMP get_accParent(IDispatch** disp_parent);

  
  CONTENT_EXPORT STDMETHODIMP get_accRole(VARIANT var_id, VARIANT* role);

  
  CONTENT_EXPORT STDMETHODIMP get_accState(VARIANT var_id, VARIANT* state);

  
  CONTENT_EXPORT STDMETHODIMP get_accValue(VARIANT var_id, BSTR* value);

  
  CONTENT_EXPORT STDMETHODIMP accSelect(LONG flags_sel, VARIANT var_id);

  CONTENT_EXPORT STDMETHODIMP get_accHelpTopic(BSTR* help_file,
                                               VARIANT var_id,
                                               LONG* topic_id);

  CONTENT_EXPORT STDMETHODIMP get_accSelection(VARIANT* selected);

  
  CONTENT_EXPORT STDMETHODIMP put_accName(VARIANT var_id, BSTR put_name) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP put_accValue(VARIANT var_id, BSTR put_val) {
    return E_NOTIMPL;
  }

  
  
  

  
  CONTENT_EXPORT STDMETHODIMP role(LONG* role);

  
  CONTENT_EXPORT STDMETHODIMP get_states(AccessibleStates* states);

  
  
  CONTENT_EXPORT STDMETHODIMP get_attributes(BSTR* attributes);

  
  
  CONTENT_EXPORT STDMETHODIMP get_uniqueID(LONG* unique_id);

  
  CONTENT_EXPORT STDMETHODIMP get_windowHandle(HWND* window_handle);

  
  CONTENT_EXPORT STDMETHODIMP get_indexInParent(LONG* index_in_parent);

  CONTENT_EXPORT STDMETHODIMP get_nRelations(LONG* n_relations);

  CONTENT_EXPORT STDMETHODIMP get_relation(LONG relation_index,
                                           IAccessibleRelation** relation);

  CONTENT_EXPORT STDMETHODIMP get_relations(LONG max_relations,
                                            IAccessibleRelation** relations,
                                            LONG* n_relations);

  CONTENT_EXPORT STDMETHODIMP scrollTo(enum IA2ScrollType scroll_type);

  CONTENT_EXPORT STDMETHODIMP scrollToPoint(
      enum IA2CoordinateType coordinate_type,
      LONG x,
      LONG y);

  CONTENT_EXPORT STDMETHODIMP get_groupPosition(LONG* group_level,
                                                LONG* similar_items_in_group,
                                                LONG* position_in_group);

  
  
  
  CONTENT_EXPORT STDMETHODIMP get_extendedRole(BSTR* extended_role) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_localizedExtendedRole(
      BSTR* localized_extended_role) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_nExtendedStates(LONG* n_extended_states) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_extendedStates(LONG max_extended_states,
                                                 BSTR** extended_states,
                                                 LONG* n_extended_states) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_localizedExtendedStates(
      LONG max_localized_extended_states,
      BSTR** localized_extended_states,
      LONG* n_localized_extended_states) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_locale(IA2Locale* locale) {
    return E_NOTIMPL;
  }

  
  
  
  CONTENT_EXPORT STDMETHODIMP get_appName(BSTR* app_name);

  CONTENT_EXPORT STDMETHODIMP get_appVersion(BSTR* app_version);

  CONTENT_EXPORT STDMETHODIMP get_toolkitName(BSTR* toolkit_name);

  CONTENT_EXPORT STDMETHODIMP get_toolkitVersion(BSTR* toolkit_version);

  
  
  
  CONTENT_EXPORT STDMETHODIMP get_description(BSTR* description);

  CONTENT_EXPORT STDMETHODIMP get_imagePosition(
      enum IA2CoordinateType coordinate_type,
      LONG* x,
      LONG* y);

  CONTENT_EXPORT STDMETHODIMP get_imageSize(LONG* height, LONG* width);

  
  
  

  

  CONTENT_EXPORT STDMETHODIMP get_accessibleAt(long row,
                                               long column,
                                               IUnknown** accessible);

  CONTENT_EXPORT STDMETHODIMP get_caption(IUnknown** accessible);

  CONTENT_EXPORT STDMETHODIMP get_childIndex(long row_index,
                                             long column_index,
                                             long* cell_index);

  CONTENT_EXPORT STDMETHODIMP get_columnDescription(long column,
                                                    BSTR* description);

  CONTENT_EXPORT STDMETHODIMP get_columnExtentAt(long row,
                                                 long column,
                                                 long* n_columns_spanned);

  CONTENT_EXPORT STDMETHODIMP get_columnHeader(
      IAccessibleTable** accessible_table,
      long* starting_row_index);

  CONTENT_EXPORT STDMETHODIMP get_columnIndex(long cell_index,
                                              long* column_index);

  CONTENT_EXPORT STDMETHODIMP get_nColumns(long* column_count);

  CONTENT_EXPORT STDMETHODIMP get_nRows(long* row_count);

  CONTENT_EXPORT STDMETHODIMP get_nSelectedChildren(long* cell_count);

  CONTENT_EXPORT STDMETHODIMP get_nSelectedColumns(long* column_count);

  CONTENT_EXPORT STDMETHODIMP get_nSelectedRows(long *row_count);

  CONTENT_EXPORT STDMETHODIMP get_rowDescription(long row,
                                                 BSTR* description);

  CONTENT_EXPORT STDMETHODIMP get_rowExtentAt(long row,
                                              long column,
                                              long* n_rows_spanned);

  CONTENT_EXPORT STDMETHODIMP get_rowHeader(IAccessibleTable** accessible_table,
                                            long* starting_column_index);

  CONTENT_EXPORT STDMETHODIMP get_rowIndex(long cell_index,
                                           long* row_index);

  CONTENT_EXPORT STDMETHODIMP get_selectedChildren(long max_children,
                                                   long** children,
                                                   long* n_children);

  CONTENT_EXPORT STDMETHODIMP get_selectedColumns(long max_columns,
                                                  long** columns,
                                                  long* n_columns);

  CONTENT_EXPORT STDMETHODIMP get_selectedRows(long max_rows,
                                               long** rows,
                                               long* n_rows);

  CONTENT_EXPORT STDMETHODIMP get_summary(IUnknown** accessible);

  CONTENT_EXPORT STDMETHODIMP get_isColumnSelected(long column,
                                                   boolean* is_selected);

  CONTENT_EXPORT STDMETHODIMP get_isRowSelected(long row,
                                                boolean* is_selected);

  CONTENT_EXPORT STDMETHODIMP get_isSelected(long row,
                                             long column,
                                             boolean* is_selected);

  CONTENT_EXPORT STDMETHODIMP get_rowColumnExtentsAtIndex(long index,
                                                          long* row,
                                                          long* column,
                                                          long* row_extents,
                                                          long* column_extents,
                                                          boolean* is_selected);

  CONTENT_EXPORT STDMETHODIMP selectRow(long row) {
    return E_NOTIMPL;
  }

  CONTENT_EXPORT STDMETHODIMP selectColumn(long column) {
    return E_NOTIMPL;
  }

  CONTENT_EXPORT STDMETHODIMP unselectRow(long row) {
    return E_NOTIMPL;
  }

  CONTENT_EXPORT STDMETHODIMP unselectColumn(long column) {
    return E_NOTIMPL;
  }

  CONTENT_EXPORT STDMETHODIMP get_modelChange(
      IA2TableModelChange* model_change) {
    return E_NOTIMPL;
  }

  
  
  
  
  
  

  CONTENT_EXPORT STDMETHODIMP get_cellAt(long row,
                                         long column,
                                         IUnknown** cell);

  CONTENT_EXPORT STDMETHODIMP get_nSelectedCells(long* cell_count);

  CONTENT_EXPORT STDMETHODIMP get_selectedCells(IUnknown*** cells,
                                                long* n_selected_cells);

  CONTENT_EXPORT STDMETHODIMP get_selectedColumns(long** columns,
                                                  long* n_columns);

  CONTENT_EXPORT STDMETHODIMP get_selectedRows(long** rows,
                                               long* n_rows);

  
  
  

  CONTENT_EXPORT STDMETHODIMP get_columnExtent(long* n_columns_spanned);

  CONTENT_EXPORT STDMETHODIMP get_columnHeaderCells(
      IUnknown*** cell_accessibles,
      long* n_column_header_cells);

  CONTENT_EXPORT STDMETHODIMP get_columnIndex(long* column_index);

  CONTENT_EXPORT STDMETHODIMP get_rowExtent(long* n_rows_spanned);

  CONTENT_EXPORT STDMETHODIMP get_rowHeaderCells(IUnknown*** cell_accessibles,
                                                 long* n_row_header_cells);

  CONTENT_EXPORT STDMETHODIMP get_rowIndex(long* row_index);

  CONTENT_EXPORT STDMETHODIMP get_isSelected(boolean* is_selected);

  CONTENT_EXPORT STDMETHODIMP get_rowColumnExtents(long* row,
                                                   long* column,
                                                   long* row_extents,
                                                   long* column_extents,
                                                   boolean* is_selected);

  CONTENT_EXPORT STDMETHODIMP get_table(IUnknown** table);

  
  
  

  CONTENT_EXPORT STDMETHODIMP get_nCharacters(LONG* n_characters);

  CONTENT_EXPORT STDMETHODIMP get_caretOffset(LONG* offset);

  CONTENT_EXPORT STDMETHODIMP get_characterExtents(
      LONG offset,
      enum IA2CoordinateType coord_type,
      LONG* out_x,
      LONG* out_y,
      LONG* out_width,
      LONG* out_height);

  CONTENT_EXPORT STDMETHODIMP get_nSelections(LONG* n_selections);

  CONTENT_EXPORT STDMETHODIMP get_selection(LONG selection_index,
                                            LONG* start_offset,
                                            LONG* end_offset);

  CONTENT_EXPORT STDMETHODIMP get_text(LONG start_offset,
                                       LONG end_offset,
                                       BSTR* text);

  CONTENT_EXPORT STDMETHODIMP get_textAtOffset(
      LONG offset,
      enum IA2TextBoundaryType boundary_type,
      LONG* start_offset,
      LONG* end_offset,
      BSTR* text);

  CONTENT_EXPORT STDMETHODIMP get_textBeforeOffset(
      LONG offset,
      enum IA2TextBoundaryType boundary_type,
      LONG* start_offset,
      LONG* end_offset,
      BSTR* text);

  CONTENT_EXPORT STDMETHODIMP get_textAfterOffset(
      LONG offset,
      enum IA2TextBoundaryType boundary_type,
      LONG* start_offset,
      LONG* end_offset,
      BSTR* text);

  CONTENT_EXPORT STDMETHODIMP get_newText(IA2TextSegment* new_text);

  CONTENT_EXPORT STDMETHODIMP get_oldText(IA2TextSegment* old_text);

  CONTENT_EXPORT STDMETHODIMP get_offsetAtPoint(
      LONG x,
      LONG y,
      enum IA2CoordinateType coord_type,
      LONG* offset);

  CONTENT_EXPORT STDMETHODIMP scrollSubstringTo(
       LONG start_index,
       LONG end_index,
       enum IA2ScrollType scroll_type);

  CONTENT_EXPORT STDMETHODIMP scrollSubstringToPoint(
      LONG start_index,
      LONG end_index,
      enum IA2CoordinateType coordinate_type,
      LONG x, LONG y);

  CONTENT_EXPORT STDMETHODIMP addSelection(LONG start_offset, LONG end_offset);

  CONTENT_EXPORT STDMETHODIMP removeSelection(LONG selection_index);

  CONTENT_EXPORT STDMETHODIMP setCaretOffset(LONG offset);

  CONTENT_EXPORT STDMETHODIMP setSelection(LONG selection_index,
                                           LONG start_offset,
                                           LONG end_offset);

  
  CONTENT_EXPORT STDMETHODIMP get_attributes(LONG offset, LONG* start_offset,
                                             LONG* end_offset,
                                             BSTR* text_attributes) {
    return E_NOTIMPL;
  }

  
  
  

  CONTENT_EXPORT STDMETHODIMP get_nHyperlinks(long* hyperlink_count);

  CONTENT_EXPORT STDMETHODIMP get_hyperlink(long index,
                                            IAccessibleHyperlink** hyperlink);

  CONTENT_EXPORT STDMETHODIMP get_hyperlinkIndex(long char_index,
                                                 long* hyperlink_index);

  
  CONTENT_EXPORT STDMETHODIMP get_anchor(long index, VARIANT* anchor) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_anchorTarget(long index,
                                               VARIANT* anchor_target) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_startIndex( long* index) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_endIndex( long* index) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_valid(boolean* valid) {
    return E_NOTIMPL;
  }

  
  CONTENT_EXPORT STDMETHODIMP nActions(long* n_actions) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP doAction(long action_index) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_description(long action_index,
                                              BSTR* description) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_keyBinding(long action_index,
                                             long n_max_bindings,
                                             BSTR** key_bindings,
                                             long* n_bindings) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_name(long action_index, BSTR* name) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP get_localizedName(long action_index,
                                                BSTR* localized_name) {
    return E_NOTIMPL;
  }

  
  
  

  CONTENT_EXPORT STDMETHODIMP get_currentValue(VARIANT* value);

  CONTENT_EXPORT STDMETHODIMP get_minimumValue(VARIANT* value);

  CONTENT_EXPORT STDMETHODIMP get_maximumValue(VARIANT* value);

  CONTENT_EXPORT STDMETHODIMP setCurrentValue(VARIANT new_value);

  
  
  

  CONTENT_EXPORT STDMETHODIMP get_URL(BSTR* url);

  CONTENT_EXPORT STDMETHODIMP get_title(BSTR* title);

  CONTENT_EXPORT STDMETHODIMP get_mimeType(BSTR* mime_type);

  CONTENT_EXPORT STDMETHODIMP get_docType(BSTR* doc_type);

  CONTENT_EXPORT STDMETHODIMP get_nameSpaceURIForID(short name_space_id,
                                                    BSTR* name_space_uri) {
    return E_NOTIMPL;
  }
  CONTENT_EXPORT STDMETHODIMP put_alternateViewMediaTypes(
      BSTR* comma_separated_media_types) {
    return E_NOTIMPL;
  }

  
  
  

  CONTENT_EXPORT STDMETHODIMP get_nodeInfo(BSTR* node_name,
                                           short* name_space_id,
                                           BSTR* node_value,
                                           unsigned int* num_children,
                                           unsigned int* unique_id,
                                           unsigned short* node_type);

  CONTENT_EXPORT STDMETHODIMP get_attributes(unsigned short max_attribs,
                                             BSTR* attrib_names,
                                             short* name_space_id,
                                             BSTR* attrib_values,
                                             unsigned short* num_attribs);

  CONTENT_EXPORT STDMETHODIMP get_attributesForNames(
      unsigned short num_attribs,
      BSTR* attrib_names,
      short* name_space_id,
      BSTR* attrib_values);

  CONTENT_EXPORT STDMETHODIMP get_computedStyle(
      unsigned short max_style_properties,
      boolean use_alternate_view,
      BSTR *style_properties,
      BSTR *style_values,
      unsigned short *num_style_properties);

  CONTENT_EXPORT STDMETHODIMP get_computedStyleForProperties(
      unsigned short num_style_properties,
      boolean use_alternate_view,
      BSTR* style_properties,
      BSTR* style_values);

  CONTENT_EXPORT STDMETHODIMP scrollTo(boolean placeTopLeft);

  CONTENT_EXPORT STDMETHODIMP get_parentNode(ISimpleDOMNode** node);

  CONTENT_EXPORT STDMETHODIMP get_firstChild(ISimpleDOMNode** node);

  CONTENT_EXPORT STDMETHODIMP get_lastChild(ISimpleDOMNode** node);

  CONTENT_EXPORT STDMETHODIMP get_previousSibling(ISimpleDOMNode** node);

  CONTENT_EXPORT STDMETHODIMP get_nextSibling(ISimpleDOMNode** node);

  CONTENT_EXPORT STDMETHODIMP get_childAt(unsigned int child_index,
                                          ISimpleDOMNode** node);

  CONTENT_EXPORT STDMETHODIMP get_innerHTML(BSTR* innerHTML) {
    return E_NOTIMPL;
  }

  CONTENT_EXPORT STDMETHODIMP get_localInterface(void** local_interface) {
    return E_NOTIMPL;
  }

  CONTENT_EXPORT STDMETHODIMP get_language(BSTR* language) {
    return E_NOTIMPL;
  }

  
  
  

  CONTENT_EXPORT STDMETHODIMP get_domText(BSTR* dom_text);

  CONTENT_EXPORT STDMETHODIMP get_clippedSubstringBounds(
      unsigned int start_index,
      unsigned int end_index,
      int* out_x,
      int* out_y,
      int* out_width,
      int* out_height);

  CONTENT_EXPORT STDMETHODIMP get_unclippedSubstringBounds(
      unsigned int start_index,
      unsigned int end_index,
      int* out_x,
      int* out_y,
      int* out_width,
      int* out_height);

  CONTENT_EXPORT STDMETHODIMP scrollToSubstring(unsigned int start_index,
                                                unsigned int end_index);

  CONTENT_EXPORT STDMETHODIMP get_fontFamily(BSTR *font_family)  {
    return E_NOTIMPL;
  }

  
  
  

  CONTENT_EXPORT STDMETHODIMP QueryService(REFGUID guidService,
                                           REFIID riid,
                                           void** object);

  
  CONTENT_EXPORT STDMETHODIMP GetObjectForChild(long child_id,
                                                IAccessibleEx** ret) {
    return E_NOTIMPL;
  }

  CONTENT_EXPORT STDMETHODIMP GetIAccessiblePair(IAccessible** acc,
                                                 long* child_id) {
    return E_NOTIMPL;
  }

  CONTENT_EXPORT STDMETHODIMP GetRuntimeId(SAFEARRAY** runtime_id) {
    return E_NOTIMPL;
  }

  CONTENT_EXPORT STDMETHODIMP ConvertReturnedElement(
      IRawElementProviderSimple* element,
      IAccessibleEx** acc) {
    return E_NOTIMPL;
  }

  
  
  
  
  
  CONTENT_EXPORT STDMETHODIMP GetPatternProvider(PATTERNID id,
                                                 IUnknown** provider);
  CONTENT_EXPORT STDMETHODIMP GetPropertyValue(PROPERTYID id, VARIANT* ret);

  
  
  
  CONTENT_EXPORT STDMETHODIMP get_ProviderOptions(enum ProviderOptions* ret) {
    return E_NOTIMPL;
  }

  CONTENT_EXPORT STDMETHODIMP get_HostRawElementProvider(
      IRawElementProviderSimple** provider) {
    return E_NOTIMPL;
  }

  
  
  

  CONTENT_EXPORT HRESULT WINAPI InternalQueryInterface(
      void* this_ptr,
      const _ATL_INTMAP_ENTRY* entries,
      REFIID iid,
      void** object);

  
  int32 ia_role() const { return ia_role_; }
  int32 ia_state() const { return ia_state_; }
  const base::string16& role_name() const { return role_name_; }
  int32 ia2_role() const { return ia2_role_; }
  int32 ia2_state() const { return ia2_state_; }
  const std::vector<base::string16>& ia2_attributes() const {
    return ia2_attributes_;
  }

  
  
  int32 blink_role() const { return BrowserAccessibility::role(); }

 private:
  
  
  
  BrowserAccessibilityWin* NewReference();

  
  
  
  
  
  BrowserAccessibilityWin* GetTargetFromChildID(const VARIANT& var_id);

  
  
  void InitRoleAndState();

  
  
  
  HRESULT GetStringAttributeAsBstr(
      AccessibilityNodeData::StringAttribute attribute,
      BSTR* value_bstr);

  
  
  void StringAttributeToIA2(AccessibilityNodeData::StringAttribute attribute,
                            const char* ia2_attr);

  
  
  void BoolAttributeToIA2(AccessibilityNodeData::BoolAttribute attribute,
                          const char* ia2_attr);

  
  
  void IntAttributeToIA2(AccessibilityNodeData::IntAttribute attribute,
                         const char* ia2_attr);

  
  
  base::string16 GetValueText();

  
  
  base::string16 TextForIAccessibleText();

  
  
  void HandleSpecialTextOffset(const base::string16& text, LONG* offset);

  
  ui::TextBoundaryType IA2TextBoundaryToTextBoundary(IA2TextBoundaryType type);

  
  
  
  LONG FindBoundary(const base::string16& text,
                    IA2TextBoundaryType ia2_boundary,
                    LONG start_offset,
                    ui::TextBoundaryDirection direction);

  
  
  BrowserAccessibilityWin* GetFromRendererID(int32 renderer_id);

  
  
  
  LONG unique_id_win_;

  
  int32 ia_role_;
  int32 ia_state_;
  base::string16 role_name_;

  
  int32 ia2_role_;
  int32 ia2_state_;

  
  std::vector<base::string16> ia2_attributes_;

  
  
  bool first_time_;

  
  base::string16 previous_text_;

  
  
  
  base::string16 old_text_;

  
  int32 old_ia_state_;

  
  std::vector<BrowserAccessibilityRelation*> relations_;

  
  base::string16 hypertext_;

  
  
  std::map<int32, int32> hyperlink_offset_to_index_;

  
  
  std::vector<int32> hyperlinks_;

  
  int previous_scroll_x_;
  int previous_scroll_y_;

  
  static LONG next_unique_id_win_;

  
  friend class BrowserAccessibility;
  friend class BrowserAccessibilityRelation;

  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityWin);
};

}  

#endif  
