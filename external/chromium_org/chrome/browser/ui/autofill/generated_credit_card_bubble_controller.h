// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_GENERATED_CREDIT_CARD_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_UI_AUTOFILL_GENERATED_CREDIT_CARD_BUBBLE_CONTROLLER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/range/range.h"

namespace content {
class WebContents;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace autofill {

class GeneratedCreditCardBubbleView;

struct TextRange {
  
  gfx::Range range;
  
  bool is_link;
  
  bool operator==(const TextRange& other) const;
};

class GeneratedCreditCardBubbleController
    : public content::WebContentsObserver,
      public content::WebContentsUserData<GeneratedCreditCardBubbleController> {
 public:
  virtual ~GeneratedCreditCardBubbleController();

  
  static void RegisterUserPrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  static void Show(content::WebContents* contents,
                   const base::string16& fronting_card_name,
                   const base::string16& backing_card_name);

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;

  
  bool IsHiding() const;

  
  gfx::Image AnchorIcon() const;

  
  const base::string16& TitleText() const;

  
  const base::string16& ContentsText() const;

  
  const std::vector<TextRange>& ContentsTextRanges() const;

  
  void OnAnchorClicked();

  
  
  void OnLinkClicked();

  
  
  content::WebContents* web_contents() { return web_contents_; }
  const content::WebContents* web_contents() const { return web_contents_; }

 protected:
  
  explicit GeneratedCreditCardBubbleController(content::WebContents* contents);

  
  base::WeakPtr<GeneratedCreditCardBubbleController> GetWeakPtr();

  
  virtual base::WeakPtr<GeneratedCreditCardBubbleView> CreateBubble();

  
  virtual base::WeakPtr<GeneratedCreditCardBubbleView> bubble();

  
  virtual bool CanShow() const;

  
  
  
  bool ShouldDisplayBubbleInitially() const;

  
  base::string16 fronting_card_name() const { return fronting_card_name_; }
  base::string16 backing_card_name() const { return backing_card_name_; }

  
  
  
  virtual void SetupAndShow(const base::string16& fronting_card_name,
                            const base::string16& backing_card_name);

 private:
  friend class
      content::WebContentsUserData<GeneratedCreditCardBubbleController>;

  
  void Show(bool was_anchor_click);

  
  void UpdateAnchor();

  
  void Hide();

  
  content::WebContents* const web_contents_;

  
  base::string16 fronting_card_name_;
  base::string16 backing_card_name_;

  
  const base::string16 title_text_;

  
  
  base::string16 contents_text_;
  std::vector<TextRange> contents_text_ranges_;

  
  
  
  base::WeakPtr<GeneratedCreditCardBubbleView> bubble_;

  
  bool should_show_anchor_;

  
  base::WeakPtrFactory<GeneratedCreditCardBubbleController> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(GeneratedCreditCardBubbleController);
};

}  

#endif  
