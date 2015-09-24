// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CLOUD_PRINT_CLOUD_PRINT_CONSTANTS_H_
#define CHROME_COMMON_CLOUD_PRINT_CLOUD_PRINT_CONSTANTS_H_

#include "base/basictypes.h"

namespace cloud_print {

extern const char kCloudPrintUserAgent[];
extern const char kChromeCloudPrintProxyHeader[];
extern const char kCloudPrintPushNotificationsSource[];
extern const char kCloudPrintAuth[];

extern const char kProxyIdValue[];
extern const char kPrinterNameValue[];
extern const char kPrinterDescValue[];
extern const char kPrinterCapsValue[];
extern const char kPrinterDisplayNameValue[];
extern const char kPrinterDefaultsValue[];
extern const char kPrinterStatusValue[];
extern const char kPrinterTagValue[];
extern const char kPrinterRemoveTagValue[];
extern const char kPrinterLocalSettingsValue[];
extern const char kMessageTextValue[];

extern const char kPrintSystemFailedMessageId[];
extern const char kGetPrinterCapsFailedMessageId[];
extern const char kEnumPrintersFailedMessageId[];
extern const char kZombiePrinterMessageId[];

extern const char kSuccessValue[];
extern const char kNameValue[];
extern const char kDisplayNameValue[];
extern const char kIdValue[];
extern const char kTicketUrlValue[];
extern const char kFileUrlValue[];
extern const char kPrinterListValue[];
extern const char kJobListValue[];
extern const char kTitleValue[];
extern const char kPrinterCapsHashValue[];
extern const char kTagsValue[];
extern const char kXMPPJidValue[];
extern const char kOAuthCodeValue[];
extern const char kCreateTimeValue[];
extern const char kPrinterTypeValue[];
extern const char kUserValue[];
extern const char kUsersValue[];
extern const char kLocalSettingsPendingXmppValue[];

extern const char kNotificationUpdateSettings[];

extern const char kChromeVersionTagName[];
extern const char kSystemNameTagName[];
extern const char kSystemVersionTagName[];

extern const char kCloudPrintServiceProxyTagPrefix[];
extern const char kCloudPrintServiceTagsHashTagName[];
extern const char kCloudPrintServiceTagDryRunFlag[];

extern const char kJobFetchReasonStartup[];
extern const char kJobFetchReasonPoll[];
extern const char kJobFetchReasonNotified[];
extern const char kJobFetchReasonQueryMore[];
extern const char kJobFetchReasonFailure[];
extern const char kJobFetchReasonRetry[];

extern const char kCreateLocalSettingsXmppPingFormat[];
extern const char kUpdateLocalSettingsXmppPingFormat[];

const int kJobDataMaxRetryCount = 1;
const int kCloudPrintAPIMaxRetryCount = -1;
const int kCloudPrintRegisterMaxRetryCount = -1;
const int kCloudPrintAuthMaxRetryCount = -1;

const int kMinJobPollIntervalSecs = 5*60;  
const int kMaxJobPollIntervalSecs = 8*60;  

const int kDefaultXmppPingTimeoutSecs = 5*60;
const int kMinXmppPingTimeoutSecs = 1*60;
const int kXmppPingCheckIntervalSecs = 60;

const int kMaxFailedXmppPings = 2;

const int kTokenRefreshGracePeriodSecs = 5*60;  

const int kNumRetriesBeforeAbandonJob = 5;

const int kJobFirstWaitTimeSecs = 1;

const int kJobWaitTimeExponentialMultiplier = 2;

}  

#endif  
