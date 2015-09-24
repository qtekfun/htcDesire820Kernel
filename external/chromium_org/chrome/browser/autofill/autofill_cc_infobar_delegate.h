// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_AUTOFILL_CC_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_AUTOFILL_AUTOFILL_CC_INFOBAR_DELEGATE_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/infobars/confirm_infobar_delegate.h"
#include "components/autofill/core/browser/autofill_metrics.h"
#include "ui/base/window_open_disposition.h"

class CreditCard;
class PersonalDataManager;

namespace content {
struct LoadCommittedDetails;
}

namespace autofill {

class AutofillCCInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  
  
  static void Create(InfoBarService* infobar_service,
                     const AutofillMetrics* metric_logger,
                     const base::Closure& save_card_callback);

#if defined(UNIT_TEST)
  static scoped_ptr<ConfirmInfoBarDelegate> Create(
      const AutofillMetrics* metric_logger,
      const base::Closure& save_card_callback) {
    return scoped_ptr<ConfirmInfoBarDelegate>(
        new AutofillCCInfoBarDelegate(metric_logger, save_card_callback));
  }
#endif

 private:
  AutofillCCInfoBarDelegate(const AutofillMetrics* metric_logger,
                            const base::Closure& save_card_callback);
  virtual ~AutofillCCInfoBarDelegate();

  void LogUserAction(AutofillMetrics::InfoBarMetric user_action);

  
  virtual void InfoBarDismissed() OVERRIDE;
  virtual int GetIconID() const OVERRIDE;
  virtual Type GetInfoBarType() const OVERRIDE;
  virtual bool ShouldExpireInternal(
      const content::LoadCommittedDetails& details) const OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual base::string16 GetLinkText() const OVERRIDE;
  virtual bool LinkClicked(WindowOpenDisposition disposition) OVERRIDE;

  
  
  const AutofillMetrics* metric_logger_;

  
  base::Closure save_card_callback_;

  
  bool had_user_interaction_;

  FRIEND_TEST_ALL_PREFIXES(AutofillMetricsTest, CreditCardInfoBar);

  DISALLOW_COPY_AND_ASSIGN(AutofillCCInfoBarDelegate);
};

}  

#endif  
