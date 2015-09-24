// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_VIEW_H_
#define CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_VIEW_H_

#include <string>

#include "base/strings/string16.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/ui/omnibox/omnibox_edit_model.h"
#include "content/public/common/url_constants.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/native_widget_types.h"

class CommandUpdater;
class GURL;
class OmniboxEditController;
class OmniboxViewMacTest;
class Profile;
class ToolbarModel;

namespace content {
class WebContents;
}

#if defined(TOOLKIT_VIEWS)
namespace gfx {
class Font;
}

namespace views {
class View;
}

namespace ui {
class DropTargetEvent;
}
#endif

class OmniboxView {
 public:
  virtual ~OmniboxView();

  
  OmniboxEditModel* model() { return model_.get(); }
  const OmniboxEditModel* model() const { return model_.get(); }

  CommandUpdater* command_updater() { return command_updater_; }
  const CommandUpdater* command_updater() const { return command_updater_; }

  
  
  virtual void SaveStateToTab(content::WebContents* tab) = 0;

  
  virtual void OnTabChanged(const content::WebContents* web_contents) = 0;

  
  virtual void Update() = 0;

  
  
  
  
  
  
  
  
  
  virtual void OpenMatch(const AutocompleteMatch& match,
                         WindowOpenDisposition disposition,
                         const GURL& alternate_nav_url,
                         size_t selected_line);

  
  
  
  virtual base::string16 GetText() const = 0;

  
  
  bool IsEditingOrEmpty() const;

  
  int GetIcon() const;

  
  
  
  void SetUserText(const base::string16& text);
  virtual void SetUserText(const base::string16& text,
                           const base::string16& display_text,
                           bool update_popup);

  
  
  virtual void SetWindowTextAndCaretPos(const base::string16& text,
                                        size_t caret_pos,
                                        bool update_popup,
                                        bool notify_text_changed) = 0;

  
  
  
  
  
  
  
  virtual void SetForcedQuery() = 0;

  
  virtual bool IsSelectAll() const = 0;

  
  virtual bool DeleteAtEndPressed() = 0;

  
  
  
  
  virtual void GetSelectionBounds(size_t* start, size_t* end) const = 0;

  
  
  virtual void SelectAll(bool reversed) = 0;

  
  
  void ShowURL();

  
  
  
  virtual void RevertAll();

  
  void RevertWithoutResettingSearchTermReplacement();

  
  
  virtual void UpdatePopup() = 0;

  
  
  
  virtual void CloseOmniboxPopup();

  
  virtual void SetFocus() = 0;

  
  
  virtual void ApplyCaretVisibility() = 0;

  
  
  
  
  
  virtual void OnTemporaryTextMaybeChanged(const base::string16& display_text,
                                           bool save_original_selection,
                                           bool notify_text_changed) = 0;

  
  
  
  
  virtual bool OnInlineAutocompleteTextMaybeChanged(
      const base::string16& display_text, size_t user_text_length) = 0;

  
  virtual void OnInlineAutocompleteTextCleared() = 0;

  
  
  virtual void OnRevertTemporaryText() = 0;

  
  
  
  
  virtual void OnBeforePossibleChange() = 0;
  
  
  virtual bool OnAfterPossibleChange() = 0;

  
  virtual gfx::NativeView GetNativeView() const = 0;

  
  
  
  
  virtual gfx::NativeView GetRelativeWindowForPopup() const = 0;

  
  virtual void SetGrayTextAutocompletion(const base::string16& input) = 0;

  
  virtual base::string16 GetGrayTextAutocompletion() const = 0;

  
  
  virtual int GetTextWidth() const = 0;

  
  virtual int GetWidth() const = 0;

  
  virtual bool IsImeComposing() const = 0;

  
  
  virtual bool IsImeShowingPopup() const;

  
  
  
  
  
  virtual bool IsIndicatingQueryRefinement() const;

#if defined(TOOLKIT_VIEWS)
  
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) = 0;
#endif

  
  static base::string16 StripJavascriptSchemas(const base::string16& text);

  
  
  
  
  
  
  
  static base::string16 SanitizeTextForPaste(const base::string16& text);

  
  
  
  static base::string16 GetClipboardText();

 protected:
  OmniboxView(Profile* profile,
              OmniboxEditController* controller,
              CommandUpdater* command_updater);

  
  virtual void TextChanged();

  
  
  
  virtual int GetOmniboxTextLength() const = 0;

  
  virtual void EmphasizeURLComponents() = 0;

  OmniboxEditController* controller() { return controller_; }
  const OmniboxEditController* controller() const { return controller_; }

 private:
  friend class OmniboxViewMacTest;
  FRIEND_TEST_ALL_PREFIXES(InstantExtendedTest, ShowURL);

  
  scoped_ptr<OmniboxEditModel> model_;
  OmniboxEditController* controller_;

  
  
  CommandUpdater* command_updater_;
};

#endif  
