// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_POPUP_CONTROLLER_IMPL_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_POPUP_CONTROLLER_IMPL_H_

#include "base/gtest_prod_util.h"
#include "base/i18n/rtl.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/autofill/autofill_popup_controller.h"
#include "content/public/browser/render_widget_host.h"
#include "ui/gfx/font.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/rect_f.h"

namespace content {
struct NativeWebKeyboardEvent;
class RenderViewHost;
class WebContents;
}

namespace gfx {
class Display;
}

namespace ui {
class KeyEvent;
}

namespace autofill {

class AutofillPopupDelegate;
class AutofillPopupView;

class AutofillPopupControllerImpl : public AutofillPopupController {
 public:
  
  
  
  
  static base::WeakPtr<AutofillPopupControllerImpl> GetOrCreate(
      base::WeakPtr<AutofillPopupControllerImpl> previous,
      base::WeakPtr<AutofillPopupDelegate> delegate,
      content::WebContents* web_contents,
      gfx::NativeView container_view,
      const gfx::RectF& element_bounds,
      base::i18n::TextDirection text_direction);

  
  void Show(const std::vector<base::string16>& names,
            const std::vector<base::string16>& subtexts,
            const std::vector<base::string16>& icons,
            const std::vector<int>& identifiers);

  
  void UpdateDataListValues(const std::vector<base::string16>& values,
                            const std::vector<base::string16>& labels);

  
  
  virtual void Hide() OVERRIDE;

  
  virtual void ViewDestroyed() OVERRIDE;

  bool HandleKeyPressEvent(const content::NativeWebKeyboardEvent& event);

  
  void set_hide_on_outside_click(bool hide_on_outside_click);

 protected:
  FRIEND_TEST_ALL_PREFIXES(AutofillExternalDelegateBrowserTest,
                           CloseWidgetAndNoLeaking);
  FRIEND_TEST_ALL_PREFIXES(AutofillPopupControllerUnitTest,
                           ProperlyResetController);

  AutofillPopupControllerImpl(base::WeakPtr<AutofillPopupDelegate> delegate,
                              content::WebContents* web_contents,
                              gfx::NativeView container_view,
                              const gfx::RectF& element_bounds,
                              base::i18n::TextDirection text_direction);
  virtual ~AutofillPopupControllerImpl();

  
  virtual void UpdateBoundsAndRedrawPopup() OVERRIDE;
  virtual void LineSelectedAtPoint(int x, int y) OVERRIDE;
  virtual void LineAcceptedAtPoint(int x, int y) OVERRIDE;
  virtual void SelectionCleared() OVERRIDE;
  virtual bool ShouldRepostEvent(const ui::MouseEvent& event) OVERRIDE;
  virtual void AcceptSuggestion(size_t index) OVERRIDE;
  virtual int GetIconResourceID(
      const base::string16& resource_name) const OVERRIDE;
  virtual bool CanDelete(size_t index) const OVERRIDE;
  virtual bool IsWarning(size_t index) const OVERRIDE;
  virtual gfx::Rect GetRowBounds(size_t index) OVERRIDE;
  virtual void SetPopupBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual const gfx::Rect& popup_bounds() const OVERRIDE;
  virtual gfx::NativeView container_view() const OVERRIDE;
  virtual const gfx::RectF& element_bounds() const OVERRIDE;
  virtual bool IsRTL() const OVERRIDE;

  virtual const std::vector<base::string16>& names() const OVERRIDE;
  virtual const std::vector<base::string16>& subtexts() const OVERRIDE;
  virtual const std::vector<base::string16>& icons() const OVERRIDE;
  virtual const std::vector<int>& identifiers() const OVERRIDE;
#if !defined(OS_ANDROID)
  virtual const gfx::Font& GetNameFontForRow(size_t index) const OVERRIDE;
  virtual const gfx::Font& subtext_font() const OVERRIDE;
#endif
  virtual int selected_line() const OVERRIDE;
  virtual bool hide_on_outside_click() const OVERRIDE;

  
  void SetSelectedLine(int selected_line);

  
  void SelectNextLine();

  
  void SelectPreviousLine();

  
  bool AcceptSelectedLine();

  
  bool RemoveSelectedLine();

  
  int LineFromY(int y);

  
  int GetRowHeightFromId(int identifier) const;

  
  bool CanAccept(int id);

  
  bool HasSuggestions();

  
  
  void SetValues(const std::vector<base::string16>& names,
                 const std::vector<base::string16>& subtexts,
                 const std::vector<base::string16>& icons,
                 const std::vector<int>& identifier);

  AutofillPopupView* view() { return view_; }

  
  virtual void ShowView();
  virtual void InvalidateRow(size_t row);

  
#if !defined(OS_ANDROID)
  
  int GetDesiredPopupWidth() const;

  
  int GetDesiredPopupHeight() const;

  
  
  
  int RowWidthWithoutText(int row) const;
#endif

  base::WeakPtr<AutofillPopupControllerImpl> GetWeakPtr();

 private:
  
  
  void ClearState();

  const gfx::Rect RoundedElementBounds() const;
#if !defined(OS_ANDROID)
  
  
  void UpdatePopupBounds();
#endif

  
  
  virtual gfx::Display GetDisplayNearestPoint(const gfx::Point& point) const;

  
  
  std::pair<int, int> CalculatePopupXAndWidth(
      const gfx::Display& left_display,
      const gfx::Display& right_display,
      int popup_required_width) const;

  
  
  std::pair<int, int> CalculatePopupYAndHeight(
      const gfx::Display& top_display,
      const gfx::Display& bottom_display,
      int popup_required_height) const;

  AutofillPopupView* view_;  
  base::WeakPtr<AutofillPopupDelegate> delegate_;

  
  
  
  content::WebContents* web_contents_;

  gfx::NativeView container_view_;  

  
  
  const gfx::RectF element_bounds_;

  
  gfx::Rect popup_bounds_;

  
  base::i18n::TextDirection text_direction_;

  
  
  content::RenderViewHost* registered_key_press_event_callback_with_;

  
  std::vector<base::string16> names_;
  std::vector<base::string16> subtexts_;
  std::vector<base::string16> icons_;
  std::vector<int> identifiers_;

  
  
  std::vector<base::string16> full_names_;

#if !defined(OS_ANDROID)
  
  gfx::Font name_font_;
  gfx::Font subtext_font_;
  gfx::Font warning_font_;
#endif

  
  
  int selected_line_;

  
  bool hide_on_outside_click_;

  content::RenderWidgetHost::KeyPressEventCallback key_press_event_callback_;

  base::WeakPtrFactory<AutofillPopupControllerImpl> weak_ptr_factory_;
};

}  

#endif  
