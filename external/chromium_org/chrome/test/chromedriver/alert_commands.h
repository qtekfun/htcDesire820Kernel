// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_ALERT_COMMANDS_H_
#define CHROME_TEST_CHROMEDRIVER_ALERT_COMMANDS_H_

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
class Value;
}

struct Session;
class Status;
class WebView;

typedef base::Callback<Status(
    Session* session,
    WebView* web_view,
    const base::DictionaryValue&,
    scoped_ptr<base::Value>*)> AlertCommand;

Status ExecuteAlertCommand(
     const AlertCommand& alert_command,
     Session* session,
     const base::DictionaryValue& params,
     scoped_ptr<base::Value>* value);

Status ExecuteGetAlert(
    Session* session,
    WebView* web_view,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteGetAlertText(
    Session* session,
    WebView* web_view,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteSetAlertValue(
    Session* session,
    WebView* web_view,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteAcceptAlert(
    Session* session,
    WebView* web_view,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteDismissAlert(
    Session* session,
    WebView* web_view,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

#endif  
