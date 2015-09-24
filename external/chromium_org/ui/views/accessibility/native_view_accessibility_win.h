// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_ACCESSIBILITY_NATIVE_VIEW_ACCESSIBILITY_WIN_H_
#define UI_VIEWS_ACCESSIBILITY_NATIVE_VIEW_ACCESSIBILITY_WIN_H_

#include <atlbase.h>
#include <atlcom.h>
#include <oleacc.h>

#include <UIAutomationCore.h>

#include <set>

#include "third_party/iaccessible2/ia2_api_all.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/views/accessibility/native_view_accessibility.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/view.h"

namespace ui {
enum TextBoundaryDirection;
enum TextBoundaryType;
}

namespace views {

class __declspec(uuid("26f5641a-246d-457b-a96d-07f3fae6acf2"))
NativeViewAccessibilityWin
  : public CComObjectRootEx<CComMultiThreadModel>,
    public IDispatchImpl<IAccessible2, &IID_IAccessible2,
                           &LIBID_IAccessible2Lib>,
    public IAccessibleText,
    public IServiceProvider,
    public IAccessibleEx,
    public IRawElementProviderSimple,
    public NativeViewAccessibility {
 public:
  BEGIN_COM_MAP(NativeViewAccessibilityWin)
    COM_INTERFACE_ENTRY2(IDispatch, IAccessible2)
    COM_INTERFACE_ENTRY2(IAccessible, IAccessible2)
    COM_INTERFACE_ENTRY(IAccessible2)
    COM_INTERFACE_ENTRY(IAccessibleText)
    COM_INTERFACE_ENTRY(IServiceProvider)
    COM_INTERFACE_ENTRY(IAccessibleEx)
    COM_INTERFACE_ENTRY(IRawElementProviderSimple)
  END_COM_MAP()

  virtual ~NativeViewAccessibilityWin();

  
  virtual void NotifyAccessibilityEvent(
      ui::AccessibilityTypes::Event event_type) OVERRIDE;
  virtual gfx::NativeViewAccessible GetNativeObject() OVERRIDE;
  virtual void Destroy() OVERRIDE;

  void set_view(views::View* view) { view_ = view; }

  

  
  virtual STDMETHODIMP accHitTest(LONG x_left, LONG y_top, VARIANT* child);

  
  STDMETHODIMP accDoDefaultAction(VARIANT var_id);

  
  STDMETHODIMP accLocation(LONG* x_left,
                           LONG* y_top,
                           LONG* width,
                           LONG* height,
                           VARIANT var_id);

  
  STDMETHODIMP accNavigate(LONG nav_dir, VARIANT start, VARIANT* end);

  
  virtual STDMETHODIMP get_accChild(VARIANT var_child, IDispatch** disp_child);

  
  virtual STDMETHODIMP get_accChildCount(LONG* child_count);

  
  STDMETHODIMP get_accDefaultAction(VARIANT var_id, BSTR* default_action);

  
  STDMETHODIMP get_accDescription(VARIANT var_id, BSTR* desc);

  
  STDMETHODIMP get_accFocus(VARIANT* focus_child);

  
  STDMETHODIMP get_accKeyboardShortcut(VARIANT var_id, BSTR* access_key);

  
  STDMETHODIMP get_accName(VARIANT var_id, BSTR* name);

  
  STDMETHODIMP get_accParent(IDispatch** disp_parent);

  
  STDMETHODIMP get_accRole(VARIANT var_id, VARIANT* role);

  
  STDMETHODIMP get_accState(VARIANT var_id, VARIANT* state);

  
  
  
  STDMETHODIMP get_accValue(VARIANT var_id, BSTR* value);
  STDMETHODIMP put_accValue(VARIANT var_id, BSTR new_value);

  
  STDMETHODIMP get_accSelection(VARIANT* selected);
  STDMETHODIMP accSelect(LONG flags_sel, VARIANT var_id);

  
  STDMETHODIMP get_accHelp(VARIANT var_id, BSTR* help);
  STDMETHODIMP get_accHelpTopic(BSTR* help_file,
                                VARIANT var_id,
                                LONG* topic_id);

  
  STDMETHODIMP put_accName(VARIANT var_id, BSTR put_name);

  
  
  

  STDMETHODIMP role(LONG* role);

  STDMETHODIMP get_states(AccessibleStates* states);

  STDMETHODIMP get_uniqueID(LONG* unique_id);

  STDMETHODIMP get_windowHandle(HWND* window_handle);

  
  
  

  STDMETHODIMP get_attributes(BSTR* attributes) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_indexInParent(LONG* index_in_parent) {
    return E_NOTIMPL;
  }
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
  STDMETHODIMP scrollToPoint(
      enum IA2CoordinateType coordinate_type,
      LONG x,
      LONG y) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_groupPosition(LONG* group_level,
                                 LONG* similar_items_in_group,
                                 LONG* position_in_group) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_localizedExtendedRole(
      BSTR* localized_extended_role) {
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
  STDMETHODIMP get_localizedExtendedStates(
      LONG max_localized_extended_states,
      BSTR** localized_extended_states,
      LONG* n_localized_extended_states) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_locale(IA2Locale* locale) {
    return E_NOTIMPL;
  }

  
  
  

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

  STDMETHODIMP get_offsetAtPoint(LONG x, LONG y,
      enum IA2CoordinateType coord_type,
      LONG* offset);

  
  
  

  STDMETHODIMP get_newText(IA2TextSegment* new_text) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_oldText(IA2TextSegment* old_text) {
    return E_NOTIMPL;
  }
  STDMETHODIMP addSelection(LONG start_offset, LONG end_offset) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_attributes(LONG offset,
                              LONG* start_offset,
                              LONG* end_offset,
                              BSTR* text_attributes) {
    return E_NOTIMPL;
  }
  STDMETHODIMP get_characterExtents(LONG offset,
      enum IA2CoordinateType coord_type,
      LONG* x, LONG* y,
      LONG* width, LONG* height) {
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
  STDMETHODIMP scrollSubstringToPoint(LONG start_index,
      LONG end_index,
      enum IA2CoordinateType coordinate_type,
      LONG x, LONG y) {
    return E_NOTIMPL;
  }

  
  
  

  STDMETHODIMP QueryService(REFGUID guidService, REFIID riid, void** object);

  
  
  
  STDMETHODIMP GetObjectForChild(long child_id, IAccessibleEx** ret) {
    return E_NOTIMPL;
  }

  STDMETHODIMP GetIAccessiblePair(IAccessible** acc, long* child_id) {
    return E_NOTIMPL;
  }

  STDMETHODIMP GetRuntimeId(SAFEARRAY** runtime_id) {
    return E_NOTIMPL;
  }

  STDMETHODIMP ConvertReturnedElement(IRawElementProviderSimple* element,
                                      IAccessibleEx** acc) {
    return E_NOTIMPL;
  }

  
  
  
  
  
  STDMETHODIMP GetPatternProvider(PATTERNID id, IUnknown** provider);
  STDMETHODIMP GetPropertyValue(PROPERTYID id, VARIANT* ret);

  
  
  
  STDMETHODIMP get_ProviderOptions(enum ProviderOptions* ret) {
    return E_NOTIMPL;
  }

  STDMETHODIMP get_HostRawElementProvider(
      IRawElementProviderSimple** provider) {
    return E_NOTIMPL;
  }

  

  
  
  static int32 MSAAEvent(ui::AccessibilityTypes::Event event);

  
  
  static int32 MSAARole(ui::AccessibilityTypes::Role role);

  
  
  static int32 MSAAState(ui::AccessibilityTypes::State state);

 protected:
  NativeViewAccessibilityWin();

  const View* view() const { return view_; }

 private:
  
  
  
  bool IsNavDirNext(int nav_dir) const;

  
  
  bool IsValidNav(int nav_dir,
                  int start_id,
                  int lower_bound,
                  int upper_bound) const;

  
  bool IsValidId(const VARIANT& child) const;

  
  void SetState(VARIANT* msaa_state, View* view);

  
  string16 TextForIAccessibleText();

  
  
  void HandleSpecialTextOffset(const string16& text, LONG* offset);

  
  ui::TextBoundaryType IA2TextBoundaryToTextBoundary(IA2TextBoundaryType type);

  
  
  
  LONG FindBoundary(const string16& text,
                    IA2TextBoundaryType ia2_boundary,
                    LONG start_offset,
                    ui::TextBoundaryDirection direction);

  
  
  
  void PopulateChildWidgetVector(std::vector<Widget*>* child_widgets);

  
  template <class Base> friend class CComObject;

  
  View* view_;

  
  long unique_id_;

  
  static long next_unique_id_;

  
  static const int kMaxViewStorageIds = 20;

  
  
  static int view_storage_ids_[kMaxViewStorageIds];

  
  static int next_view_storage_id_index_;

  DISALLOW_COPY_AND_ASSIGN(NativeViewAccessibilityWin);
};

}  

#endif  
