// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_NEW_CREDIT_CARD_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_UI_AUTOFILL_NEW_CREDIT_CARD_BUBBLE_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "ui/gfx/image/image.h"

class Profile;

namespace content {
class WebContents;
}

namespace autofill {

class NewCreditCardBubbleView;
class AutofillProfile;
class CreditCard;

struct CreditCardDescription {
 CreditCardDescription();
 ~CreditCardDescription();
 
 gfx::Image icon;
 
 base::string16 name;
 
 base::string16 description;
};

class NewCreditCardBubbleController {
 public:
  virtual ~NewCreditCardBubbleController();

  
  
  
  static void Show(content::WebContents* web_contents,
                   scoped_ptr<CreditCard> new_card,
                   scoped_ptr<AutofillProfile> billing_profile);

  
  const base::string16& TitleText() const;

  
  const CreditCardDescription& CardDescription() const;

  
  const base::string16& LinkText() const;

  
  void OnBubbleDestroyed();

  
  void OnLinkClicked();

  
  Profile* profile() { return profile_; }

  
  content::WebContents* web_contents() { return web_contents_; }

 protected:
  
  explicit NewCreditCardBubbleController(content::WebContents* web_contents);

  
  virtual base::WeakPtr<NewCreditCardBubbleView> CreateBubble();

  
  virtual base::WeakPtr<NewCreditCardBubbleView> bubble();

  
  
  virtual void SetupAndShow(scoped_ptr<CreditCard> new_card,
                            scoped_ptr<AutofillProfile> billing_profile);

 private:
  friend class NewCreditCardBubbleCocoaUnitTest;

  
  void Hide();

  
  
  Profile* const profile_;

  
  content::WebContents* const web_contents_;

  
  scoped_ptr<CreditCard> new_card_;
  scoped_ptr<AutofillProfile> billing_profile_;

  
  const base::string16 title_text_;

  
  const base::string16 link_text_;

  
  
  struct CreditCardDescription card_desc_;

  
  
  
  base::WeakPtr<NewCreditCardBubbleView> bubble_;

  
  base::WeakPtrFactory<NewCreditCardBubbleController> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(NewCreditCardBubbleController);
};

}  

#endif  
